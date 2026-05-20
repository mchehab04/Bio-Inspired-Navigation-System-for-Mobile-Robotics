#include <stdio.h>
#include <math.h>

#define PI 3.1415926535

int main (int argv, char *argc[])
{
  float a, b, c, borne_inf, borne_sup, pas, x, gauss;
  int nb_echt;
  
  if(argv < 6)
  {
    printf("erreur dans le nombre d'argument\n");
    printf("l'equation de la gaussienne est a*exp((x-b)²/(2c²))\n");
    printf("vous devez specifier les valeurs a, b, c et l'intervalle de calcul\n");
    printf(" ainsi que le nombre d'echantions desires\n");
    printf("ex: gauss 1 0 1 -3 3 11\n\n");
    printf("note: si vous voulez une gaussienne normalisee il faut choisir a=1/(c*sqrt(2*Pi))\n");
    printf("vous pouvez tapper un n a la place de a pour obtenir ce resultat\n");
    printf("ex: gauss n 0 1 -3 3 11\n\n");
    return 1;
  }
  
  
  if((sscanf(argc[1], "%f", &a) != 1) && (argc[1][0]!='n'))
  {
    printf("erreur: impossible d'interpreter le premier argument '%s'\n", argc[1]);
    return 1;
  }
  if(sscanf(argc[2], "%f", &b) != 1)
  {
    printf("erreur: impossible d'interpreter le deuxieme argument '%s'\n", argc[2]);
    return 1;
  }
  if(sscanf(argc[3], "%f", &c) != 1)
  {
    printf("erreur: impossible d'interpreter le troisieme argument '%s'\n", argc[3]);
    return 1;
  }
  if(sscanf(argc[4], "%f", &borne_inf) != 1)
  {
    printf("erreur: impossible d'interpreter le quatrieme argument '%s'\n", argc[4]);
    return 1;
  }
  if(sscanf(argc[5], "%f", &borne_sup) != 1)
  {
    printf("erreur: impossible d'interpreter le cinquieme argument '%s'\n", argc[5]);
    return 1;
  }
  if(sscanf(argc[6], "%i", &nb_echt) != 1)
  {
    printf("erreur: impossible d'interpreter le sixieme argument '%s'\n", argc[6]);
    return 1;
  }
  
  if(argc[1][0]=='n')
  {
    a = 1 / (c * sqrt(2.0 * PI));
  }
  
  pas = (borne_sup - borne_inf) / (nb_echt - 1);

  //printf("a=%f, b=%f, c=%f, borne inf=%f, borne sup=%f, pas=%f\n", a, b, c, borne_inf, borne_sup, pas);
  
  for(x=borne_inf; nb_echt>0; x+=pas, nb_echt--)
  {
    gauss = a * expf(-(x-b)*(x-b) / (2.0 * c * c));
    printf("%f ", gauss);
  }
  
  return 0;
}
