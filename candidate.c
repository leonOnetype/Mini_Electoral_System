#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "helper.h"
#include "candidate.h"


/// Fonction principale
void candidateManagement(void)
{
    //Prerequis: aucun
    //Action: gère tout la partie du programme concernant les candidates
    //Idée:

    char choix = '\0';
    int id = 0;
    FILE* fichier = NULL;

P:
    printf("\t\t               Menu :\n");
    color(14, 0);
    printf("\t\t                     1)-View the list of registered candidate\n");
    printf("\t\t                     2)-Add a new candidate\n");
    printf("\t\t                     3)-Edit candidate's informations\n");
    printf("\t\t                     4)-Quit\n");
    color(15, 0);

    fflush(stdin);

    printf("\t\t Your choice: ");
    scanf("%c",&choix);
    while(!strchr("1234", choix)) // on ne considere que les choix compris entre 1 et 4
    {
        printf("\n\t Wrong entry, please make your choice again: ");
        fflush(stdin);
        scanf("%c",&choix);
    }
    getchar();

    switch(choix)
    {
    case'1':
        fichier = fopen("candidate.txt", "r");
        color(7, 0);
        printf("\n\t-------------------------------> List of registered candidate\n");
        color(15, 0);

        if(!viewRegisteredCandidates(fichier))
        {
            printf("\a\t\t\t       No registered candidate yet. \n");
        }

        color(7, 0);
        printf("\t----------------------------------------------------------------\n");
        color(15, 0);

        fclose(fichier);
        system("pause");
        system("cls");
        fflush(stdin);
        goto P;
        break;

    case '2':
        system("cls");
        color(7, 0);

        printf("\n\n\t\t-------------------------------------> Adding a candidate\n");

        int p = false;
        fichier = fopen("constantes.txt" , "r");
        rewind(fichier);
        fscanf(fichier ,"%d" , &p );
        fclose(fichier);

        // Le vote à déjà eu lieu?
        if(p)// <-- Oui
        {
            color(12, 0);
            printf("\n\t\t\a Impossible to add a new candidate, the voting has already done.\n\n");
        }

        else // <-- Non (alors on enregistre le candidate)
        {

            color(15, 0);
            fichier = fopen("candidate.txt", "a+");
            addCandidate(fichier, fichier,false, 0 );
            fclose(fichier);
        }

        color(7, 0);
        printf("\t\t----------------------------------------------------------------\n");
        color(15, 0);

        system("pause");
        system("cls");
        fflush(stdin);
        goto P;
        break;

    case '3':
        color(7, 0);
        printf("\t\t-------------------------------------> Editing a candidate\n");
        color(15, 0);

        if(!editCandidate())
            printf("\t\t  No candidate is registered yet.\n");

        color(7, 0);
        printf("\t---------------------------------------------------------------\n");
        color(15, 0);

        system("pause");
        system("cls");
        fflush(stdin);
        goto P;
        break;

    case '4':
        break;
    }
}


void addCandidate(FILE* enregFile, FILE* sourceFile, int option , int nb_of_lane)
{
    //Prerequis: deux fichiers et une option necessaire dans le cas ou on modifie un candidate
    //Action :
    //Idée: enregistrer dans un fichier universel une structure contenant les infos du candidate


    // creation d'un candidate vide(abus de language) et recolte des données du candidate
    Candidate candidate;
    int taille = 0, stop = false, i = 0;

    printf("\t\t Name: ");
    color(7, 0);
    fgets(candidate.name, TAILLE_MAX, stdin);
    color(15,0);
    treatTexte(candidate.name);

    printf("\t\t Last name: ");
    color(7, 0);
    fgets(candidate.last_name, TAILLE_MAX, stdin);
    color(15, 0);
    treatTexte(candidate.last_name);

    printf("\t\t Sex: ");
    char c = '\0';
    candidate.sex = getCaracter("fFmM", "Sex");
    // on s'assure que le sexe est soit F/f ou alors M/m


    printf("\t\t CNI number: ");
    color(7, 0);
    fgets(candidate.CNI_number, 10, stdin);
    color(15, 0);
    treatTexte(candidate.CNI_number);

    candidate.login_number = numberOfRegisteredCandidate(enregFile) + 1 ;

    candidate.nb_of_lane = nb_of_lane;

    // on teste si une personne ne s'est pas déja enregistrer avec une de ces infos.
    switch(candidateIsDouble(sourceFile, candidate, option))
    {
    case 'n':
        color(9, 0);
        printf("\a\t\t A candidate is already registered with this name: %s %s\n",candidate.last_name, candidate.name);
        printf("\t\t---------------------- candidate no added ---------------------- \n");

        break;
    case 'i':
        color(9, 0);
        printf(L"\a\t\t A candidate is already registered with this CNI number: %s\n",candidate.CNI_number);
        printf("\t\t---------------------- candidate no added---------------------- \n");
        break;
    default:// on sauvegarde les info du candidate
        fwrite(&candidate, sizeof(Candidate), 1, enregFile);
        color(7, 0);
        if(option)
            printf("\a\t\t--------------------------- candidate edited ---------------------------- \n");
        else
            printf("\a\t\t--------------------------- candidate added ---------------------------- \n");
        break;
    }
    color(15, 0);
}

