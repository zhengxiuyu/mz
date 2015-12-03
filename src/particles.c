#include <mz.h>
#include "common.h"

int mz_init_particles(mz_particles *particles, unsigned int num_particles) {
    if (!particles)
        return MZ_INVALID_ARGUMENTS;
    memset(particles, 0, sizeof(mz_particles));
    particles->num_particles = num_particles;
    particles->positions = calloc(num_particles, sizeof(float[2]));
    mz_checkmem(particles->positions);
    particles->velocities = calloc(num_particles, sizeof(float[2]));
    mz_checkmem(particles->velocities);
    particles->lambdas = calloc(num_particles, sizeof(float));
    mz_checkmem(particles->lambdas);
    particles->dpositions = calloc(num_particles, sizeof(float[2]));
    mz_checkmem(particles->dpositions);
    return MZ_SUCCESS;

error:
    mz_deinit_particles(particles);
    return MZ_OUT_OF_MEMORY;
}

void mz_deinit_particles(mz_particles *particles) {
    free(particles->positions);
    free(particles->velocities);
    free(particles->lambdas);
    free(particles->dpositions);
    memset(particles, 0, sizeof(mz_particles));
}

