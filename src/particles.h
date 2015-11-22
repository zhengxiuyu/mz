#ifndef PARTICLES_H
#define PARTICLES_H

#include "grid.h"

struct mz_particles {
    mz_grid         *grid;
    float           (*positions)[2];
    float           *constraints;
    float           (*constraint_gradients)[2];
    unsigned int    num_particles;
};

#endif /* end of include guard: PARTICLES_H */
