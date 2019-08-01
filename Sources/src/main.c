/*
 Nom : Plague
 Version : - dev-
 Dernière modification : 1 Août 2019
 
 Liste des choses à faire :
  - Gestion des sélection dans les mutations et menu info
  - Gestion des avions
  - Moteurs du jeu (gestion du temps et sauvegarde)
  - Gestion des pays
 */

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/std/stdio.h>
#include <gint/std/string.h>


//display_barre : affiche les infos de la barre en dessous du monde
void display_barre (const int *adn, const int *recherche, const int *limite);

//display_menu : affiche les données sur l'écran menu du jeu à partir duquel on peut modifier la maladie
void display_menu (const int *adn, const int *contagion, const int *severite, const int *letalite, const int *sel_symp, const int *sel_capa, const int *sel_trans);

//display_info : affiche les infos sur la maladie selectionnée
void display_info (const char *nom, const int adn, const int conta, const int leta, const int sev);

//display_stats : affiche les statistiques des humains
void display_stats(const double *sains, const double *infectes, const double *morts, const double *gueris, const double *total);

//menu : gère les tableaux des mutations en fonction de la variable 'variable'
int menu (int variable, const image_t *img_fonds, int nv_symp, int nv_capa, int nv_trans);

//modele_infectieux : calcule la propagation de la maladie
void modele_infectieux (double *sains, double *infectes, double *morts, double *gueris, double *s_avant, double *i_avant, double *m_avant, double *g_avant, int *contagion, int *severite, int *letalite, int *changement);

//floor : renvoie la partie entière d'une variable
double floor (double x);

//init_mat : remplis la matrice dest à partir de src
void init_mat(int x, int y, int dest[][x], int src[][x]);


