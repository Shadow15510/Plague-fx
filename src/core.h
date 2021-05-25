#ifndef _PLAGUE_CORE_H
#define _PLAGUE_CORE_H


// Duration for internal clock
#define ENGINE_TICK 50

// Number of planes on screen
#define NB_PLANES 5

// Number of humans
#define TOTAL_POP 10000000000


// game : all statistics of the current game
struct game
{
    // Disease parameters
    int contagion, severity, lethality;

    // DNA points
    int dna;

    // Mutations counts and sprite selected
    int abilities, symptoms, transmissions;
    int abilities_sel, symptoms_sel, transmissions_sel;

    // Research data
    int research, limit;

    // Infectious pattern parameters
    long long int healthy, infected, dead, cured;

    // Time
    int time;

    // Planes
    struct plane *planes[NB_PLANES + 1];
};


// plane : information about planes
struct plane
{
    // Plane's coordinates
    int x, y;

    // Plane's direction
    int direction;

    // Coordinates of the plane's destination
    int dest_x, dest_y;

    //Coordinates of the plane's departure
    int depa_x, depa_y;

};


// mutation : contain the map of the mutation available
struct mutation_table
{
    // Data of the map
    int data[4][8];
};


// get_inputs : detect and manage inputs
int get_inputs(const int background, int *mutation_menu);


// next_frame : compute the plane's positions
void next_frame(struct game *current_game);


// manage_mutation : an independant sub-programm which allow to select, buy and see the mutations.
void manage_mutation(struct game *current_game, const int mutation_menu);

#endif /* _PLAGUE_CORE_H */