#ifndef MZ_H
#define MZ_H

typedef struct {
    float       min[3];
    float       max[3];
} mz_domain;

typedef struct {
    float       rest_density;
    float       gravity;
    mz_domain   domain;
} mz_params;

typedef struct mz_particles mz_particles;

mz_particles *mz_particles_new(mz_params *params);
void mz_particles_delete(mz_particles *particles);

typedef struct mz_emitter mz_emitter;

mz_emitter *mz_emitter_new(unsigned int num_particles);
void mz_emitter_delete(mz_emitter *emitter);
void mz_emitter_set(mz_emitter *emitter, unsigned int i, float position[3]);
int mz_emitter_emit(const mz_emitter *emitter, mz_particles *particles);

#endif /* end of include guard: PARTICLES_H */
