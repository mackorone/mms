#include "Cell.h"
#include "Cellmod.h"

#include <stdbool.h>

struct Cellmod * createCellmod(struct Cell *incell, struct Cell *inoldPrev, int inoldDist, bool inoldExplored, bool inoldTraversed,
                               bool *inoldWalls, bool *inoldWallsInspected) {

    struct Cellmod *retVal = malloc(sizeof(struct Cellmod));
    retVal->cell = incell; // A pointer to the particular cell that has been modified
    retVal->oldPrev = inoldPrev; // The old prev cell of the cell
    retVal->oldDist = inoldDist; // The old distance of the cell
    retVal->oldExplored = inoldExplored; // The old exploredness of the cell (not sure if we need or if we can set to false by default)
    retVal->oldTraversed = inoldTraversed; // The old traversedness of the cell (again, not sure if this is necessary)
    retVal->oldWalls[0] = *inoldWalls;
    retVal->oldWalls[1] = *(inoldWalls + 1);
    retVal->oldWalls[2] = *(inoldWalls + 2);
    retVal->oldWalls[3] = *(inoldWalls + 3);
    retVal->oldWallsInspected[0] = *inoldWallsInspected;
    retVal->oldWallsInspected[1] = *(inoldWallsInspected+1);
    retVal->oldWallsInspected[2] = *(inoldWallsInspected+2);
    retVal->oldWallsInspected[3] = *(inoldWallsInspected+3);

    return retVal;
    
}

struct Cellmod * copyOfCellmod(struct Cellmod *original) {

    struct Cellmod *newMod = malloc(sizeof(struct Cellmod));
    newMod->cell = original->cell;
    newMod->oldPrev = original->oldPrev;
    newMod->oldDist = original->oldDist;
    newMod->oldExplored = original->oldExplored;
    newMod->oldTraversed = original->oldTraversed;
    newMod->oldWalls[0] = original->oldWalls[0];
    newMod->oldWalls[1] = original->oldWalls[1];
    newMod->oldWalls[2] = original->oldWalls[2];
    newMod->oldWalls[3] = original->oldWalls[3];
    newMod->oldWallsInspected[0] = original->oldWallsInspected[0];
    newMod->oldWallsInspected[1] = original->oldWallsInspected[1];
    newMod->oldWallsInspected[2] = original->oldWallsInspected[2];
    newMod->oldWallsInspected[3] = original->oldWallsInspected[3];

    return newMod;

}

void destroyCellmod (Cellmod *trash) {
    free(trash);
}
