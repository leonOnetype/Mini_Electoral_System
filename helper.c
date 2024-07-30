#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

# include "helper.h"

void color(int t,int f)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,f*16+t);
}

void treatTexte(char* texte)
{
    int length = strlen(texte);
    int i = 0;

    texte[length-1] = '\0';

    // on cherche s'il ya des espaces a la fin de la chaine
    for(i = length-1  ; i>0 ; i--)
    {
        if(texte[i] == ' ')
        {
            if(texte[i-1] == ' ')
            {}
            else
            {
                texte[i] = '\0';
                i=0;
            }

        }
    }
}

int getNumber()
{
    int length = 0, i=0 ;
    char chaine[10] = {'\0'};
    char c = '\0';

    for(i = 0 ; i<10 ; i++)
    {

        fflush(stdin);
        c = getch();
        if(c == 8)
        {
            if(i>0)
            {
                chaine[i-1] = '\0';
                i -=2;
                printf("\b \b");
            }
            else
                i--;
        }
        else if(c == 13)
        {
            // on quite la boucle
            i = 10;
        }
        else if(!isdigit(c))
        {
        }
        else
        {
            chaine[i] = c;
            printf("%c", c);
        }

    }
    return strtol(chaine, NULL, 0);
}

char getCaracter(char* values, char* texte)
{
    char c = '\0', to_back='\0';
    int i = 0;

    for(i=0 ; i<1 ; i++)
    {
        fflush(stdin);
        c = getch();
        if(c == 8) // <-- S'il appuie sue la touche backspace
        {
            if(to_back  == '\0')
            {}
            else
            {
                printf("\b \b");
                to_back = '\0';
            }
            i-=2;
        }
        else if(c == 13) // <-- S'il appuie sur la touche entrer
        {
            if (to_back  == '\0')
            {
                printf("\n\t\t %s: ", texte);
                i-=2;
            }
            else
            {
                printf("\n");
                i=1;
            }

        }
        else // <-- S'il appuie sur une autre touche
        {
            if(!strchr(values, c) || to_back != '\0') {}
            else
            {
                to_back = c;
                color(7, 0);
                printf("%c", to_back);
                color(15, 0);
            }
            i-=2;
        }
    }

    return to_back;
}
