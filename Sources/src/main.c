/*
 Nom : Plague
 Version : - dev-
 Dernière modification : 27 Juillet 2019
 
 Liste des choses à faire :
 */

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/std/stdio.h>


//display_barre : affiche les infos de la barre en dessous du monde
void display_barre(const int *adn, const int *recherche);

//display_menu : affiche les données sur l'écran menu du jeu à partir duquel on peut modifier la maladie
void display_menu(const int *adn, const int *contagion, const int *severite, const int *letalite);

//display_info : affiche les infos sur la maladie selectionnée
void display_info(void);

//floor : renvoie la partie entière d'une variable
double floor(double x);


int main(void)
{
    extern const font_t font_plague;//déclaration de la police custom
    extern const image_t img_titre;//déclaration de l'image-titre
    
    extern const image_t img_fonds;//déclaration des différents fonds du jeu
   /*
    extern const image_t img_avions;//déclaration des avions
    extern const image_t img_pieces;//déclaration des pièces pour les tableaux (selectionnées, cachées, …)
    extern const image_t img_capa;//déclaration des capacités
    extern const image_t img_symp;//déclaration des symptômes
    extern const image_t img_trans;//déclaration des transmissions
    */
    
    dfont(&font_plague);//On change la police pour la police custom
    
    int fond = 1, fin = 0, key = 0;//variables diverses pour le jeu
    int recherche = 25, adn = 0, contagion = 0, severite = 0, letalite = 0;//variables pour la maladie
    
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
                display_barre(&adn, &recherche);// monde avec la barre en dessous
                break;
            case 3:
                display_menu(&adn, &contagion, &severite, &letalite);//Menu de modification de la maladie
                break;
            case 4:
                display_info();//Menu info
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
            case KEY_EXIT:
                if (fond != 1) fond = 1;
                else fin = 1;
                break;
        }
        
    }
	return 0;
}


void display_barre(const int *adn, const int *recherche)
{
    //recherche (jauge = 74 pxl) donc : 74 * (recherche / 100) pour le pourcentage
    int variable;
    char string[100];
    variable = 74 * *recherche / 100;
    sprintf(string, "%d", *adn);
    dtext(9, 58, string, C_BLACK, C_NONE);
    dline(51, 60, 51 + variable, 60, C_BLACK);
    dline(51, 59, 51 + variable, 59, C_BLACK);
}


void display_menu(const int *adn, const int *contagion, const int *severite, const int *letalite)
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


void display_info(void)
{
    dtext(46, 25, "TOUX", C_BLACK, C_NONE);
    dtext(73, 33, "0123", C_BLACK, C_NONE);
}


double floor(double x)
{
    return (int)x;
}
