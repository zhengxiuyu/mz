#include <mz.h>
#include "common.h"

int mz_init_fluid(
    mz_fluid *fluid,
    float rest_density,
    unsigned int num_particles
) {
    if (!fluid)
        return MZ_INVALID_ARGUMENTS;
    memset(fluid, 0, sizeof(mz_fluid));
    fluid->num_particles = num_particles;
    mz_calloc(fluid->positions, num_particles, sizeof(float[2]));
    mz_calloc(fluid->velocities, num_particles, sizeof(float[2]));
    mz_calloc(fluid->lambdas, num_particles, sizeof(float));
    mz_calloc(fluid->dpositions, num_particles, sizeof(float[2]));
    mz_calloc(fluid->densities, num_particles, sizeof(float));
    fluid->rest_density = rest_density;
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
    free(fluid->densities);
    memset(fluid, 0, sizeof(mz_fluid));
}

int mz_copy_fluid(mz_fluid *dst, const mz_fluid *src) {
    if (dst->num_particles != src->num_particles) {
        mz_debug("Particle count does not match.\n");
        return MZ_INVALID_ARGUMENTS;
    }

    memcpy(dst->positions, src->positions, sizeof(float[2]) * src->num_particles);
    memcpy(dst->velocities, src->velocities, sizeof(float[2]) * src->num_particles);
    memcpy(dst->lambdas, src->lambdas, sizeof(float) * src->num_particles);
    memcpy(dst->dpositions, src->dpositions, sizeof(float[2]) * src->num_particles);
    memcpy(dst->densities, src->densities, sizeof(float) * src->num_particles);
    dst->rest_density = src->rest_density;
    dst->num_particles = src->num_particles;
    return MZ_SUCCESS;
}
