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
                      int index, const float position[2],
                      float rest_density, float support)
{
    int i = grid->start_ids[index];
    int end = start + grid->num_particles[index];
    int id;
    float dot, diff[2], r, w;

    for (; i < end; i++) {
        id = grid->ids[i];
        mz_diff(diff, position, particles->position[id]);
        dot = mz_dot(diff, diff);
        if (dot > support * support) continue;
        r = sqrtf(dot);
        w = eval_poly6(r, support);
        /* TODO! */
    }
}

static void
calc_constraints(mz_particles *particles, const mz_grid *grid,
                 float rest_density, float support)
{
    int i = 0;
    int coord[2], coord2[2], index;

    for (i = 0; i < particles->num_particles; i++) {
        mz_grid_coord_from_position(grid, coord, particles->positions[i]);

        for (coord2[0] = coord[0] - 1; coord2[0] <= coord[0] + 1; coord2[0]++) {
            for (coord2[1] = coord[1] - 1; coord2[1] <= coord[1] + 1; coord2[1]++) {
                index = mz_grid_index_from_coord(grid, coord2);

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

