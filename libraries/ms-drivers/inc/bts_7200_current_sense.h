#pragma once

// Reads current values from 2 ADC pins on the BTS 7200 switched between with a selection pin.
// Requires GPIO, interrupts, soft timers, and ADC to be initialized in ADC_MODE_SINGLE.

#include "adc.h"
#include "gpio.h"
#include "soft_timer.h"

typedef void (*Bts7200DataCallback)(uint16_t reading_out_0, uint16_t reading_out_1, void *context);

typedef struct {
  GpioAddress *select_pin;
  GpioAddress *sense_pin;
  uint32_t interval_us;
  Bts7200DataCallback callback;  // set to NULL for no callback
  void *callback_context;
} Bts7200Settings;

typedef struct Bts7200Storage {
  uint16_t reading_out_0;
  uint16_t reading_out_1;
  GpioAddress *select_pin;
  GpioAddress *sense_pin;
  uint32_t interval_us;
  SoftTimerId timer_id;
  Bts7200DataCallback callback;
  void *callback_context;
} Bts7200Storage;

// Initialize the BTS7200 with the given settings.
StatusCode bts_7200_init(Bts7200Storage *storage, Bts7200Settings *settings);

// Read the latest measurements. This does not get measurements from the storage but instead
// reads them from the BTS7200 itself.
StatusCode bts_7200_get_measurement(Bts7200Storage *storage, uint16_t *meas0, uint16_t *meas1);

// Set up a soft timer which periodically updates the storage with the latest measurements.
StatusCode bts_7200_start(Bts7200Storage *storage);

// Stop the timer associated with the storage and return whether it was successful.
bool bts_7200_stop(Bts7200Storage *storage);
