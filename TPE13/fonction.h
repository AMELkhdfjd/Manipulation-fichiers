
#ifndef FONCTION_H_INCLUDED
#define FONCTION_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

#define TAILLEBLOC 30
#define LGR_CHAINE 50
#define MAX_CHEVAUCHEMENT 10

// Compactage d'un fichier T�VC (EXO13)

/*D�claration__Structures*/

typedef struct Tbloc{
    char tab[TAILLEBLOC];
}Tbloc;

typedef Tbloc Buffer;

typedef struct Entete
{
    int NumDernierBloc;          /*num�ro du dernier bloc*/
    int PosLibre;                       /*Premi�re position libre dans le dernier bloc*/
    int nbIns;                           /*nombre total d'enregistrements ins�r�s*/
    int nbSup;                          /*nombre d'�l�ments supprim�s logiquement*/
} Entete;

typedef struct TnOVC
{
    FILE *F;
    Entete Entete;
} TnOVC;

TnOVC* Ouvrir(char* filename, char mod);

void Fermer(TnOVC* Ptr);

int entete(TnOVC* Ptr, int i);

void aff_entete(TnOVC* Ptr, int i, int val);

void lireDir(TnOVC *Ptr, int i, Buffer* buf);      /*proc�dure pour la lecture d'un bloc*/

void ecrireDir(TnOVC *Ptr, int i, Buffer buf);     /*proc�dure pour l'�criture d'un bloc*/

int allocBloc(TnOVC *Ptr);      /*allocation d'un bloc*/
char* RecupChaine(TnOVC* Fich, int* i, int* j, Buffer* Buf, int taille);

// ***
TnOVC *chargement_initial(char *nomfich,int n);



void AfficheTout(TnOVC* Fich);
void compactage(TnOVC *fic);
void Affichage_Entete(TnOVC * fichier);


#endif // FONCTION_H_INCLUDED
