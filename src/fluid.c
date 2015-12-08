#include <mz.h>
#include "common.h"

int mz_init_fluid(mz_fluid *fluid, unsigned int num_particles) {
    if (!fluid)
        return MZ_INVALID_ARGUMENTS;
    memset(fluid, 0, sizeof(mz_fluid));
    fluid->num_particles = num_particles;
    mz_calloc(fluid->positions, num_particles, sizeof(float[2]));
    mz_calloc(fluid->velocities, num_particles, sizeof(float[2]));
    mz_calloc(fluid->lambdas, num_particles, sizeof(float));
    mz_calloc(fluid->dpositions, num_particles, sizeof(float[2]));
    return MZ_SUCCESS;

error:
    mz_deinit_fluid(fluid);
    return MZ_OUT_OF_MEMORY;
}

void mz_deinit_fluid(mz_fluid *fluid) {
    free(fluid->positions);
    free(fluid->velocities);
    free(fluid->lambdas);
    free(fluid->dpositions);
    memset(fluid, 0, sizeof(mz_fluid));
}

