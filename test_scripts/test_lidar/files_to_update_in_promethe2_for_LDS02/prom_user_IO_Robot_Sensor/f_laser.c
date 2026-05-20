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
#include <libx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <Kernel_Function/find_input_link.h>
#include <Kernel_Function/prom_getopt.h>
#include <libhardware.h>

#define DEBUG



void function_laser(int gpe)
{

	
	int i, last_neuron;
	double seuil_max,seuil_min,tmp;
	Laser *laser=laser_get_first_laser();
	
	seuil_min=laser->seuil_min;
	seuil_max=laser->seuil_max;
	
	
	last_neuron=def_groupe[gpe].premier_ele+def_groupe[gpe].nbre;
	for (i = 0; i < def_groupe[gpe].nbre; i++)
	{
	      tmp=laser_get_intensity_by_degres( laser, i * 360. / ((float)(def_groupe[gpe].nbre ) ) -180.);	
	      //cprints("tmp= %f", (float)tmp);
	      if (tmp<=0) tmp = 0.;
		else if ( tmp > seuil_max ) {
			  //cprints("*******************************MAAAAAXXXXXXXXXXXXXX****************");

			tmp=0.;
		} else if ( tmp < seuil_min ) {
			//cprints("*******************************MIIIIIIIIIIIIINNNNNNNNNNNNN****************");

			tmp=1.;
			
		} else {
			//cprints("*******************************ELSEEEEEEEEEEEEEEEE****************");

			
			tmp=1.-((tmp-seuil_min)/(seuil_max-seuil_min));
			
		}
		neurone_s[last_neuron-i]=neurone_s1[last_neuron-i]=neurone_s2[last_neuron-i]=(float)tmp;
	}
}

 

void function_laser_LMS(int gpe)
{
	int cpt,i,j; /*,n;*/
	double val,seuil_max,seuil_min,tmp;
	double proximite[180];
	Laser *laser=laser_get_first_laser();
	
	if ( def_groupe[gpe].nbre != 61 ) { /* utile pour notre script, mais a modifier ensuite*/
		printf("#%s# WARNING : il y a %d neurones pour ce groupe. Il faut 61 neurones. \n", __FUNCTION__, def_groupe[gpe].nbre);
		exit(0);
	}
	
	for(i=0;i<def_groupe[gpe].nbre;i++)
	{
	  neurone_s[def_groupe[gpe].premier_ele+i]=neurone_s1[def_groupe[gpe].premier_ele+i]=neurone_s2[def_groupe[gpe].premier_ele+i]=0.;
	}

/* 	n=laser_read(laser);*/
	
	seuil_min=laser->seuil_min;
	seuil_max=laser->seuil_max;
	
	if (seuil_min >= seuil_max) {  /* pg: avant test equalite mais est ce utile? + pb float */
		printf("ERREUR : 'seuil max' <= 'seuil min' \n");
		exit(0);
	}
	
	/*conversion des dsitances mesurees en vecteur de proximite */
	for(i=0;i<180;i++) {
		val=laser->values[i];
		
		if ( seuil_max-val < 0 ) {
			tmp=0;
		} else if ( val <= seuil_min ) {
			tmp=1;
		} else {
			tmp=1-((val-seuil_min)/(seuil_max-seuil_min));
		}
		
		proximite[i]=tmp;
		/*printf("%f ", tmp);*/
	}
	/*printf("\n\n");*/
	
	cpt = 0;
	/*code pour etendre un spike en un creneau de largeur de 6 neurones pour detecter les objets fins type poteau...*/
	for (i = def_groupe[gpe].premier_ele; i < def_groupe[gpe].premier_ele + def_groupe[gpe].nbre; i++) {
		if ( i < 15 || i > 45 || cpt>177) {
			tmp=0;
		} else {
			cpt+=6;
			tmp=0;
			for(j=-3;j<3;j++) {
				if ( proximite[cpt+j] > tmp)
					tmp=proximite[cpt+j]; /*recherche du max*/
			}
			/*tmp=(proximite[cpt-3]+proximite[cpt-2]+proximite[cpt-1]+proximite[cpt]+proximite[cpt+1]+proximite[cpt+2])/6.;*/
		}
		/*printf("%f ", tmp);*/
		neurone_s[def_groupe[gpe].nbre-i]=neurone_s1[def_groupe[gpe].nbre-i]=neurone_s2[def_groupe[gpe].nbre-i]=tmp;
	}
	/*printf("\n\n");*/
	
}
