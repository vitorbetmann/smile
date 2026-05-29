#pragma once

// Data Types ——————————————————————————————————————————————————————————————————————————————————————

/** @brief Represents a single active particle in the system. */
typedef struct
{
    float x, y; /**< Current position. */
    float velocityX, velocityY; /**< Current velocity components. */
    float accelerationX, accelerationY; /**< Current acceleration components. */
    float age; /**< Time elapsed since emission, in seconds. */
    float lifetime;
    /**< Total lifespan of the particle, in seconds. Pass INFINITY for an immortal particle. */
    float angle; /**< Current rotation angle, in radians. */
    float angularVelocity; /**< Rotation speed, in radians per second. */
} Particle;

/**
 * @brief Geometric shape used to sample particle spawn positions within the emission area.
 */
typedef enum
{
    PS_SHAPE_ELLIPSE, /**< Particles emitted within an elliptical area. */
    PS_SHAPE_RECT, /**< Particles emitted within a rectangular area. */
    SHAPE_COUNT /**< Number of emission shapes (sentinel). */
} psEmissionShape;

/** @brief Opaque handle representing a ParticleSystem instance. */
typedef struct psInternalParticleSystem ParticleSystem;

/** @brief Callback invoked for each active particle during psForEach. */
typedef void (*ParticleFn)(const Particle *p, void *context);

/** @brief Callback that modifies a particle once per update step. */
typedef void (*InfluenceFn)(Particle *p, void *context);

/** @brief Callback that modifies a particle using a pre-update snapshot of all active particles. */
typedef void (*SnapshotInfluenceFn)(Particle *p, const Particle *snapshot, int count,
                                    void *context);

// Prototypes ——————————————————————————————————————————————————————————————————————————————————————

// Lifecycle

/**
 * @brief Allocates and initializes a new ParticleSystem with the given capacity and origin.
 *
 * @param maxParticles Maximum number of particles the system can hold.
 * @param originX      X coordinate of the emission origin.
 * @param originY      Y coordinate of the emission origin.
 *
 * @return Pointer to the new ParticleSystem (caller-owned), or nullptr on failure.
 */
ParticleSystem *psCreate(int maxParticles, float originX, float originY);

/**
 * @brief Frees all memory associated with the ParticleSystem.
 *
 * @param ps ParticleSystem to destroy.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int psDestroy(ParticleSystem *ps);

/**
 * @brief Deactivates all particles without freeing the system or clearing the stream accumulator.
 *
 * @param ps ParticleSystem to reset.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int psReset(ParticleSystem *ps);

// Emission

/**
 * @brief Emits a fixed number of particles immediately.
 *
 * @param ps     ParticleSystem to emit from.
 * @param amount Number of particles to emit.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int psBurst(ParticleSystem *ps, int amount);

/**
 * @brief Sets the continuous emission rate in particles per second; pass 0 to stop streaming.
 *
 * @param ps   ParticleSystem to configure.
 * @param rate Emission rate in particles per second.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int psStream(ParticleSystem *ps, float rate);

// Update / Query

/**
 * @brief Advances the simulation by dt seconds, ageing particles, culling expired ones, and
 *        emitting streamed ones.
 *
 * @param ps ParticleSystem to update.
 * @param dt Delta time in seconds since the last update.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int psUpdate(ParticleSystem *ps, float dt);

/**
 * @brief Iterates over all active particles and invokes fn for each one.
 *
 * @param ps      ParticleSystem to iterate.
 * @param fn      Callback invoked for each active particle.
 * @param context Optional caller-supplied pointer passed to fn.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int psForEach(const ParticleSystem *ps, ParticleFn fn, void *context);

// Getters

// -- Position

/**
 * @brief Returns the X coordinate of the emission origin.
 *
 * @param ps ParticleSystem to query.
 *
 * @return X coordinate of the emission origin.
 */
float psGetX(const ParticleSystem *ps);

/**
 * @brief Returns the Y coordinate of the emission origin.
 *
 * @param ps ParticleSystem to query.
 *
 * @return Y coordinate of the emission origin.
 */
