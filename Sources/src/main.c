/*
 Nom : Plague
 Version : - dev-
 Dernière modification : 31 Juillet 2019
 
 Liste des choses à faire :
 */

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/std/stdio.h>
#include <gint/std/string.h>


//display_barre : affiche les infos de la barre en dessous du monde
void display_barre (const int *adn, const int *recherche, const int *limite);

//display_menu : affiche les données sur l'écran menu du jeu à partir duquel on peut modifier la maladie
void display_menu (const int *adn, const int *contagion, const int *severite, const int *letalite);

//display_info : affiche les infos sur la maladie selectionnée
void display_info (const char *nom, const int adn, const int conta, const int leta, const int sev);

//menu : gère les tableaux des mutations en fonction de la variable 'variable'
int menu (int variable, const image_t img_fonds, int nv_symp, int nv_capa, int nv_trans);

//floor : renvoie la partie entière d'une variable
double floor (double x);

//copy : copie le contenu de src en écrasant dest. Uniquement pour des chaînes de caractères !!
char *copy (char *dest, const char *src);


int main (void)
{
    extern const font_t font_plague;//déclaration de la police custom
    extern const image_t img_titre;//déclaration de l'image-titre
    extern const image_t img_fonds;//déclaration des différents fonds du jeu
    //extern const image_t img_avions;//déclaration des avions
    
    dfont(&font_plague);//On change la police pour la police custom
    
    int fond = 1, fin = 0, key = 0, menu_muta = 0;//variables diverses pour le jeu
    int recherche = 0, limite = 100, adn = 0, contagion = 0, severite = 0, letalite = 0, nv_symp = 1, nv_capa = 1, nv_trans = 1;//variables pour la maladie
    
	dclear(C_WHITE);
    dimage(0, 0, &img_titre);
    dupdate();
    getkey();
    //Fin de l'écran d'accueil
    
    while (fin == 0)
    {
        dclear(C_WHITE);
        dsubimage(0, 0, &img_fonds, 0, 64 * (fond - 1) + (fond - 1), 128, 64, 0);
        
        switch (fond)// affichage supplémentaires dépendant des fonds.
        {
            case 2:
                display_barre(&adn, &recherche, &limite);// monde avec la barre en dessous
                break;
            case 3:
                display_menu(&adn, &contagion, &severite, &letalite);//Menu de modification de la maladie
                break;
        }
        
        dupdate();
        key = getkey().key;
        
        switch (key)
        {
            case KEY_OPTN:
                if (fond == 1) fond = 2;
                else if (fond == 2) fond = 1;
                break;
            case KEY_VARS:
                fond = 3;
                break;
            case KEY_F1:
                if (fond == 3) menu_muta = 1;
                break;
            case KEY_F3:
                if (fond == 3) menu_muta = 2;
                break;
            case KEY_F5:
                if (fond == 3) menu_muta = 3;
                break;
            case KEY_F6:
                if (fond == 3) fond = 1;
                break;
            case KEY_EXIT:
                if (fond != 1) fond = 1;
                else fin = 1;
                break;
        }
        if (menu_muta != 0) menu_muta = menu(menu_muta, img_fonds, nv_symp, nv_capa, nv_trans);
    }
	return 0;
}


void display_barre (const int *adn, const int *recherche, const int *limite)
{
    //recherche (jauge = 74 pxl) donc : 74 * (recherche / 100) pour le pourcentage
    int variable;
    char string[100];
    variable = 74 * *recherche / *limite;
    sprintf(string, "%d", *adn);
    dtext(9, 58, string, C_BLACK, C_NONE);
    dline(51, 60, 51 + variable, 60, C_BLACK);
    dline(51, 59, 51 + variable, 59, C_BLACK);
}


void display_menu (const int *adn, const int *contagion, const int *severite, const int *letalite)
{
    // toutes les jauges font 68 pxl de long.
    int variable;
    char string[100];
    sprintf(string, "%d", *adn);
    dtext(102, 37, string, C_BLACK, C_NONE);
    
    variable = 68 * *contagion / 25;
    dline(57, 48, 57 + variable, 48, C_BLACK);
    dline(57, 49, 57 + variable, 49, C_BLACK);
    
    variable = 68 * *severite / 20;
    dline(57, 54, 57 + variable, 54, C_BLACK);
    dline(57, 55, 57 + variable, 55, C_BLACK);
    
    variable = 68 * *letalite / 25;
    dline(57, 60, 57 + variable, 60, C_BLACK);
    dline(57, 61, 57 + variable, 61, C_BLACK);
}


void display_info (const char *nom, const int adn, const int conta, const int leta, const int sev)
{
    char string[100];

    dtext(47, 25, nom, C_BLACK, C_NONE);
    sprintf(string, "%d", adn);
    dtext(73, 33, string, C_BLACK, C_NONE);
    
    sprintf(string, "%d", conta);
    dtext(81, 41, string, C_BLACK, C_NONE);
    
    sprintf(string, "%d", leta);
    dtext(74, 49, string, C_BLACK, C_NONE);
    
    sprintf(string, "%d", sev);
    dtext(75, 57, string, C_BLACK, C_NONE);
}


int menu (int variable, const image_t img_fonds, int nv_symp, int nv_capa, int nv_trans)
{
    extern image_t img_muta;
    extern image_t img_pieces;
    int x = 1, y = 1, i, j, fin = 0, key = 0, no;
    int tableau[4][8] = {{1, 2, 5, 4, 3, 0, 0, 0},{0, 0, 14, 13, 0, 0, 0, 0},{0, 0, 0, 0, 12, 0, 0, 0},{0, 0, 6, 8, 7, 11, 10, 9}};
    
    while (fin == 0)
    {
        dclear(C_WHITE);
        dsubimage(0, 0, &img_fonds, 0, 260, 128, 64, 0);
        
        for (i = 0 ; i <= 7 ; i++)
        {
            for (j = 0 ; j <= 3; j++)
            {
                if (tableau[j][i] != 0) dsubimage(15 * i + i, 15 * j + j, &img_muta, 15 * (variable - 1) + (variable - 1), 15 * (tableau[j][i] - 1) + (tableau[j][i] - 1), 15, 15, 0);
            }
        }
        dupdate();
        key = getkey().key;
        switch (key)
        {
            case KEY_EXIT:
                fin = 1;
                break;
        }
    }
    return 0;
}

double floor (double x)
{
    return (int)x;
}


char *copy (char *dest, const char *src)
{
    unsigned int i;
    for (i =0 ; i <= strlen(src) ; i++) dest[i] = src[i];
    dest[strlen(src) + 1] = '\0';
    return dest;
}
