#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wchar.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "election.h"
#include "helper.h"
#include "voter.h"
#include "candidate.h"

int Vote()
{
    Voter tmp_voter;
    Candidate tmp_candidate;

    // question on efface d'abord s'il y'avait quelques fichier
    remove("tmpFileCandidate.txt");
    remove("tmpFileVoter.txt");

    FILE* efichier = fopen("voter.txt", "r");
    FILE* cfichier = fopen("candidate.txt","r");
    FILE* fichier1 = fopen("tmpFileVoter.txt", "a+");
    FILE* fichier2 = fopen("tmpFileCandidate.txt", "a+");
    int i =0, id_convert = 0,login_number =0, t = 0;
    char id[10];

    color(14, 0);
    printf("\n\t\t----------------------------- The vote !!!!!! ------------------------------\n\n");
    color(15, 0);
    printf("\t\t Enter your login number: ");
    login_number = getNumber();

    if(login_number > numberOfRegisteredCandidate(efichier))
    {
        printf("\a\t No voter is registered with this number!\n");
        fclose(fichier1);
        fclose(fichier2);
        fclose(efichier);
        fclose(cfichier);
        remove(fichier1);
        remove(fichier2);
        return false;
    }

    // si on arrive la c'est que c'est bon
    // on recupere le nom de l'Voter
    rewind(efichier);
    rewind(fichier1);

    for(i = 1 ; i<login_number ; i++)
    {
        fread(&tmp_voter, sizeof(Voter), 1, efichier);
        fwrite(&tmp_voter, sizeof(Voter), 1, fichier1);
    }
    fread(&tmp_voter, sizeof(Voter), 1, efichier);

    printf("\n\t\t Voter:%s %s\n\n", tmp_voter.last_name, tmp_voter.name);

    if(tmp_voter.lane == 0 || tmp_voter.lane == -1)
    {
        color(7, 0);
        printf(L"\t\t\a You have already assigned your lane to a candidatee. Sorry you can't vote anymore! \n");
        color(15, 0);

        fclose(fichier1);
        fclose(fichier2);
        fclose(efichier);
        fclose(cfichier);
        remove(fichier1);
        remove(fichier2);
        return true;
    }
    else
    {
        printf("\t--------------------------------- Registered candidates --------------------------------------\n\n");

        if(!viewRegisteredCandidates(cfichier))
        {
            fclose(fichier1);
            fclose(fichier2);
            fclose(efichier);
            fclose(cfichier);
            remove("tmpFileVoter.txt");
            remove("tmpFileCandidate.txt");
            return false;
        }

        // on recupere et traite le choix de l'Voter
        color(6 , 0);
        printf("\n\t  Candidatee of your choice(just his number): \n\t You can vote null ballot(Press n) or report your vote(Press a): ");
        scanf("%s", id);
        color(15 , 0);
        id_convert = strtol(id,NULL, 0);
        while(id_convert > numberOfRegisteredCandidate(cfichier))
        {
            printf("\n\t Wrong entry. Please make your choice again: ");
            scanf("%s", &id);
            id_convert = strtol(id,NULL, 0);
        }

        // on teste s'il veut amorcer son vote ou veut voter le buletin Nul
        if(strchr(id,'a') || strchr(id,'A')) // amorer le vote
        {
            fclose(fichier1);
            fclose(fichier2);
            fclose(efichier);
            fclose(cfichier);
            remove("tmpFileVoter.txt");
            remove("tmpFileCandidate.txt");
            return true;
        }
        else
        {

            /// Modification du nombre de la voie de l'électeur dans le fichier
            // ici l'Voter a deja voté donc on met sa voie a 0 et on modifie ces information dans le fichier
            if(strchr(id,'n') || strchr(id,'N'))
            {
                tmp_voter.lane = -1; // le bulletin nulle
            }
            else
                tmp_voter.lane = 0;

            fwrite(&tmp_voter, sizeof(Voter), 1,fichier1);

            while(fread(&tmp_voter, sizeof(Voter), 1, efichier))
                fwrite(&tmp_voter, sizeof(Voter), 1,fichier1);

            fclose(efichier);
            fclose(fichier1);
            remove("voter.txt");
            rename("tmpFileVoter.txt","voter.txt");

            ///Modification du nombre de voie du candidate dans le fichier
            rewind(cfichier);
            rewind(fichier2);
            for(i = 1 ; i<id_convert ; i++)
            {
                fread(&tmp_candidate, sizeof(Candidate), 1, cfichier);
                fwrite(&tmp_candidate, sizeof(Candidate), 1, fichier2);
            }
            // on recupère les infos de notre candidate
            fread(&tmp_candidate, sizeof(Candidate), 1, cfichier);

            color(7, 0);
            if(strchr(id,'n') || strchr(id,'N'))
            {
                printf("\t\t\n\a You voted for the null ballot \n\n");
                // on ne fait donc pa de modification du nombres de voie du candidate
            }
            else
            {
                // on modifie son nombre de voie
                printf("\t\t\n\a You have voted  %s %s \n\n",tmp_candidate.last_name, tmp_candidate.name);
                tmp_candidate.nb_of_lane += 1;
            }
            color(15, 0);

            // on l'enregistre
            fwrite(&tmp_candidate, sizeof(Candidate), 1,fichier2);

            while(fread(&tmp_candidate, sizeof(Candidate), 1, cfichier))
                fwrite(&tmp_candidate, sizeof(Candidate), 1,fichier2);

            fclose(cfichier);
            fclose(fichier2);
            remove("candidate.txt");
            rename("tmpFileCandidate.txt","candidate.txt");
        }
    }

    return true;
}

///##################*********** FONCION GERANT L'AFFICHAGE DES STATISTIQUES DU VOTE *************#####################

void viewStats()
{
    FILE* fichier1 = fopen("candidate.txt", "r");
    FILE* fichier2 = fopen("voter.txt", "r");

    Voter tmp_voter;
    Candidate tmp_candidate;
    int counter = 0;
    float percentage = 0.0;


    color(14, 0);
    printf("\n\t\t ------------------------ General stats of the vote ---------------------------\n");
    color(15, 0);
    rewind(fichier1);
    rewind(fichier2);

    // on determine le nombre d'Voter ayan voté
    while(fread(&tmp_voter, sizeof(Voter), 1, fichier2))
    {
        if(tmp_voter.lane == 0)
            counter++;
    }

    printf("\t\t Number of people who voted: %d \n\n",counter);
    printf("\t\t----------------------------- Result of the vote -----------------------------------\n");

    // on determine le pourcentage de chaque candidate
    while(fread(&tmp_candidate, sizeof(Candidate), 1, fichier1))
    {
        if(counter == 0 || tmp_candidate.nb_of_lane == 0 )
            percentage = 0.0;
        else
        {
            percentage = ( (tmp_candidate.nb_of_lane)/(float)counter)*100;
        }

        printf("\t\t -Candidatee: %s %-15s  -Number of lane: %d   -Percentage: %3.2f\x25\n\n", tmp_candidate.last_name, tmp_candidate.name, tmp_candidate.nb_of_lane, percentage);
    }
    fclose(fichier1);
    fclose(fichier2);

}

// fonction qui permet de reinitialiser le programme
void resetSystem(void)
{
    remove("candidate.txt");
    remove("voter.txt");
    remove("constantes.txt");

}
///##################*********** EXTRA  *****************##############
