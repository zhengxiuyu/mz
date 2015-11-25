#include <mz.h>
#include "common.h"

int mz_init_grid(mz_grid *grid, const mz_particles *particles,
             const mz_domain *domain, float dx)
{
    if (!grid  || !particles || dx <= 0.0f) return MZ_INVALID_ARGUMENTS;
    memset(grid, 0, sizeof(mz_grid));
    grid->domain = *domain;
    grid->dx = dx;
    grid->extent[0] = domain->max[0] - domain->min[0];
    grid->extent[1] = domain->max[1] - domain->min[1];
    grid->num_cells[0] = ceil(grid->extent[0] / dx);
    grid->num_cells[1] = ceil(grid->extent[1] / dx);
    grid->num_cells_total = grid->num_cells[0] * grid->num_cells[1];
    grid->num_faces = calloc(grid->num_cells_total, sizeof(unsigned int));
    mz_checkmem(grid->num_faces);
    grid->start_ids = calloc(grid->num_cells_total, sizeof(unsigned int));
    mz_checkmem(grid->start_ids);
    grid->ids = calloc(particles->num_particles, sizeof(unsigned int));
    mz_checkmem(grid->ids);
    return MZ_SUCCESS;

error:
    mz_deinit_grid(grid);
    return MZ_OUT_OF_MEMORY;
}

void mz_deinit_grid(mz_grid *grid)
{
    free(grid->num_faces);
    free(grid->start_ids);
    free(grid->ids);
    memset(grid, 0, sizeof(mz_grid));
}

int mz_update_grid(mz_grid *grid, const mz_particles *particles)
{
    size_t size;
    unsigned int i;

    if (!grid || !particles) return MZ_INVALID_ARGUMENTS;

    /* reset per cell data */
    size = grid->num_cells_total * sizeof(unsigned int);
    memset(grid->num_cells, 0, size);
    for (i = 0; i < grid->num_cells_total; i++)
        grid->start_ids[i] = -1;
    /* TODO : The rest ... */
    return MZ_SUCCESS;
}

void mz_grid_coord_from_position(const mz_grid *grid, int coordinate[2],
                                 const float position[2])
{
    coordinate[0] = (position[0] - grid->domain.min[0]) / grid->dx;
    coordinate[1] = (position[1] - grid->domain.min[1]) / grid->dx;
    coordinate[0] = mz_clamp(coordinate[0], 0, grid->num_cells[0] - 1);
    coordinate[1] = mz_clamp(coordinate[1], 0, grid->num_cells[1] - 1);
}

int mz_grid_index_from_position(const mz_grid *grid, float position[2])
{
    int coord[2];
    mz_grid_coord_from_position(grid, coord, position);
    return mz_grid_index_from_coord(grid, coord);
}