int numberOfRegisteredCandidate(FILE* fichier)
{
    //Prerequis: le fichier contenant tous les candidates ouvert en mode lectire
    //Action: compte le nombre de structures dans le fichier et renvoi le resultat trouver
    //Idee: Incrementer un compteur a chaque fois q'on tombe sur une structure non vide


    rewind( fichier);
    Candidate tmp_candidate;
    int counter = 0;

    while(fread(&tmp_candidate, sizeof(Candidate), 1, fichier))
    {
        counter++;
    }

    return counter;
}

char candidateIsDouble(FILE* fichier, Candidate candidate, int option)
{
    //Prerequis: le fichier contenant tous mes candidates et le candidate en question
    //Action: cherche s'il ya déja une occurence du candidate q'on souhaite enregistrer

    rewind(fichier); // ca ne derangera pas puisque on ecrit a la fin et le comptage va nous la fin du fichier
    Candidate tmp_candidate;

    while(fread(&tmp_candidate, sizeof(Candidate), 1, fichier))
    {

        if(!strcmp(tmp_candidate.name, candidate.name) && !strcmp(tmp_candidate.last_name, candidate.last_name) && option)
        {
        }
        if(!strcmp(tmp_candidate.name, candidate.name) && !strcmp(tmp_candidate.last_name, candidate.last_name) && !option)
            return 'n';

        if(!strcmp(tmp_candidate.CNI_number, candidate.CNI_number) && option)
        {
        }
        if(!strcmp(tmp_candidate.CNI_number, candidate.CNI_number) && !option)
            return 'i';
    }

    return 'o';

}

int viewRegisteredCandidates(FILE* fichier)
{
    //Prerequis: le fichier contenant la liste des candidates ouvert en mode lecture
    //Action: affiche tous les candidates enregistrés
    //Idee: lecture du fichier contenant les infos sur les candidates

    if(!fichier)
        return false;

    rewind(fichier);

    Candidate tmp_candidate;
    printf("  ---------------------------------------------------------------------------------------------------------------------\n");
    wprintf(L" |  Number N%lc  | Login number |     CNI number   |            Name            |              Last name          | Sex\n",167);
    printf("  ---------------------------------------------------------------------------------------------------------------------\n");

    while(fread(&tmp_candidate, sizeof(Candidate), 1, fichier))
    {
        printf("\t %-6d|", tmp_candidate.login_number);
        printf("        %-6d|    %-14s|         %-19s|         %-21s   |  %c\n",tmp_candidate.login_number,
               tmp_candidate.CNI_number,tmp_candidate.name, tmp_candidate.last_name, toupper(tmp_candidate.sex));
        printf("  ----------------------------------------------------------------------------------------------------------------------\n");
    }

    return true;
}

