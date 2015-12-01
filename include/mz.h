#ifndef MZ_H
#define MZ_H

#include <stdbool.h>

enum {
    MZ_SUCCESS              = 0,
    MZ_OUT_OF_MEMORY,
    MZ_INVALID_ARGUMENTS
};

typedef struct {
    float min[2];
    float max[2];
    float extent[2];
} mz_domain;

typedef struct {
    float (*positions)[2];
    float (*velocities)[2];
    float *lambdas;                                     /* step sizes        */
    float *dpositions[2];                               /* positions updates */
#ifdef
    float *densities;
    int num_particles;
} mz_particles;

/*
 * Acceleration structure for quickly finding particle neighbors.
 * TODO: Sorting particles by their cell id instead of storing [ids] might
 *       increase overall memory coherency.
 * TODO: Using Z-ordering might increase memory coherency.
 */
typedef struct {
    mz_domain domain;
    float dx;
    int num_cells_total;
    int num_cells[2];
    int *num_particles;                 /* # particles for each cell.         */
    int *start_ids;                     /* reference to the first particle id
                                           for each cell in [ids].            */
    int *ids;                           /* particle ids for referencing
                                           particles within a cell            */
} mz_grid;

int mz_make_domain(
	mz_domain *domain,
    float xmin,
    float ymin,
    float xmax,
    float ymax
);


#define mz_grid_index_from_coord(grid, coord)                                  \
    (coord)[0] * (grid)->num_cells[0] + (coord)[1]

int mz_init_particles(mz_particles *particles, unsigned int num_particles);
void mz_deinit_particles(mz_particles *particles);

int mz_init_grid(
	mz_grid *grid,
    const mz_particles *particles,
    const mz_domain *domain,
    float dx
);
int mz_update_grid(mz_grid *grid, const mz_particles *particles);
void mz_deinit_grid(mz_grid *grid);

/*
 * Computes the grid coordinate of the cell containing [position] and stores
 * it in [coord]. Returns false if the position is outside the grid, otherwise
 * true.
 */
bool mz_grid_coord_from_position(
	const mz_grid *grid,
    int coord[2],
    const float position[2]
);

/*
 * Computes the cell index for the cell containing [position] and stores it in
 * [index]. Returns false if the position is outside the grid, otherwise true.
 */
bool mz_grid_index_from_position(
	const mz_grid *grid,
    int *index,
	float position[2]);

int mz_enforce_incompressibility(
	mz_particles *particles,
    const mz_grid *grid,
    float rest_density,
    float support
);

#endif /* end of include guard: PARTICLES_H */
