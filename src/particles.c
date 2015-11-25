#include <mz.h>
#include "common.h"

int mz_init_particles(mz_particles *particles, unsigned int num_particles)
{
    if (!particles) return MZ_INVALID_ARGUMENTS;
    memset(particles, 0, sizeof(mz_particles));
    particles->num_particles = num_particles;
    particles->positions = calloc(num_particles, sizeof(float[2]));
    mz_checkmem(particles->positions);
    particles->velocities = calloc(num_particles, sizeof(float[2]));
    mz_checkmem(particles->velocities);
    particles->constraints = calloc(num_particles, sizeof(float));
    mz_checkmem(particles->constraints);
    particles->constraint_gradients = calloc(num_particles, sizeof(float[2]));
    mz_checkmem(particles->constraint_gradients);
    return MZ_SUCCESS;

error:
    mz_deinit_particles(particles);
    return MZ_OUT_OF_MEMORY;
}

void mz_deinit_particles(mz_particles *particles)
{
    free(particles->positions);
    free(particles->velocities);
    free(particles->constraints);
    free(particles->constraint_gradients);
    memset(particles, 0, sizeof(mz_particles));
}

