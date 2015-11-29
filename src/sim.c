#include <mz.h>
#include "common.h"

static float eval_poly6(float r, float h)
{
    const float c = 4.0 / M_PI;
    float h2 = h * h;
    float h8 = h2 * h2 * h2 * h2;
    float a = h2 - r * r;
    float b = c / h8;
    return b * a * a * a;
}

static void calc_constraints(mz_particles *particles, const mz_grid *grid,
                             float rest_density, float support)
{
    int i = 0;

    for (i = 0; i < particles->num_particles; i++) {
         /* ... */
    }
}

int mz_enforce_incompressibility(mz_particles *particles, const mz_grid *grid,
                                 float rest_density, float support)
{
    if (support != grid->dx) {
        mz_debug("Expected support to be equal to grid cell size.\n");
        return MZ_INVALID_ARGUMENTS;
    }

    calc_constraints(particles, grid, rest_density, support);
    return MZ_SUCCESS;
}
