#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "helper.h"
#include "voter.h"


/// Fonction principale
void voterManagement(void)
{
    //Prerequis: aucun
    //Action: gere tout la partie du programme concernant les voters
    //Idée:

    char choix = '\0';
    FILE* fichier = NULL;


P:
    printf("\t\t               Menu de la section:\n");
    color(14, 0);
    wprintf(L"\t\t                       1)--->View the list of registered voter \n");
    wprintf(L"\t\t                       2)--->Add a new voter\n");
    wprintf(L"\t\t                       3)--->Edit a voter's informations\n");
    wprintf(L"\t\t                       4)--->Delete a voter\n");
    printf("\t\t                       5)--->Quit\n");
    color(15, 0);

    fflush(stdin);

    printf("\t\t Your choice: ");
    scanf("%c",&choix);
    while(!strchr("12345", choix)) // on ne considere que les choix compris entre 1 et 5
    {
        wprintf(L"\t Wrong entry. Please make your choice again: ");
        fflush(stdin);
        scanf("%c",&choix);
    }
    getchar();

    switch(choix)
    {
    case'1':
            color(7, 0);
        printf("\n\t----------------------------------- Registered voters  -----------------------------------\n");
        color(15, 0);

        fichier = fopen("voter.txt", "r");

        if(!viewRegisteredVoters(fichier))
         printf("\t\t\t\a            No voter registered yet.\n");

        fclose(fichier);

        color(7, 0);
        printf("\t-----------------------------------------------------------------------------------------------------\n");
        color(15, 0);

        system("pause");
        system("cls");
        fflush(stdin);

        goto P;
        break;

    case '2':
        system("cls");
        color(7, 0);
        printf("\n\n\t----------------------------------- Adding a new voter -----------------------------------\n");
        color(15, 0);

        fichier = fopen("voter.txt", "a+");
        addVoter(fichier, fichier,false );
        fclose(fichier);

        color(7, 0);
        wprintf(L"\t-----------------------------------------------------------------------------------------------------\n");
        color(15, 0);

        system("pause");
        system("cls");
        fflush(stdin);
        goto P;
        break;

    case '3':
        color(7, 0);
        printf("\t\t----------------------------------- Editing a voter --------------------------------------\n");
        color(15, 0);


        if(!editVoter())
            printf("\t\t\a---> Impossible to edit voters, no registered voters yet.\n");

        color(7, 0);
        printf("\t\t---------------------------------------------------------------------------------------------------\n");
        color(15, 0);
        fclose(fichier);

        system("pause");
        system("cls");
        fflush(stdin);

        goto P;
        break;

    case '4':
        color(7, 0);
        printf("\t\t----------------------------------- Deleting a voter -------------------------------------\n");
        color(15, 0);

        if(!Supprimervoter())
            printf("\t\t\a---> Impossible to delete voters, no registered voters yet.\n");

        color(7, 0);
        wprintf(L"\t\t------------------------------------- Voter deleted -------------------------------------. \n");
        color(15, 0);

        fclose(fichier);

        system("pause");
        system("cls");
        fflush(stdin);

        goto P;

        break;

    case '5':
        break;

    }
    color(14,0);
}


void addVoter(FILE* enregFile, FILE* sourceFile, int option)
{
    //Prerequis: deux fichier et une option necessaire dans le cas on modifie un voter
    //Action :
    //Idée: enregistrer dans un fichier universel une structure contanant les infos du voter


    // creation d'un voter vide(abus de language) et recolte des données du voter
    Voter voter;
    int taille = 0, i = 0;

    printf("\t\t Name: ");
    color(7, 0);
    fgets(voter.name, TAILLE_MAX, stdin);
    color(15, 0);
    treatTexte(voter.name);

    printf("\t\t Last name: ");
    color(7, 0);
    fgets(voter.last_name, TAILLE_MAX, stdin);
    color(15, 0);
    treatTexte(voter.last_name);

    printf("\t\t Sex: ");
    char c = '\0';
    voter.sex = getCaracter("fFmM", "Sexe");

    printf("\t\t CNI number: ");
    color(7, 0);
    fgets(voter.CNI_number, 10, stdin);
    color(15, 0);
    treatTexte(voter.CNI_number);
    voter.number = numberOfRegisteredVoter(enregFile) + 1;

    if(option==true)
        {}
    else
        voter.lane = 1;


    // on teste si une personne ne c'est pas déja enregistrer avec une de ces infos.
    switch(voterIsDouble(sourceFile, voter, option))
    {
    case 'n':
        color(9, 0);
        printf("\a\t\t A voter is already registered with this name : %s %s\n",voter.last_name, voter.name);
        printf("\t\t--------------------------------- Voter no added --------------------------------- \n");
        break;
    case 'i':
        color(9, 0);
        printf("\a\t\t A voter is already registered with this CNI number: %s\n",voter.CNI_number);
        printf("\t\t-------------------------------- Voter no added ---------------------------------- \n");
        break;
    default:// on sauvegarde les info du voter
        color(7, 0);
        fwrite(&voter, sizeof(voter), 1, enregFile);
        if(option)
            printf("\t----------------------------------- Voter edited ------------------------------------- \n");
        else
            printf("\t------------------------------------- Voter added ------------------------------------- \n");

        break;
    }
    color(15, 0);
}

