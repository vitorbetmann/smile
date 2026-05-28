#pragma once

// Includes ————————————————————————————————————————————————————————————————————————————————————————

#include "ParticleSystem.h"

// Data Types ——————————————————————————————————————————————————————————————————————————————————————

/** @brief Describes the shape and spread bounds of the particle emission area. */
typedef struct
{
    psEmissionShape shape;        /**< Geometric shape of the emission area. */
    float           innerSpreadX; /**< Inner spread half-extent on X; particles are not emitted within this zone. */
    float           innerSpreadY; /**< Inner spread half-extent on Y; particles are not emitted within this zone. */
    float           outerSpreadX; /**< Outer spread half-extent on X; particles are not emitted beyond this zone. */
    float           outerSpreadY; /**< Outer spread half-extent on Y; particles are not emitted beyond this zone. */
} psInternalEmissionArea;

/** @brief Internal state of a ParticleSystem instance. */
typedef struct psInternalParticleSystem
{
    int maxParticles;    /**< Maximum number of particles the system can hold. */
    int activeParticles; /**< Number of currently active particles. */

    float originX; /**< X coordinate of the emission origin. */
    float originY; /**< Y coordinate of the emission origin. */

    float minVelocityX; /**< Minimum X velocity for new particles. */
    float minVelocityY; /**< Minimum Y velocity for new particles. */
    float maxVelocityX; /**< Maximum X velocity for new particles. */
    float maxVelocityY; /**< Maximum Y velocity for new particles. */

    float minAccelerationX; /**< Minimum X acceleration for new particles. */
    float minAccelerationY; /**< Minimum Y acceleration for new particles. */
    float maxAccelerationX; /**< Maximum X acceleration for new particles. */
    float maxAccelerationY; /**< Maximum Y acceleration for new particles. */

    float minLifetime; /**< Minimum lifetime in seconds for new particles. */
    float maxLifetime; /**< Maximum lifetime in seconds for new particles. */

    float streamRate;        /**< Continuous emission rate in particles per second. */
    float streamAccumulator; /**< Fractional particle count accumulated since the last emit. */

    psInternalEmissionArea emissionArea; /**< Configured emission area shape and spread. */

    InfluenceFn  influenceFn;      /**< Per-particle influence callback, or nullptr if none. */
    void        *influenceContext; /**< Context pointer passed to influenceFn. */

    SnapshotInfluenceFn  snapshotFn;      /**< Snapshot influence callback, or nullptr if none. */
    void                *snapshotContext; /**< Context pointer passed to snapshotFn. */
    Particle            *snapshotBuffer;  /**< Buffer holding the pre-update particle snapshot. */

    Particle particles[]; /**< Flexible array of all particle slots (active and idle). */
} psInternalParticleSystem;

// Prototypes ——————————————————————————————————————————————————————————————————————————————————————

/**
 * @brief Samples a spawn position within the configured emission area around the given origin.
 *
 * @param area    Emission area configuration describing the shape and spread bounds.
 * @param originX X coordinate of the emission origin.
 * @param originY Y coordinate of the emission origin.
 * @param outX    Output pointer receiving the sampled X coordinate.
 * @param outY    Output pointer receiving the sampled Y coordinate.
 */
void psInternalSamplePosition(psInternalEmissionArea area, float originX, float originY,
                              float *outX, float *outY);
