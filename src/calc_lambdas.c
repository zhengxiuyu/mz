#include <mz.h>
#include "common.h"

#define EPS 0.0001

#define is_vald_coord(grid, coord)                                            \
    (mz_inrange((coord)[0], 0, (grid)->num_cells[0] - 1) &&                   \
        mz_inrange((coord)[1], 0, (grid)->num_cells[1] - 1))

static float eval_poly6(float r, float h) {
    const float c = 4.0 / M_PI;
    float h2 = h * h;
    float h8 = h2 * h2 * h2 * h2;
    float a = h2 - r * r;
    float b = c / h8;
    return b * a * a * a;
}

static float eval_spiky_grad(float r, float h) {
    const float c = -30.0 / M_PI;
    float h5 = h * h * h * h * h;
    float b = h - r;
    return c / (h5 * r) * b * b;
}

/*
 * Update the density and constraint gradient from particles within a grid cell.
 */
static void calc_quantities_cell(
    mz_particles *particles,
    int particle_index,
    const mz_grid *grid,
    int cell_index,
    float *density,                 /* [out] updated density                 */
    float *mag_gradient,            /* [out] updated constraint gradient     */
    const float position[2],
    float support
) {
    int i = grid->start_ids[cell_index];
    int imax = i + grid->num_particles[cell_index];

    for (; i < imax; i++) {
        int k = grid->ids[i];
        float dot, diff[2], r, w;;

        mz_sub(diff, position, particles->positions[k]);
        dot = mz_dot(diff, diff);
        if (dot > support * support)
            continue;
        r = sqrtf(dot);
        w = eval_poly6(r, support);
        *density += w;
        
        /* eval gradient */
        if (k == particle_index) {
            float spiky = eval_spiky_grad(r, support);
            gradient[0] += spiky * diff[0];
            gradient[1] += spiky * diff[1];
        } else {

        }
    }
}

int mz_calc_lambdas(
    mz_particles *particles,
    const mz_grid *grid,
    float rest_density,
    float support
) {
    int i = 0;
    int cp[2], cc[2];                           /* grid coordinates          */
    int index;                                  /* particle index            */
    float dens = 0.0;                           /* density for particle i    */
    float grad[2];                              /* constraint gradient       */

    if (support != grid->dx) {
        mz_debug("Expected support to equal grid spacing");
        return MZ_INVALID_ARGUMENTS;
    }

    for (i = 0; i < particles->num_particles; i++) {
        dens = 0.0;
        memset(grad, 0, sizeof(float[2]));
        mz_grid_coord_from_position(grid, cp, particles->positions[i]);
        for (cc[0] = cp[0] - 1; cc[0] <= cp[0] + 1; cc[0]++) {
            for (cc[1] = cp[1] - 1; cc[1] <= cp[1] + 1; cc[1]++) {
                index = mz_grid_index_from_coord(grid, cc);
                if (!is_vald_coord(grid, cc))
                    continue;
                calc_quantities_cell(particles, i, grid, index, &dens, grad,
                    particles->positions[i], support);
            }
        }
        dens = dens / rest_density - 1.0;
    }
    return MZ_SUCCESS;
}