int numberOfRegisteredVoter(FILE* fichier)
{
    //Prerequis: le fichier contenant tous les voters ouvert en mode lectire
    //Action: compte le namebre de structures dans le fichier et renvoi le resultat trouver
    //Idee: Incrementer un compteur a chaque fois q'on tombe sur une structure non vide


    rewind( fichier); // ca ne derangera pas puisque on ecrit a la fin et le comptage va nous la fin du fichier
    Voter tmp_voter;
    int counter = 0;

    while(fread(&tmp_voter, sizeof(Voter), 1, fichier))
    {
        counter++;
    }

    return counter;
}

char voterIsDouble(FILE* fichier, Voter voter, int option)
{
    //Prerequis: le fichier contenant tous mes voters et le voter en question
    //Action: cherche s'il ya déja une occurence du voter q'on souhaite enregistrer
    //Idee: Incrementer un compteur a chaque fois q'on tombe sur une structure non vide

    rewind( fichier); // ca ne derangera pas puisque on ecrit a la fin et le comptage va nous la fin du fichier
    Voter tmp_voter;

    while(fread(&tmp_voter, sizeof(Voter), 1, fichier))
    {
        if(!strcmp(tmp_voter.name, voter.name) && !strcmp(tmp_voter.last_name, voter.last_name) && option)
        {
        }
        if(!strcmp(tmp_voter.name, voter.name) && !strcmp(tmp_voter.last_name, voter.last_name) && !option)
            return 'n';

        if(!strcmp(tmp_voter.CNI_number, voter.CNI_number) && option)
        {
        }
        if(!strcmp(tmp_voter.CNI_number, voter.CNI_number) && !option)
            return 'i';
    }


    return 'o';

}
int viewRegisteredVoters(FILE* fichier)
{
    //Prerequis: le fichier contenant la liste des voters ouvert en mode lecture
    //Action: affiche tous les voters enregistrés
    //Idee: lecture du fichier contenant les infos sur les voters
    int couleur = 15;

    if(!fichier)
        return false;

    rewind(fichier);

    Voter tmp_voter;
    printf("  ---------------------------------------------------------------------------------------------------------------------\n");
    wprintf(L"  |  Voter N%lc  |     CNI Number    |               Name              |               Last name              | Sex\n",167);
    printf("  ---------------------------------------------------------------------------------------------------------------------\n");

    while(fread(&tmp_voter, sizeof(Voter), 1, fichier))
    {
        if(tmp_voter.lane == 0 || tmp_voter.lane == -1) // si l"voter a déja voter
            couleur = 8;
        else  // sinon
            couleur = 15;

        color(couleur, 0);
        printf("\t %-6d|", tmp_voter.number);
        printf("     %-14s|         %-24s|         %-26s   |  %c\n",tmp_voter.CNI_number,tmp_voter.name,tmp_voter.last_name,toupper(tmp_voter.sex));
        printf("  ----------------------------------------------------------------------------------------------------------------------\n");
        color(15, 0);
    }
    return true;
}

