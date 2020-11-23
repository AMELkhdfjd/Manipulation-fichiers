#include <stdio.h>
#include <stdlib.h>
#include "fonction.h"
int main()
{

   int choix , i,nb;
   char  nom[20], mode[20];
   TnOVC *f;
   printf("Op%crations d'acc%cs sur un Fichier de type TnOVC\n",130,138);
   printf("Capacit%c maximale des blocs = %d caract%cres\n",130, TAILLEBLOC,138);
   printf("Donnez le nom du fichier : \n");
   scanf(" %s", nom);
   printf("Ancien ou Nouveau ? (a/n) : ");
   scanf(" %s", mode);
   if ( mode[0] == 'a' || mode[0] == 'A' )
   {


	 f = Ouvrir( nom , 'A');
   }
   else {

	printf("**************chargement initial**************\n");
	printf("Donner le nombre d'enregistrement que vous voulez ins%crer dans votre fichier SVP !!\n",130);
	scanf("%d",&nb);
	 f = chargement_initial(nom,nb);



   }


   // Menu principal ...
   do {
    printf("\n--------- M E N U ---------\n");
	printf("1) Afficher l'ent%cte du fichier\n",136);
	printf("2) compactage de fichier\n");
	printf("3) Affichage de bloc(s)\n");
	printf("4) Afficher le co%ct \n",150);
	printf("0) Quitter le programme\n");
	printf("\tchoix : ");
	scanf(" %d", &choix);
	printf("\n---------------------------\n\n");

	switch(choix) {
	   case 1:
	       printf("l'ent%cte du fichier est : ",136);
	       Affichage_Entete(f);

		   break;
	  case 2:
	      compactage(f);
	       break;
	  case 3:
	      printf("les enregistrements de votre fichier sont : ");
           AfficheTout(f);
           break;
      case 4:
           printf("le co%ct du compactage est : \n",150 );
           printf("le nombre de lectures phisiques est :%d\n ",nbLecDir());
           printf("le nombre d'%ccritures phisiques est :%d\n ",130,nbEcrDir());
           break;

	}
   } while ( choix != 0);

   // Fermeture du fichier (sauvegarde de l'entete) ...
   Fermer(f);

   return 0;


}
