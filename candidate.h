#ifndef __CANDIDATE__
# define __CANDIDATE__

// structure d'un candidat
 typedef struct Candidate
 {
     int login_number;
     char name[TAILLE_MAX];
     char last_name[TAILLE_MAX];
     char CNI_number[10];
     char sex;
     int nb_of_lane;
 }Candidate;

 // fonction pour la partie candidat
void candidateManagement(void);
int viewRegisteredCandidates(FILE* fichier);
void addCandidate(FILE* enregFile , FILE* sourceFile , int option , int nb_of_lane);
int editCandidate(void );
int numberOfRegisteredCandidate(FILE* fichier);
char candidateIsDouble(FILE* fichier,Candidate candidate , int option_modification);

#endif // __CANDIDAT__
