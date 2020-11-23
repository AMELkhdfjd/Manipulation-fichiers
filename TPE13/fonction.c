#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonction.h"

int nbLecturesDir=0;
int nbEcrituresDir=0;


// Compactage d'un fichier TÔVC (EXO13)

TnOVC *Ouvrir(char *filename,char mod)    /*Ouverture du fichier selon mod, si c'est 'A' ou 'a' => ouverture d'un ancien fichier, sinon => ouverture d'un nouveau fichier*/

{
        TnOVC (*Ptr)=malloc(sizeof(TnOVC));

        if ((mod == 'A') || (mod =='a')) (Ptr->F)=fopen(filename,"rb+");
        else (Ptr->F)=fopen(filename,"wb+");

        if ((mod == 'A') || (mod =='a'))                     /*Ouverture d'un ancien fichier*/
        {
            fread(&(Ptr->Entete), sizeof(Entete), 1, Ptr->F);
        }
        else
        {
            /* Initialisation de l'Entete03*/
            (Ptr->Entete).NumDernierBloc=0;
            (Ptr->Entete).PosLibre=0;
            (Ptr->Entete).nbIns=0;
            (Ptr->Entete).nbSup=0;
            fwrite(&(Ptr->Entete),sizeof(Entete),1,Ptr->F);
        }
        return Ptr;
}

void Fermer(TnOVC *Ptr)     /*Fermeture d'un fichier et sauvegarde de l'Entete03*/
{
        fseek(Ptr->F,0,0);
        fwrite(&(Ptr->Entete),sizeof(Entete),1,Ptr->F);  /* Mise a jour de l'entête*/
        fclose(Ptr->F);
        Ptr=NULL;
}

int entete(TnOVC *Ptr, int i)   /*fonction qui retourne les paramètres de l'Entete03*/
{
        if (i==1) return ((Ptr->Entete).NumDernierBloc);
        if (i==2) return ((Ptr->Entete).PosLibre);
        if (i==3) return ((Ptr->Entete).nbIns);
        return ((Ptr->Entete).nbSup);
}

void aff_entete(TnOVC *Ptr, int i, int val)      /*procédure pour la modification des paramètres de l'Entete03*/
{
        if (i==1) (Ptr->Entete).NumDernierBloc=val;
        else if (i==2) (Ptr->Entete).PosLibre=val;
        else if (i==3) (Ptr->Entete).nbIns=val;
        else (Ptr->Entete).nbSup=val;
}

void lireDir(TnOVC *Ptr, int i, Buffer* buf)       /*procédure pour la lecture d'un bloc*/
{
        if (i<=entete(Ptr, 1))            /*si le numéro de bloc < nb de blocs total*/
        {
            nbLecturesDir++;
            fseek(Ptr->F, sizeof(Entete)+(i-1)*sizeof(Tbloc), SEEK_SET);        /*On dépasse l'Entete03 + (i-1) premiers blocs*/
            fread(buf, sizeof(Tbloc), 1, Ptr->F);
        }
}

void ecrireDir(TnOVC *Ptr, int i, Buffer buf)     /*procédure pour l'écriture d'un bloc*/
{
//
            nbEcrituresDir++;
            fseek(Ptr->F, sizeof(Entete)+(i-1)*sizeof(Tbloc), SEEK_SET);
            fwrite(&buf, sizeof(Tbloc), 1, Ptr->F);
//
}

int allocBloc(TnOVC *Ptr)        /*allocation d'un bloc*/
{
        aff_entete(Ptr, 1, entete(Ptr, 1)+1);
        return entete(Ptr,1);
}



void AfficheTout(TnOVC* Fich)
{
    int i=1, j=0; Buffer Buf;

    lireDir(Fich, i, &Buf);
    while ( i < entete(Fich, 1) || (i == entete(Fich,1) && j != entete(Fich,2)) )
    {
        printf("%c", Buf.tab[j]);
        j++;
        if(j>=TAILLEBLOC)
        {
            j=0; i++; lireDir(Fich, i, &Buf); printf("_");
        }
    }
    printf("***\n");

}