int editVoter(void)
{
    //Prerequis:  fclose(fichier);l'identifiant du voter et le fichier contenant les voters ouvert en mode lecture
    //Action: modifie les informations relatifs à un voter
    //Idee:
    remove("tmpFileVoter.txt");

    FILE* fichier = fopen("voter.txt", "r");
    FILE* fichier1 = fopen("tmpFileVoter.txt", "a+");
    int number = 0;

    if(!fichier1)
    {
        fclose(fichier1);
        fclose(fichier);
        remove("tmpFileVoter.txt");
        return false;
    }

    if(!viewRegisteredVoters(fichier))
    {
        fclose(fichier);
        fclose(fichier1);
        remove("tmpFileVoter.txt");
        return false;
    }

    wprintf(L"\t\t Enter your number: ");
    number = getNumber();

    rewind(fichier);
    rewind(fichier1);

    Voter tmp_voter;
    int i = 0;
    char continuer ='\0';

    // on copie les info qui vienne avant celles de notre candidat les met dans un autre fichier
    for(i = 1 ; i<number ; i++)
    {
        fread(&tmp_voter, sizeof(Voter), 1, fichier);
        fwrite(&tmp_voter, sizeof(Voter), 1, fichier1);
    }

    // on recupère les infos de notre candidat
    fread(&tmp_voter, sizeof(Voter), 1, fichier);

    // on procède a la modefication
    if(tmp_voter.lane == 0)
    {
        color(10 , 0);
        printf("\n\t\t\a You have already voted. You can no longer modifier your information!!!! \n");
        return true;
    }

    color(9, 0);
    printf("\n\t\t You are trying to change the informations of  %s %s.\n", tmp_voter.last_name, tmp_voter.name);
    printf("\t\t Are you sure you want to continue? y/n ? \n\n");
    color(15, 0);

    do
    {
        continuer = getch();
    }
    while(!strchr("yn", continuer));

    if(continuer == 'y')
    {
        printf("\t Current informations:\n",tmp_voter.last_name, tmp_voter.name);
        printf("\t\t------------------------------\n");
        wprintf(L"\t\t Voter N%lc:%d\n", 167, tmp_voter.number);
        printf("\t\t -->Name:%s\n", tmp_voter.name);
        printf("\t\t -->Last name:%s\n", tmp_voter.last_name);
        printf("\t\t -->Sex:%c\n", tmp_voter.sex);
        printf("\t\t -->CNI number:%s \n\n", tmp_voter.CNI_number);
        printf("\t\t------------------------------\n");
        printf("\t Enter new informations:\n");

        rewind(fichier1);
        addVoter(fichier1, fichier, true);   // on active l'option de modification de lélecteur

        // on remet le cursue virtuel au bon endroit
        rewind(fichier);
        int tmp_numero = number;
        while(fread(&tmp_voter, sizeof(Voter), 1, fichier) && number > 1)
            tmp_numero--;

        // on continue la lecture
        while(fread(&tmp_voter, sizeof(Voter), 1, fichier))
            fwrite(&tmp_voter, sizeof(Voter), 1,fichier1);

        fclose(fichier);
        fclose(fichier1);
        remove("voter.txt");
        rename("tmpFileVoter.txt","voter.txt");
    }

    else
    {
        fclose(fichier);
        fclose(fichier1);
        remove("tmpFileVoter");
    }
    return true;
}

int Supprimervoter(void)
{
    //Prerequis: l'identifiant du voter et le fichier contenant les voters ouvert en mode lecture
    //Action: modifie les informations relatifs à un voter
    //Idee: recopier le contenu d'un fichier dans un autre fichier sauf un élément notre voter et ensuite renameer le fichier

    remove("tmpFileVoter.txt");

    FILE* fichier = fopen("voter.txt", "r");
    FILE* fichier1 = fopen("tmpFileVoter.txt", "a+");
    int number = 0;

    if(!fichier)
    {
        fclose(fichier1);
        fclose(fichier);
        remove("tmpFileVoter.txt");
        return false;
    }

    if(!viewRegisteredVoters(fichier))
    {
        fclose(fichier);
        fclose(fichier1);
        remove("tmpFileVoter.txt");
        return false;
    }

    printf("\t\t Voter number: ");
    number = getNumber();

    rewind(fichier);
    rewind(fichier1);

    Voter tmp_voter;
    int i = 0;
    char continuer ='\0';

    // on copie les info qui vienne avant celles de notre candidat les met dans un autre fichier
    for(i = 1 ; i<number ; i++)
    {
        fread(&tmp_voter, sizeof(Voter), 1, fichier);
        fwrite(&tmp_voter, sizeof(Voter), 1, fichier1);
    }

    // on recupère les infos de notre candidat
    fread(&tmp_voter, sizeof(Voter), 1, fichier);

    // on procède a la modefication
    color(9, 0);
    printf("\n\t\t Voter deleted: %s %s \n",tmp_voter.last_name, tmp_voter.name);
    wprintf(L"\t\t Are you sure you want to continue? y/n ? \n\n",136,136, 150);
    color(15, 0);

    do
    {
        continuer = getch();
    }
    while(!strchr("yn", continuer));


    if(continuer == 'y' || continuer == 'Y' )
    {
        // on continue la lecture
        while(fread(&tmp_voter, sizeof(Voter), 1, fichier))
        {
            // on déincrement les numéro de tous les voters qui sont devant lui
            tmp_voter.number = tmp_voter.number - 1;
            //
            fwrite(&tmp_voter, sizeof(Voter), 1,fichier1);
        }

        fclose(fichier);
        fclose(fichier1);
        remove("voter.txt");
        rename("tmpFileVoter.txt","voter.txt");
    }

    else
    {
        fclose(fichier);
        fclose(fichier1);
        remove("tmpFileVoter.txt");
    }

    return true;
}