int editCandidate(void)
{
    //Prerequis: l'identifiant du candidate et le fichier contenant les candidates ouvert en mode lecture
    //Action: modifie les informations relatifs a un candidate
    //Idee:

    remove("tmpFileCandidate.txt");

    FILE* fichier = fopen("candidate.txt", "r");
    FILE* fichier1 = fopen("tmpFileCandidate.txt", "a+");
    int id = 0;

    if(!fichier)
    {
        fclose(fichier1);
        fclose(fichier);
        remove("tmpFileCandidate.txt");
        return false;
    }

    if(!viewRegisteredCandidates(fichier)) //<-- On tente d'afficher la liste des candidates
    {
        fclose(fichier);
        fclose(fichier1);
        remove("tmpFileCandidate.txt");
        return false;
    }

    printf("\t\t Enter your login number: ");
    id = getNumber();

    rewind(fichier);
    rewind(fichier1);

    Candidate tmp_candidate;
    char continuer ='\0';

    while(id > numberOfRegisteredCandidate(fichier))
    {
        printf("\n\t\t Wrong login number. Please try again.\n ");
        printf("\t\t Login number: ");
        id = getNumber();
    }

    // on remet le curseur virtuel au bon endroit
    rewind(fichier);

    // on copie des infos qui vienent avant celles de notre candidate les met dans un autre fichier
    int i=0;
    for(i = 1 ; i<id ; i++)
    {
        fread(&tmp_candidate, sizeof(Candidate), 1, fichier);
        fwrite(&tmp_candidate, sizeof(Candidate), 1, fichier1);
    }
    // on recupère les infos de notre candidate
    fread(&tmp_candidate, sizeof(Candidate), 1, fichier);

    // on procède a la modefication
    color(9, 0);
    printf("\n\t\t  You are trying to change the informations of : ");
    printf("%s %s \n",tmp_candidate.last_name, tmp_candidate.name );
    printf("\t\t Are you sure you want to continue? y/n ? \n");
    color(8, 0);

    char c = '\0';
    continuer = '\0';
    for(i=0 ; i<1 ; i++) // on s'assure que le sexe est soi F/f ou alors M/m
    {
        fflush(stdin);
        c = getch();
        if(c == 8)
        {
            if(continuer  == '\0')
            {}
            else
            {
                printf("\b \b");
                continuer = '\0';
            }
            i-=2; // petite precausion pour etre sure qu'on va recommancer la boucle (en fait on ne fait qu'un seul tour )
        }
        else
        {
            if(!strchr("ynYN", c))
                 i -=2;
            else
            {
                continuer = c;
                 i = 1;
            }

        }
    }

    if(continuer == 'y' || continuer == 'Y')
    {
        color(15, 0);
        printf("\n\t Personal informations %s %s:\n",tmp_candidate.last_name, tmp_candidate.name);
        color(8, 0);
        printf("\t\t---------------------------------------------\n");
        printf("\t\t Login number:%d \n", tmp_candidate.login_number);
        printf("\t\t -->Nane:%s\n", tmp_candidate.name);
        printf("\t\t -->Last name:%s\n", tmp_candidate.last_name);
        printf("\t\t -->Sex:%c\n", tmp_candidate.sex);
        printf("\t\t -->CNI number:%s \n\n", tmp_candidate.CNI_number);
        printf("\t\t---------------------------------------------\n");
        color(15,0);
        printf("\t Enter new informations:\n");

        rewind(fichier1);
        addCandidate(fichier1, fichier, 1 , tmp_candidate.nb_of_lane);   // on active l'option de modification de candicat

        //on remet me curseur virtuel au bon endroit car la fonction precedente va l'eu mettre a la fin du fichier
        rewind(fichier);
        int tmp_id = id;
        while(fread(&tmp_candidate, sizeof(Candidate), 1, fichier) && tmp_id > 1)
            tmp_id--;

        // on continue la lecture
        while(fread(&tmp_candidate, sizeof(Candidate), 1, fichier))
            fwrite(&tmp_candidate, sizeof(Candidate), 1, fichier1);

        fclose(fichier);
        fclose(fichier1);
        remove("candidate.txt");
        rename("tmpFileCandidate.txt","candidate.txt");

    }
    else
    {
        fclose(fichier);
        fclose(fichier1);
        remove("tmpFileCandidat.txt");
    }
    return true;
}

