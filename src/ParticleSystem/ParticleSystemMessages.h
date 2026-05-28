#pragma once

// Module Name —————————————————————————————————————————————————————————————————————————————————————

#define ORI "ParticleSystem"

// Context Messages ————————————————————————————————————————————————————————————————————————————————

static constexpr char MSG_INVALID_SPREAD_RANGE[] = "Outer spread must be greater than inner spread";
static constexpr char MSG_INVALID_LIFETIME_RANGE[] = "min lifetime must not exceed max lifetime";
static constexpr char MSG_INVALID_VELOCITY_RANGE[] = "min velocity must not exceed max velocity";
static constexpr char MSG_INVALID_ACCELERATION_RANGE[] =
    "min acceleration must not exceed max acceleration";
