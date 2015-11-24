#ifndef MZ_H
#define MZ_H

enum {
    MZ_SUCCESS              = 0,
    MZ_OUT_OF_MEMORY,
    MZ_INVALID_ARGUMENTS
};

typedef struct {
    float       min[2];
    float       max[2];
} mz_domain;

typedef struct {
    float       rest_density;
    float       gravity;
    mz_domain   domain;
} mz_params;

typedef struct {
    float           (*positions)[2];
    float           (*velocities)[2];
    float           (*constraints);
    float           (*constraint_gradients)[2];
    unsigned int    num_particles;
} mz_particles;

int mz_init_particles(mz_particles *particles, unsigned int num_particles);
void mz_deinit_particles(mz_particles *particles);

/*
 * Acceleration structure for quickly finding particle neighbors.
 * TODO: Sorting particles by their cell id instead of storing [ids] might
 *       increase overall memory coherency.
 * TODO: Using Z-ordering might increase memory coherency.
 */
typedef struct {
    mz_domain       domain;
    float           extent[2];
    float           dx;
    unsigned int    num_cells_total;
    unsigned int    num_cells[2];
    unsigned int    *num_faces;         /* # faces for each cell. */
    unsigned int    *start_ids;         /* reference to the first particle id
                                           for each cell in [ids]. */
    unsigned int    *ids;               /* particle ids for referencing particles
                                           within a cell */
} mz_grid;

int mz_init_grid(mz_grid *grid, const mz_particles *particles,
                 const mz_domain *domain, float dx);
void mz_deinit_grid(mz_grid *grid);
int mz_update_grid(mz_grid *grid, const mz_particles *particles);


#endif /* end of include guard: PARTICLES_H */
