#ifndef HISTORY_H_
#define HISTORY_H_

#include "Cell.h"
#include "Cellmod.h"
#include "CellStack.h"
#include "MStacksQueue.h"

#include <stdbool.h>

struct History {

    // The short term memory of the robot
    int m_stm; 

    // The number of steps since reaching the last checkpoint
    int m_size;

    // Checkpoint objects
    struct Cell *m_checkpointCell;
    struct CellStack *m_checkpointStack; // NOTE: JIMMY MADE THIS A POINTER

    // Queue of stacks for explore
    struct MStacksQueue m_stacks;

    // Queue of ints counting the references for each stack in m_stacks
    struct List * m_stackReferenceCounts; // of ints

    // List of List of cell modification objects for each step.
    struct List * m_modifiedCells; // of list of cellmod

};

// Set the history object for the origin
void h_initialize(struct History *hist, int stm, struct Cell *origin, bool firstTime);

// Checkpoint accessors
struct Cell * getCheckpointCell(struct History *); // Returns a pointer the checkpoint cell
struct CellStack * getCheckpointStack(struct History *); // Returns the stack that was present at the most recent checkpoint
struct CellStack * getCheckpointPath(struct History *); // Returns the path to the checkpoint cell

// Called by Floodfill to update the history object
void moved(struct History *);
void stackUpdate(struct History *, struct CellStack *newStack); // JIMMY CHANGED PARAMETER TO POINTER
void modifiedCellsUpdate(struct History *, struct List * ); // JIMMY CHANGED PARAMETER TO POINTER

// Resets the modifications to each of the cells stored in short term mem
void resetModifiedCells(struct History *);

#endif // HISTORY_H_