float psGetY(const ParticleSystem *ps);

// -- Count

/**
 * @brief Returns the number of currently active particles.
 *
 * @param ps ParticleSystem to query.
 *
 * @return Number of active particles, or a negative result code on failure.
 */
int psGetActive(const ParticleSystem *ps);

/**
 * @brief Returns the number of currently idle particle slots.
 *
 * @param ps ParticleSystem to query.
 *
 * @return Number of idle particle slots, or a negative result code on failure.
 */
int psGetIdle(const ParticleSystem *ps);

// Setters

// -- Position

/**
 * @brief Sets the emission origin to the given coordinates.
 *
 * @param ps ParticleSystem to configure.
 * @param x  New X coordinate of the origin.
 * @param y  New Y coordinate of the origin.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int psSetOrigin(ParticleSystem *ps, float x, float y);

/**
 * @brief Configures the inner and outer emission spread around the origin.
 *
 * @param ps     ParticleSystem to configure.
 * @param innerX Inner spread half-extent on X; particles are not emitted within this boundary.
 * @param innerY Inner spread half-extent on Y; particles are not emitted within this boundary.
 * @param outerX Outer spread half-extent on X; particles are not emitted beyond this boundary.
 * @param outerY Outer spread half-extent on Y; particles are not emitted beyond this boundary.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int psSetSpread(ParticleSystem *ps, float innerX, float innerY, float outerX, float outerY);

// -- Movement

/**
 * @brief Sets the random velocity range assigned to each newly emitted particle.
 *
 * @param ps   ParticleSystem to configure.
 * @param minX Minimum X velocity.
 * @param minY Minimum Y velocity.
 * @param maxX Maximum X velocity.
 * @param maxY Maximum Y velocity.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int psSetVelocity(ParticleSystem *ps, float minX, float minY, float maxX, float maxY);

/**
 * @brief Sets the random acceleration range assigned to each newly emitted particle.
 *
 * @param ps   ParticleSystem to configure.
 * @param minX Minimum X acceleration.
 * @param minY Minimum Y acceleration.
 * @param maxX Maximum X acceleration.
 * @param maxY Maximum Y acceleration.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int psSetAcceleration(ParticleSystem *ps, float minX, float minY, float maxX, float maxY);

/**
 * @brief Sets the random angular velocity range assigned to each newly emitted particle.
 *
 * @param ps  ParticleSystem to configure.
 * @param min Minimum angular velocity in radians per second.
 * @param max Maximum angular velocity in radians per second.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int psSetAngularVelocity(ParticleSystem *ps, float min, float max);

// -- Lifetime

/**
 * @brief Sets the random lifetime range assigned to each newly emitted particle.
 *
 * @param ps  ParticleSystem to configure.
 * @param min Minimum lifetime in seconds.
 * @param max Maximum lifetime in seconds.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int psSetLifetime(ParticleSystem *ps, float min, float max);

// -- Shape

/**
 * @brief Sets the geometric shape used to sample particle spawn positions.
 *
 * @param ps    ParticleSystem to configure.
 * @param shape Emission shape to use.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int psSetEmissionShape(ParticleSystem *ps, psEmissionShape shape);

// -- Influence

/**
 * @brief Registers a per-particle callback applied to every active particle each update. It's
 * called before the snapshot influence callback if both are set.
 *
 * @param ps      ParticleSystem to configure.
 * @param context Optional caller-supplied pointer passed to fn on each call.
 * @param fn      Influence callback; pass nullptr to remove the current influence.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int psSetInfluence(ParticleSystem *ps, void *context, InfluenceFn fn);

/**
 * @brief Registers a snapshot-based influence callback applied to every active particle each update.
 *
 * @param ps      ParticleSystem to configure.
 * @param context Optional caller-supplied pointer passed to fn on each call.
 * @param fn      Snapshot influence callback; pass nullptr to remove the current influence.
 *
 * @return 0 on success, or a negative result code on failure.
 */
int psSetSnapshotInfluence(ParticleSystem *ps, void *context, SnapshotInfluenceFn fn);
