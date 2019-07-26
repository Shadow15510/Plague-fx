/*
 Nom : Plague
 Version : - dev-
 Dernière modification : 26 Juillet 2019
 */

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/std/stdio.h>

//display : affiche l'écran de la calculatrice avec les données en rapport avec l'écran
void display(const image_t *img_fonds, const int *fond, const int *recherche, const int *adn);

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
    
    int fond = 1, fin = 0, key = 0;//variables diverses
    int recherche = 25, adn = 888;//variables pour la maladie
    
	dclear(C_WHITE);
    dimage(0, 0, &img_titre);
    dupdate();
    getkey();
    //Fin de l'écran d'accueil
    
    while (fin == 0)
    {
        display(&img_fonds, &fond, &recherche, &adn);
        key = getkey().key;
        switch (key)
        {
            case KEY_LEFT:
                if(fond > 1) fond -= 1;
                break;
            case KEY_RIGHT:
                if (fond < 5) fond += 1;
                break;
            case KEY_EXIT:
                fin = 1;
                break;
        }
        
    }
	return 0;
}


void display(const image_t *img_fonds, const int *fond, const int *recherche, const int *adn)
{
    int variable;
    char string[100];
    dclear(C_WHITE);
    dsubimage(0, 0, &(*img_fonds), 0, 64 * (*fond - 1) + (*fond - 1), 128, 64, 0);
    
    switch (*fond) // affichage supplémentaires dépendant des fonds.
    {
        case 2:// monde avec la barre en dessous
            //recherche (max = 74 pxl) en % => 74 * (recherche / 100)
            variable = 74 * *recherche/100;
            sprintf(string, "%d", *adn);
            dtext(9, 58, string, C_BLACK, C_NONE);
            dline(51, 60, 51 + variable, 60, C_BLACK);
            dline(51, 59, 51 + variable, 59, C_BLACK);
            break;
        case 3://Menu de modification de la maladie
            break;
        case 4://Menu info
            dtext(46, 25, "TOUX", C_BLACK, C_NONE);
            dtext(73, 33, "0123", C_BLACK, C_NONE);
            break;
            
    }
    dupdate();
}

double floor(double x)
{
    return (int)x;
}
