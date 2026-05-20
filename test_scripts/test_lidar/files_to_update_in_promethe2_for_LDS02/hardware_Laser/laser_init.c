/*
Copyright  ETIS — ENSEA, Université de Cergy-Pontoise, CNRS (1991-2014)
promethe@ensea.fr

Authors: P. Andry, J.C. Baccon, D. Bailly, A. Blanchard, S. Boucena, A. Chatty, N. Cuperlier, P. Delarboulas, P. Gaussier, 
C. Giovannangeli, C. Grand, L. Hafemeister, C. Hasson, S.K. Hasnain, S. Hanoune, J. Hirel, A. Jauffret, C. Joulain, A. Karaouzène,  
M. Lagarde, S. Leprêtre, M. Maillard, B. Miramond, S. Moga, G. Mostafaoui, A. Pitti, K. Prepin, M. Quoy, A. de Rengervé, A. Revel ...

See more details and updates in the file AUTHORS 

This software is a computer program whose purpose is to simulate neural networks and control robots or simulations.
This software is governed by the CeCILL v2.1 license under French law and abiding by the rules of distribution of free software. 
You can use, modify and/ or redistribute the software under the terms of the CeCILL v2.1 license as circulated by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
As a counterpart to the access to the source code and  rights to copy, modify and redistribute granted by the license, 
users are provided only with a limited warranty and the software's author, the holder of the economic rights,  and the successive licensors have only limited liability. 
In this respect, the user's attention is drawn to the risks associated with loading, using, modifying and/or developing or reproducing the software by the user in light of its specific status of free software, 
that may mean  that it is complicated to manipulate, and that also therefore means that it is reserved for developers and experienced professionals having in-depth computer knowledge. 
Users are therefore encouraged to load and test the software's suitability as regards their requirements in conditions enabling the security of their systems and/or data to be ensured 
and, more generally, to use and operate it in the same conditions as regards security. 
The fact that you are presently reading this means that you have had knowledge of the CeCILL v2.1 license and that you accept its terms.
 */
#include <Laser.h>
#include "tools/include/local_var.h"
#include <net_message_debug_dist.h>
#include <Serial.h>


void (*lh_urg_disconnect)(urg_t *urg);
const char * (*lh_urg_error)(urg_t *urg);
int (*lh_urg_connect)(urg_t *urg, const char *device, long baudrate);
int (*lh_urg_dataMax)(urg_t *urg);
int (*lh_urg_requestData)(urg_t *urg,urg_request_type request_type,int first_index,int last_index);
int (*lh_urg_receiveData)(urg_t *urg, long data[], int data_max);
long (*lh_urg_maxDistance)(urg_t *urg);
long (*lh_urg_minDistance)(urg_t *urg);
int (*lh_urg_deg2index)(urg_t *urg, int degree);
int (*lh_urg_parameters)(urg_t*urg ,urg_parameter_t*param);

void laser_init_USE_SICK_LMS(Laser * laser);
void laser_init_USE_URG_04LX(Laser * laser);

void laser_init(Laser *laser )
{
   float seuil_min,seuil_max;

   if(laser->laser_type==USE_SICK_LMS)
   {
      laser_init_USE_SICK_LMS(laser);
   }
   else if(laser->laser_type==USE_URG_04LX)
   {
      laser_init_USE_URG_04LX(laser);
   }

   seuil_min=laser->seuil_min;
   seuil_max=laser->seuil_max;

   if (seuil_min >= seuil_max) {  /* pg: avant test equalite mais est ce utile? + pb float */
      printf("ERREUR : 'seuil max' <= 'seuil min' dans laser_init %s \n",laser->name);
      exit(0);
   }

}

void laser_init_USE_SICK_LMS(Laser *laser)
{
    /* LDS-02 is 360°; keep the "SICK" entry point but allocate 360 bins. */
    laser->nb_values = 360;
    laser->values = (long *)malloc(sizeof(long) * laser->nb_values);

#ifdef DEBUG
    printf("laser_connecting (LDS-02 via LMS path)\n");
#endif

    /* We pass the same params; connectToLMS() now simply opens at 115200. */
    laser->fd = connectToLMS(RANGE_180, RES_1_DEG, CMMODE, laser->port, BAUD_38400);
}

void laser_init_USE_URG_04LX(Laser *laser) 
{	
   int ret = 0;

   /*Allocation des pointeurs de la structure*/
   laser->urg=(urg_t*)malloc(sizeof(urg_t));
   laser->urg_parameters=(urg_parameter_t*)malloc(sizeof(urg_parameter_t));

   /*Connection au telemetre URG*/
   ret = (*lh_urg_connect)(laser->urg, laser->port, 115200);

   if (ret != 0)
   {
      urg_exit(laser->urg, "lh_urg_connect(): unable to connect");
   }
   printf("connection to %s is established \n", laser->name);


   /*Recuperation des parametres et allocation du tableau des valeurs*/
   ret = (*lh_urg_parameters)(laser->urg, laser->urg_parameters);
   if (ret != 0)
   {
      urg_exit(laser->urg, "lh_urg_parameters()");
   }
   laser->nb_values = (*lh_urg_dataMax)(laser->urg);
   laser->values=(long * ) malloc(sizeof(long) * laser->nb_values);
   printf("nb_values of laser = %i\n", laser->nb_values);
}


void* laser_capture_all_laser(void* laser_tabl)
{
   Laser ** laser_table = (Laser ** )laser_tabl;
   int i;
   static int init_thread=0;
   /**/
   for(i=0;i<nb_laser;i++)
   {
      laser_table[i]->enabled=1;
   }

   while(laser_table[0]->enabled == 1)
   {
      for(i=0;i<nb_laser;i++)
      {
         laser_read(laser_table[i]);
         if(init_thread == 0)
         {
            pthread_mutex_unlock(&(laser_table[i]->mutex_laser_start));
            init_thread=1;
         }
      }
   }

   for(i=0;i<nb_laser;i++)
   {
      laser_table[i]->enabled=0;
   }

   return 0;
}
