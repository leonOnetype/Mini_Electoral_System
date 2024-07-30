#ifndef __VOTER__
#define __VOTER__

// structure d'un electeur
  typedef struct Voter
 {
     int number;
     char name[TAILLE_MAX];
     char last_name[TAILLE_MAX];
     char CNI_number[10];
     char sex;
     int lane;
 }Voter;


// fonction pour la partie electeur
void voterManagement(void);
int viewRegisteredVoters(FILE* fichier);
void addVoter(FILE* enregFile , FILE* sourceFile , int option);
int editVoter(void);
int deleteVoter(void);
int numberOfRegisteredVoter(FILE* fichier);
char voterIsDouble(FILE* fichier,Voter voter , int option_editing);
#endif // __VOTER__