int main (void)
{
    extern const font_t font_plague;//déclaration de la police custom
    extern const image_t img_titre;//déclaration de l'image-titre
    extern const image_t img_fonds;//déclaration des différents fonds du jeu
    //extern const image_t img_avions;//déclaration des avions
    
    dfont(&font_plague);//On change la police pour la police custom
    
    int fond = 1, fin = 0, key = 0, menu_muta = 0;//variables diverses pour le jeu
    int recherche = 0, limite = 100, adn = 0, contagion = 0, severite = 0, letalite = 0, nv_symp = 1, nv_capa = 1, nv_trans = 1, sel_symp = 1, sel_capa = 1, sel_trans = 1;//variables pour la maladie
    double total = 10000000000, sains = total, infectes = 0, morts = 0, gueris = 0, s_avant, i_avant, m_avant, g_avant;//variables pour les statistiques des humains
    double changement = 0, priorite = 0;//variables pour le modèle infectieux
    
	dclear(C_WHITE);
    dimage(0, 0, &img_titre);
    dupdate();
    getkey();
    //Fin de l'écran d'accueil
    
    while (fin == 0)
    {
        dclear(C_WHITE);
        dsubimage(0, 0, &img_fonds, 0, 65 * (fond - 1), 128, 64, 0);
        
        switch (fond)// affichage supplémentaires dépendant des fonds.
        {
            case 2:
                display_barre(&adn, &recherche, &limite);// monde avec la barre en dessous
                break;
            case 3:
                display_menu(&adn, &contagion, &severite, &letalite, &sel_symp, &sel_capa, &sel_trans);//Menu de modification de la maladie
                break;
            case 6:
                display_stats(&sains, &infectes, &morts, &gueris, &total);
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
            case KEY_SQUARE:
                fond = 6;
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
        if (menu_muta != 0) menu_muta = menu(menu_muta, &img_fonds, nv_symp, nv_capa, nv_trans);
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


void display_menu (const int *adn, const int *contagion, const int *severite, const int *letalite, const int *sel_symp, const int *sel_capa, const int *sel_trans)
{
    // toutes les jauges font 67 pxl de long.
    extern image_t img_muta;
    int variable;
    char string[100];
    sprintf(string, "%d", *adn);
    dtext(102, 37, string, C_BLACK, C_NONE);
    
    variable = 67 * *contagion / 26;
    dline(57, 48, 57 + variable, 48, C_BLACK);
    
    variable = 67 * *severite / 20;
    dline(57, 54, 57 + variable, 54, C_BLACK);
    
    variable = 67 * *letalite / 33;
    dline(57, 60, 57 + variable, 60, C_BLACK);

    dsubimage(5, 15, &img_muta, 0, 16 * (*sel_symp - 1), 15, 15, 0);
    dsubimage(35, 15, &img_muta, 16, 16 * (*sel_capa - 1), 15, 15, 0);
    dsubimage(65, 15, &img_muta, 32, 16 * (*sel_trans - 1), 15, 15, 0);
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


void display_stats (const double *sains, const double *infectes, const double *morts, const double *gueris, const double *total)
{
    //toutes les jauges font 63 pxl
    int variable;
    
    variable = 62 * *sains / *total;
    dline(61, 31, 61 + variable, 31, C_BLACK);
    
    variable = 63 * *infectes / *total;
    dline(61, 39, 61 + variable, 39, C_BLACK);
    
    variable = 63 * *morts / *total;
    dline(61, 47, 61 + variable, 47, C_BLACK);
    
    variable = 63 * *gueris / *total;
    dline(61, 55, 61 + variable, 55, C_BLACK);
    
}


int menu (int menu_muta, const image_t *img_fonds, int nv_symp, int nv_capa, int nv_trans)
{
    extern image_t img_muta;
    extern image_t img_pieces;
    int x = 0, y = 0, i, j, fin = 0, key = 0, lim = 0;
    int tableau[4][8];
    
    int symp_1[4][8] = {{1, 15, 15, 15, 3, 0, 0, 0},{0, 0, 15, 15, 0, 0, 0, 0},{0, 0, 15, 0, 15, 0, 0, 0},{0, 15, 15, 6, 15, 15, 0, 0}};
    int symp_2[4][8] = {{1, 2, 15, 4, 3, 0, 0, 0},{0, 0, 15, 15, 0, 0, 0, 0},{0, 0, 15, 0, 12, 0, 0, 0},{0, 15, 7, 6, 9, 15, 0, 0}};
    int symp_3[4][8] = {{1, 2, 5, 4, 3, 0, 0, 0},{0, 0, 15, 15, 0, 0, 0, 0},{0, 0, 15, 0, 12, 0, 0, 0},{0, 10, 7, 6, 9, 11, 0, 0}};
    int symp_4[4][8] = {{1, 2, 5, 4, 3, 0, 0, 0},{0, 0, 14, 13, 0, 0, 0, 0},{0, 0, 8, 0, 12, 0, 0, 0},{0, 10, 7, 6, 9, 11, 0, 0}};
    
    int capa_1[4][8] = {{1, 0, 0, 0, 6, 0, 0, 0},{7, 7, 0, 7, 0, 0, 0, 0},{0, 0, 0, 0, 7, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0}};
    int capa_2[4][8] = {{1, 0, 0, 0, 6, 0, 0, 0},{2, 7, 0, 3, 0, 0, 0, 0},{0, 0, 0, 0, 7, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0}};
    int capa_3[4][8] = {{1, 0, 0, 0, 6, 0, 0, 0},{2, 5, 0, 3, 0, 0, 0, 0},{0, 0, 0, 0, 4, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0}};
    
    int trans_1[4][8] = {{1, 14, 14, 0, 14, 5, 0, 0},{0, 0, 0, 14, 0, 0, 0, 11},{9, 14, 0, 0, 0, 0, 0, 14},{0, 0, 14, 14, 0, 0, 0, 14}};
    int trans_2[4][8] = {{1, 2, 14, 0, 6, 5, 0, 0},{0, 0, 0, 14, 0, 0, 0, 11},{9, 10, 0, 0, 0, 0, 0, 12},{0, 0, 0, 14, 14, 0, 0, 14}};
    int trans_3[4][8] = {{1, 2, 3, 0, 6, 5, 0, 0},{0, 0, 0, 14, 0, 0, 0, 11},{9, 10, 0, 0, 0, 0, 0, 12},{0, 0, 0, 7, 14, 0, 0, 13}};
    int trans_4[4][8] = {{1, 2, 3, 0, 6, 5, 0, 0},{0, 0, 0, 4, 0, 0, 0, 11},{9, 10, 0, 0, 0, 0, 0, 12},{0, 0, 0, 7, 8, 0, 0, 13}};
    
    switch (menu_muta)//Remplissage de la matrice pour afficher les mutations
    {
        case 1:
            lim = 15;
            switch (nv_symp)
        {
            case 1:
                init_mat(8, 4, tableau, symp_1);
                break;
            case 2:
                init_mat(8, 4, tableau, symp_2);
                break;
            case 3:
                init_mat(8, 4, tableau, symp_3);
                break;
            case 4:
                init_mat(8, 4, tableau, symp_4);
                break;
        }
            break;
        case 2:
            lim = 7;
            switch (nv_capa)
        {
            case 1:
                init_mat(8, 4, tableau, capa_1);
                break;
            case 2:
                init_mat(8, 4, tableau, capa_2);
                break;
            case 3:
                init_mat(8, 4, tableau, capa_3);
                break;
        }
            break;
        case 3:
            lim = 14;
            switch (nv_trans)
        {
            case 1:
                init_mat(8, 4, tableau, trans_1);
                break;
            case 2:
                init_mat(8, 4, tableau, trans_2);
                break;
            case 3:
                init_mat(8, 4, tableau, trans_3);
                break;
            case 4:
                init_mat(8, 4, tableau, trans_4);
                break;
        }
            break;
    }
    
    while (fin == 0)
    {
        dclear(C_WHITE);
        dsubimage(0, 0, img_fonds, 0, 260, 128, 64, 0);
        
        for (i = 0 ; i <= 7 ; i++)
        {
            for (j = 0 ; j <= 3; j++)
            {
                if (tableau[j][i] != 0) dsubimage(16 * i, 16 * j, &img_muta, 16 * (menu_muta - 1), 16 * (tableau[j][i] - 1), 15, 15, 0);
            }
        }
        if (tableau[y][x] == lim) dsubimage(16 * x - 1, 16 * y - 1, &img_pieces, 0, 18, 17, 17, 0);
        else dsubimage(16 * x - 1, 16 * y - 1, &img_pieces, 0, 0, 17, 17, 0);
        dupdate();
        key = getkey().key;
        switch (key)
        {
            case KEY_LEFT:
                if (x > 0) x -= 1;
                break;
            case KEY_RIGHT:
                if (x < 8) x += 1;
                break;
            case KEY_UP:
                if (y > 0) y -= 1;
                break;
            case KEY_DOWN:
                if (y < 4) y += 1;
                break;
            case KEY_EXIT:
                fin = 1;
                break;
        }
    }
    return 0;
}


void modele_infectieux (double *sains, double *infectes, double *morts, double *gueris, double *s_avant, double *i_avant, double *m_avant, double *g_avant, int *contagion, int *severite, int *letalite, int *changement)
{
    double r = *contagion / 2600, a = *severite / (200 - *changement), b = *letalite / 3300;
    
    *sains = floor(*s_avant - r * *s_avant);
    *infectes = floor (*i_avant + r * *s_avant - *i_avant * b - *i_avant * a);
    *morts = floor (*m_avant + *infectes * b);
    *gueris = floor(*g_avant + *infectes * a);
    
    *s_avant = *sains;
    *i_avant = *infectes;
    *m_avant = *morts;
    *g_avant = *gueris;
}


double floor (double x)
{
    return (int)x;
}


void init_mat(int x, int y, int dest[][x], int src[][x])
{
    for (int i = 0 ; i < x ; i++)
    {
        for (int j = 0 ; j < y ; j++)
        {
            dest[j][i] = src[j][i];
        }
    }
}
