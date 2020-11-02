#pragma once
// Measures values from power selection to send over CAN and handles faults.

// Requires gpio, interrupts, CAN, ADC (in ADC_MODE_SINGLE), and soft timers to be initialized.
// TODO: Improve fault handling, disable LTC through driver (probably after powertrain
// mockup)

#include "adc.h"
#include "can_transmit.h"
#include "gpio.h"
#include "gpio_it.h"
#include "soft_timer.h"
#include "status.h"

#include "power_select_defs.h"

#define POWER_SELECT_MEASUREMENT_INTERVAL_MS 1000
#define POWER_SELECT_MEASUREMENT_INTERVAL_US (POWER_SELECT_MEASUREMENT_INTERVAL_MS * 1000)

typedef enum {
  POWER_SELECT_AUX = 0,
  POWER_SELECT_DCDC,
  POWER_SELECT_PWR_SUP,
  NUM_POWER_SELECT_MEASUREMENTS,
} PowerSelectMeasurement;

#define NUM_POWER_SELECT_VOLTAGE_MEASUREMENTS (NUM_POWER_SELECT_MEASUREMENTS)
#define NUM_POWER_SELECT_CURRENT_MEASUREMENTS (NUM_POWER_SELECT_MEASUREMENTS)
#define NUM_POWER_SELECT_TEMP_MEASUREMENTS (NUM_POWER_SELECT_MEASUREMENTS - 1)

// Storage for previous measurements
typedef struct {
  uint16_t voltages[NUM_POWER_SELECT_VOLTAGE_MEASUREMENTS];
  uint16_t currents[NUM_POWER_SELECT_CURRENT_MEASUREMENTS];
  uint16_t temps[NUM_POWER_SELECT_TEMP_MEASUREMENTS];  // no power supply temp measurements
  uint16_t fault_bitset; // TODO: send this over CAN instead of the individual fault
} PowerSelectStorage;

// Initialize power selection
StatusCode power_select_init(void);

// Start periodically measuring sense values
StatusCode power_select_start(void);