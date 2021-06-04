#include <gint/keyboard.h>
#include <gint/timer.h>
#include <gint/clock.h>
#include <math.h>

#include "mutation_engine.h"


void get_mutation(const struct game *current_game, const int mutation_menu, int table[4][8])
{
    // Load symptoms
    extern struct mutation_table mt_symptoms_1;
    extern struct mutation_table mt_symptoms_2;
    extern struct mutation_table mt_symptoms_3;
    extern struct mutation_table mt_symptoms_4;

    // Load abilities
    extern struct mutation_table mt_abilities_1;
    extern struct mutation_table mt_abilities_2;
    extern struct mutation_table mt_abilities_3;

    // Load transmissions
    extern struct mutation_table mt_transmissions_1;
    extern struct mutation_table mt_transmissions_2;
    extern struct mutation_table mt_transmissions_3;

    if (mutation_menu == 1)
    {   
        if (current_game->mutations_count[0] < 4) init_mat(8, 4, table, mt_symptoms_1.data);
        else if (current_game->mutations_count[0] < 8) init_mat(8, 4, table, mt_symptoms_2.data);
        else if (current_game->mutations_count[0] < 11) init_mat(8, 4, table, mt_symptoms_3.data);
        else init_mat(8, 4, table, mt_symptoms_4.data);
    }

    if (mutation_menu == 2)
    {
        if (current_game->mutations_count[1] < 1) init_mat(8, 4, table, mt_abilities_1.data);
        else if (current_game->mutations_count[1] < 3) init_mat(8, 4, table, mt_abilities_2.data);
        else init_mat(8, 4, table, mt_abilities_3.data);
    }

    if (mutation_menu == 3)
    {
        if (current_game->mutations_count[2] < 5) init_mat(8, 4, table, mt_transmissions_1.data);
        else if (current_game->mutations_count[2] < 10) init_mat(8, 4, table, mt_transmissions_2.data);
        else init_mat(8, 4, table, mt_transmissions_3.data);   
    }
}


void mutation_select(struct game *current_game, const int mutation_menu)
{
    int key = 0, end = 0;
    struct cursor c = {0, 0, 0};

    static volatile int tick = 1;
    int t = timer_configure(TIMER_ANY, CURSOR_TICK*1000, GINT_CALL(callback_tick, &tick));
    if (t >= 0) timer_start(t);


    while (!end)
    {   
        // Cursor blinking gestion
        while (!tick) sleep();
        tick = 0;
        c.display = (c.display + 1) % 2;

        // Get and display the mutation menu
        int table[4][8];
        get_mutation(current_game, mutation_menu, table);
        display_mutation(table, c, mutation_menu);
        
        // Get the key
        key = rtc_key();
        
        // Manage input
        if (key == KEY_EXIT) end = 1;
        if (key == KEY_EXE && table[c.y][c.x] != 15 && table[c.y][c.x] != 0)
        {
            end = mutation_buy(current_game, c, mutation_menu, table);
        }
                    
        if (key == KEY_LEFT && c.x > 0) c.x --;
        if (key == KEY_RIGHT && c.x < 7) c.x ++;
        if (key == KEY_UP && c.y > 0) c.y --;
        if (key == KEY_DOWN && c.y < 3) c.y ++;
    }
    if (t >= 0) timer_stop(t);
}


int mutation_buy(struct game *current_game, const struct cursor c, const int mutation_menu, const int table[4][8])
{
    int button_selected = 1;
    int key = 0, id = table[c.y][c.x];

    struct mutation *mutation_data = get_mutation_data(mutation_menu, table[c.y][c.x]);

    while (1)
    {
        display_mutation_buy(c, mutation_menu, table, button_selected, current_game);
        key = getkey().key;

        if (key == KEY_DOWN || key == KEY_UP) button_selected = (button_selected + 1) % 2;
        if (key == KEY_EXIT) return 1;
        if (key == KEY_EXE)
        {
            if (!button_selected) return 0;
            else
            {
                // if the player has'nt bought this mutation yet
                if (!current_game->mutations_bought[mutation_menu - 1][id - 1])
                {
                    // if the player has enought DNA points
                    if (current_game->dna >= mutation_data->dna)
                    {
                        // Take DNA points and save the purchase
                        current_game->dna = current_game->dna - mutation_data->dna;
                        current_game->mutations_bought[mutation_menu - 1][id - 1] = 1;
                        current_game->mutations_count[mutation_menu - 1] ++;
                        current_game->mutations_selected[mutation_menu - 1] = id;

                        // Update
                        update_disease(current_game);
                        const char *msg[5] = {"mutation", "achetee", "", "", ""};
                        message(msg);
                    }
                    else {const char *msg[5] = {"achat", "impossible", "", "", ""}; message(msg);}
                }

                // if the player has already bought this mutation
                else
                {
                    current_game->mutations_selected[mutation_menu - 1] = id;
                    update_disease(current_game);
                    const char *msg[5] = {"mutation", "selectionnee", "", "", ""};
                    message(msg);
                }
                
            }
            return 1;
        }
    }
    return 0;
}


void update_disease(struct game *current_game)
{
    struct mutation *symptom = get_mutation_data(1, current_game->mutations_selected[0]);
    struct mutation *ability = get_mutation_data(2, current_game->mutations_selected[1]);
    struct mutation *transmission = get_mutation_data(3, current_game->mutations_selected[2]);

    // disease parameters
    current_game->contagion = symptom->contagion + ability->contagion + transmission->contagion;
    current_game->severity = symptom->severity + ability->severity + transmission->severity;
    current_game->lethality = symptom->lethality + ability->lethality + transmission->lethality;
    
    // research parameters
    current_game->limit = RESEARCH_LIMIT + symptom->changement + ability->changement + transmission->changement;
    current_game->priority = floor((current_game->severity + current_game->lethality) / 40);
}


struct mutation *get_mutation_data(const int mutation_menu, const int id)
{
    extern struct mutation symptoms_data[14];
    extern struct mutation abilities_data[6];
    extern struct mutation transmissions_data[13];
    extern struct mutation default_value;

    if (!id) return &default_value;

    struct mutation *mutation_data = &default_value;

    switch (mutation_menu)
    {
        case 1:
            mutation_data = &symptoms_data[id - 1];
            break;

        case 2:
            mutation_data = &abilities_data[id - 1];
            break;

        case 3:
            mutation_data = &transmissions_data[id - 1];
            break;
    }
    return mutation_data;
}


void init_mat(int x, int y, int dest[][x], int src[][x])
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            dest[j][i] = src[j][i];
        }
    }
}
