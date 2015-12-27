#include <mz.h>
#include "common.h"

int mz_init_fluid(
    struct mz_fluid *fluid,
    double rest_density,
    unsigned int num_particles
) {
    if (!fluid)
        return MZ_INVALID_ARGUMENTS;
    memset(fluid, 0, sizeof(struct mz_fluid));
    fluid->num_particles = num_particles;
    mz_calloc(fluid->positions, num_particles, sizeof(double[2]));
    mz_calloc(fluid->velocities, num_particles, sizeof(double[2]));
    mz_calloc(fluid->lambdas, num_particles, sizeof(double));
    mz_calloc(fluid->dpositions, num_particles, sizeof(double[2]));
    mz_calloc(fluid->densities, num_particles, sizeof(double));
    fluid->rest_density = rest_density;
    return MZ_SUCCESS;

error:
    mz_deinit_fluid(fluid);
    return MZ_OUT_OF_MEMORY;
}

void mz_deinit_fluid(struct mz_fluid *fluid) {
    free(fluid->positions);
    free(fluid->velocities);
    free(fluid->lambdas);
    free(fluid->dpositions);
    free(fluid->densities);
    memset(fluid, 0, sizeof(struct mz_fluid));
}

int mz_copy_fluid(struct mz_fluid *dst, const struct mz_fluid *src) {
    if (dst->num_particles != src->num_particles) {
        mz_debug("Particle count does not match.\n");
        return MZ_INVALID_ARGUMENTS;
    }

    memcpy(dst->positions, src->positions, sizeof(double[2]) * src->num_particles);
    memcpy(dst->velocities, src->velocities, sizeof(double[2]) * src->num_particles);
    memcpy(dst->lambdas, src->lambdas, sizeof(double) * src->num_particles);
    memcpy(dst->dpositions, src->dpositions, sizeof(double[2]) * src->num_particles);
    memcpy(dst->densities, src->densities, sizeof(double) * src->num_particles);
    dst->rest_density = src->rest_density;
    dst->num_particles = src->num_particles;
    return MZ_SUCCESS;
}

