#include <mz.h>
#include "common.h"

static float
eval_poly6(float r, float h)
{
    const float c = 4.0 / M_PI;
    float h2 = h * h;
    float h8 = h2 * h2 * h2 * h2;
    float a = h2 - r * r;
    float b = c / h8;
    return b * a * a * a;
}

static void
calc_constraints_cell(mz_particles *particles, const mz_grid *grid,
                      int index, float *density, const float position[2],
                      float support)
{
    int i = grid->start_ids[index];
    int end = i + grid->num_particles[index];
    int id;
    float dot, diff[2], r, w;

    for (; i < end; i++) {
        id = grid->ids[i];
        mz_sub(diff, position, particles->positions[id]);
        dot = mz_dot(diff, diff);
        if (dot > support * support) continue;
        r = sqrtf(dot);
        w = eval_poly6(r, support);
        *density += w;
        /* TODO! */
    }
}

static void
calc_constraints(mz_particles *particles, const mz_grid *grid,
                 float rest_density, float support)
{
    int i = 0;
    int cp[2], cc[2];               /* grid coordinates */
    int index;                      /* particle index   */

    for (i = 0; i < particles->num_particles; i++) {
        float dens = 0.0;           /* density for particle i */

        mz_grid_coord_from_position(grid, cp, particles->positions[i]);
        for (cc[0] = cp[0] - 1; cc[0] <= cp[0] + 1; cc[0]++) {
            for (cc[1] = cp[1] - 1; cc[1] <= cp[1] + 1; cc[1]++) {
                index = mz_grid_index_from_coord(grid, cc);
                if (!mz_inrange(cc[0], 0, grid->num_cells[0] - 1) ||
                    !mz_inrange(cc[1], 0, grid->num_cells[1] - 1))
                    continue;
                calc_constraints_cell(particles, grid, index, &index,
                                      particles->position[i], support);
            }
        }
    }
}

int
mz_enforce_incompressibility(mz_particles *particles, const mz_grid *grid,
                             float rest_density, float support)
{
    if (support != grid->dx) {
        mz_debug("Expected support to be equal to grid cell size.\n");
        return MZ_INVALID_ARGUMENTS;
    }

    calc_constraints(particles, grid, rest_density, support);
    return MZ_SUCCESS;
}