TnOVC *chargement_initial(char *nomfich,int n)
{


       int i,j,k,stop;
       TnOVC *f;
       Buffer buf;
       char ch[50];
       f= Ouvrir(nomfich, 'N' );// un nouveau fichier
       i = 1; // num de bloc à remplir
       j = 0;  // num d'enreg dans le bloc

       int z = 0; /// contient le nombre d'enregistrements effacés logiquement.
    printf("Donner votre enregistrement suivant les r%cgles :\n ",138);
    printf("1)_ le premier caract%cre de l'enregistrement doit %ctre 1 s'il est effac%c 0 sinon.\n ",138,136,130);
    printf("2)_les 5 caract%cres suivants doivent %ctre la taille de votre enregistrement le caract%cre d'effacement est compris dans la taille de l'enregistrement.\n",138,136);
    printf("3)_ le reste de votre enregistrement.\n");
    for(k=1;k<=n;k++)
    {
        printf("Donner votre enregistrement  :  ");
        scanf("%s",ch);
        if(ch[0] == '1')
        {
            z++;//incrémenter le nombre d'enregistrements effacés logiquement.
        }
        for(int w = 1;w<=strlen(ch);w++)
        {

            buf.tab[j]  = ch[w-1];
            //printf("%d  \t  %d\n",buf.tab[j],j);
            j++;
            if(j == TAILLEBLOC)
            {
                ecrireDir(f,i,buf);
                printf("ecriture bloc \n");
                i++;
                j=0;

            }
        }



    }
    if (j > 0 ) ecrireDir(f,i,buf);///on ecrit le dernier enregistrement et on mis à jour l'entête.
    else i--;
///mise à jour de l'entête.
    aff_entete(f,1,i);
    aff_entete(f,2,j);
    aff_entete(f,3,n);
    aff_entete(f,4,z);
    return f;
}

char* RecupChaine(TnOVC* Fich, int* i, int* j, Buffer* Buf, int taille)
{
    char Chaine[MAX_CHEVAUCHEMENT*TAILLEBLOC];

    for(int k=0; k<taille; k++)
    {
        if((*j)<TAILLEBLOC)
        {
            Chaine[k] = Buf->tab[*j];
            *j = (*j) +1;
        }
        else
        {
            *i = (*i) +1;
            lireDir(Fich, *i, Buf);
            *j =1; Chaine[k] = Buf->tab[0];
        }
    }
    Chaine[taille] = '\0';
    char* Ch=malloc(sizeof(char)*MAX_CHEVAUCHEMENT*TAILLEBLOC);
    strcpy(Ch, Chaine);
    return Ch;
}



void compactage(TnOVC *f)
{
    int i=1,j=0,k=0,s=1,z=0;
    Buffer buf1,buf2;
    lireDir(f,i,&buf1);
    int eff,taille;//l'effacement est sur 1 caractère il est 1 lorsque l'enr esf effacé logiquement 0 sinon.
    int stop = 0;
    while(stop  == 0)
    {
        eff = atoi(RecupChaine(f,&i,&j,&buf1,1));
        taille = atoi(RecupChaine(f,&i,&j,&buf1,5));
        if(eff == 0)    ///si l'enregistrement n'est pas effacé. on ecrit dans buf2.
        {

            z++;///z contient le nombre d'enregistrement non effacés.
            if(j>=6)
            {

                j=j-6;/*on revient au début de l'enregistrement pour le copier dans buf2.*/
            }
            else
            {

                lireDir(f,i-1,&buf1);///si la fonction RecupCaine a passé dans un autre bloc en laissant quelques information de
                i--;                   ///l'enregistrement comme la taille dans le bloc précédent on relit  ce dernier à fin de les récupérer.
                j= TAILLEBLOC -(6-j);///pour revenir au début de l'enregistrement.
            }
            for(int w=1;w<taille+6;w++)
            {
                buf2.tab[k] = buf1.tab[j];
                k++;j++;
                if(j == TAILLEBLOC)
                {

                    i++;
                    if(i<=entete(f,1))
                    {
                        lireDir(f,i,&buf1);
                        j=0;
                    }
                    else
                    {
                        stop = 1;
                    }
                }
                if(k == TAILLEBLOC)
                {

                    ecrireDir(f,s,buf2);
                    k=0;
                    s++;
                }
            }

        }
        else///l'enregistrement est effacé on passe au debut de l'enregistrement suivant.
        {
            j=j+taille-1 ;/*on passe à la fin de l'enregistrement effacé.*/
            if(j>= TAILLEBLOC)
            {

                j = j-TAILLEBLOC;
                i++;
                if(i<= entete(f,1))
                {
                    lireDir(f,i,&buf1);
                }
                else
                {
                    stop = 1;
                }
            }
        }

        if(i == entete(f,1)  && j == entete(f,2)  || (i > entete(f,1)))    stop  = 1;

    }
    ///on ecrtit le dernier bloc.
    if(k >0)
    {
        ecrireDir(f,s,buf2);
    }
    else   s--;

    ///on mis à jour l'entête.
    aff_entete(f,4,0);
    aff_entete(f,3,z);
    aff_entete(f,2,k);
    aff_entete(f,1,s);
}

void Affichage_Entete(TnOVC * fichier)
{
    printf("\n Les caracteristiques du fichier ----------------------------------------\n\n");
    printf(" -> Adresse dernier bloc : %d\n",entete(fichier,1));
    printf(" -> position libre dernier bloc : %d\n",entete(fichier,2));
    printf(" -> Nombre d'enregistrements inser%cs : %d\n",130,entete(fichier,3));
    printf(" -> Nombre d'enregistrements supprim%cs : %d\n",130,entete(fichier,4));
}

int nbLecDir()
{
    return nbLecturesDir;
}

int nbEcrDir()
{
    return nbEcrituresDir;
}
