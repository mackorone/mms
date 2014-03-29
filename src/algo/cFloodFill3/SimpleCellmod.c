#include "Cell.h"
#include "SimpleCellmod.h"

#include <stdbool.h>

struct SimpleCellmod * createSimpleCellmod(struct Cell *incell, struct Cell *inprev, int indist, bool *inwalls) {

    struct SimpleCellmod *retVal = malloc(sizeof(struct SimpleCellmod));
    retVal->cell = incell; // A pointer to the particular cell that has been modified
    retVal->prev = inprev; // The old prev cell of the cell
    retVal->dist = indist; // The old distance of the cell
    retVal->walls[0] = *inwalls;
    retVal->walls[1] = *(inwalls + 1);
    retVal->walls[2] = *(inwalls + 2);
    retVal->walls[3] = *(inwalls + 3);

    return retVal;
    
}

struct SimpleCellmod * copyOfSimpleCellmod(struct SimpleCellmod *original);

    struct SimpleCellmod *newMod = malloc(sizeof(struct SimpleCellmod));
    newMod->cell = original->cell;
    newMod->prev = original->prev;
    newMod->dist = original->dist;
    newMod->walls[0] = original->walls[0];
    newMod->walls[1] = original->walls[1];
    newMod->walls[2] = original->walls[2];
    newMod->walls[3] = original->walls[3];

    return newMod;

}

void destroySimpleCellmod (SimpleCellmod *trash) {
    free(trash);
}
