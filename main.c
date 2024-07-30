#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>// pour gerer les accents

#include "helper.h"
#include "election.h"

int main(int argc, char* argv[])
{
    //Prerequis: Aucun
    //Action: gere tous l'application
    //Idée:
    char choix = '\0';
    FILE* fichier = NULL;
    int p = false;

P:
    color(11 , 0);
    printf("\t\t\t            ************************************************\n");
    printf("\t\t\t            *               MINI ELECTORAL SYSTEM          *\n");
    printf("\t\t\t            ************************************************\n\n");
    color(15 , 0);
    printf("\t\t               System Menu:\n");
        color(14,0);
    printf("\t\t                     1)-CANDIDATE MANAGEMENT\n");
    printf("\t\t                     2)-VOTER MANAGEMENT\n");
    printf("\t\t                     3)-VOTE\n");
    printf("\t\t                     4)-VIEW STATS\n");
    printf("\t\t                     5)-RESET SYSTEM\n");
    printf("\t\t                     6)-QUIT\n");
    color(15,0);

    printf("\t\t What is your choice: ");
    char c = '\0';
    int i = 0;
    choix = getCaracter("123456", "Please make your choice again");


    switch(choix)
    {
    case'1':
        system("cls"); // We clear the screen

        candidateManagement();

        system("cls");// We clear the screen again
        fflush(stdin);// we empty of tempon
        goto P;
        break;

    case '2':
        system("cls");

        voterManagement();

        system("cls");
        fflush(stdin);
        goto P;
        break;

    case '3':
        system("cls");

        p = Vote();

        fichier = fopen("constantes.txt" , "w");
        fprintf(fichier , "%d", p);
        fclose(fichier);

        system("pause");
        system("cls");
        fflush(stdin);
        goto P;
        break;

    case '4':
        system("cls");

        viewStats();

        system("pause");
        system("cls");
        fflush(stdin);
        goto P;
        break;

    case '5':
        system("cls");

        resetSystem();

        system("pause");
        system("cls");
        goto P;
        break;
    case '6':
        break;
    }
    return 0;
}
