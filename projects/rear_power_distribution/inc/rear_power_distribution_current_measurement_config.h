#pragma once

// Standard hardware configurations for rear_power_distribution_current_measurement.

#include "rear_power_distribution_current_measurement.h"

// to-do: the ones on the BTS7040s
// note: this enum (currently, pre-bts7040) *must* correspond to the Bts7200 enum
typedef enum {
  REAR_POWER_DISTRIBUTION_CURRENT_LEFT_CAMERA = 0,
  REAR_POWER_DISTRIBUTION_CURRENT_RIGHT_CAMERA,
  REAR_POWER_DISTRIBUTION_CURRENT_DRIVER_DISPLAY,
  REAR_POWER_DISTRIBUTION_CURRENT_REAR_DISPLAY,
  REAR_POWER_DISTRIBUTION_CURRENT_LEFT_DRIVER_DISPLAY,
  REAR_POWER_DISTRIBUTION_CURRENT_RIGHT_DRIVER_DISPLAY,
  REAR_POWER_DISTRIBUTION_CURRENT_LEFT_FRONT_TURN_LIGHT,
  REAR_POWER_DISTRIBUTION_CURRENT_RIGHT_FRONT_TURN_LIGHT,
  REAR_POWER_DISTRIBUTION_CURRENT_MAIN_PI,
  REAR_POWER_DISTRIBUTION_CURRENT_REAR_PI,
  REAR_POWER_DISTRIBUTION_CURRENT_5V_SPARE_1,
  REAR_POWER_DISTRIBUTION_CURRENT_5V_SPARE_2,
  REAR_POWER_DISTRIBUTION_CURRENT_SPARE_2,
  REAR_POWER_DISTRIBUTION_CURRENT_CENTRE_CONSOLE,
  REAR_POWER_DISTRIBUTION_CURRENT_STEERING,
  REAR_POWER_DISTRIBUTION_CURRENT_PEDAL,
  NUM_REAR_POWER_DISTRIBUTION_CURRENTS,
} RearPowerDistributionCurrent;

// ... these are actually front power distribution
typedef enum {
  // first thing named is when DSEL is 0, then when DSEL is 1
  REAR_POWER_DISTRIBUTION_BTS7200_LEFT_RIGHT_CAMERA = 0,
  REAR_POWER_DISTRIBUTION_BTS7200_DRIVER_REAR_DISPLAY,
  REAR_POWER_DISTRIBUTION_BTS7200_LEFT_RIGHT_DRIVER_DISPLAY,
  REAR_POWER_DISTRIBUTION_BTS7200_LEFT_RIGHT_FRONT_TURN_LIGHT,
  REAR_POWER_DISTRIBUTION_BTS7200_MAIN_REAR_PI,
  REAR_POWER_DISTRIBUTION_BTS7200_5V_SPARE_1_2,
  REAR_POWER_DISTRIBUTION_BTS7200_SPARE_2_CENTRE_CONSOLE,
  REAR_POWER_DISTRIBUTION_BTS7200_STEERING_PEDAL,
  NUM_REAR_POWER_DISTRIBUTION_BTS7200_CHANNELS,
} RearPowerDistributionBts7200;

extern const RearPowerDistributionCurrentHardwareConfig FRONT_POWER_DISTRIBUTION_HW_CONFIG;
