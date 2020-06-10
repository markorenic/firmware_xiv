// A simple smoke test for ADS1015

// Periodically take reading from user selected channels and log the result
// Configurable items: wait time, I2C port, channels to be tested and conversion enable
#include "ads1015.h"
#include "gpio.h"
#include "i2c.h"
#include "interrupt.h"
#include "log.h"
#include "soft_timer.h"
#include "status.h"
#include "wait.h"

#define SMOKE_READING_WAIT_MS 1000  // Set wait time between each set of readings
#define SMOKE_ADS1015_ADDR ADS1015_ADDRESS_GND
#define SMOKE_ADS1015_I2C_PORT I2C_PORT_1  // I2C_PORT_2
#define NUM_TEST_CHANNELS 4                // Number of channels to be tested

static Ads1015Storage s_storage = { 0 };
static bool READ_CONVERTED = false;

// To test a channel, uncomment that channel in this array, has to match NUM_TEST_CHANNELS
const Ads1015Channel test_channels[NUM_TEST_CHANNELS] = { ADS1015_CHANNEL_0, ADS1015_CHANNEL_1,
                                                          ADS1015_CHANNEL_2, ADS1015_CHANNEL_3 };

static void prv_read_and_log(SoftTimerId timer_id, void *context) {
  Ads1015Storage *storage = context;
  int16_t reading = 0;
  StatusCode status = STATUS_CODE_OK;

  if (!READ_CONVERTED) {
    for (size_t i = 0; i < SIZEOF_ARRAY(test_channels) && status == STATUS_CODE_OK; i++) {
      status = ads1015_read_raw(storage, test_channels[i], &reading);
      if (status == STATUS_CODE_OK)
        LOG_DEBUG("Channel: %d; Raw value: %d\n", test_channels[i], reading);
      else
        LOG_DEBUG("Fail to take raw reading\n");
    }
  } else {
    for (size_t i = 0; i < SIZEOF_ARRAY(test_channels) && status == STATUS_CODE_OK; i++) {
      status = ads1015_read_converted(storage, test_channels[i], &reading);
      if (status == STATUS_CODE_OK)
        LOG_DEBUG("Channel: %d; Converted value: %d mVolt\n", test_channels[i], reading);
      else
        LOG_DEBUG("Fail to take converted reading\n");
    }
  }
  soft_timer_start_millis(SMOKE_READING_WAIT_MS, prv_read_and_log, &s_storage, NULL);
}

int main(void) {
  gpio_init();
  interrupt_init();
  soft_timer_init();
  I2CSettings i2c_settings = {
    .speed = I2C_SPEED_FAST,                    //
    .scl = { .port = GPIO_PORT_B, .pin = 10 },  //
    .sda = { .port = GPIO_PORT_B, .pin = 11 },  //
  };
  i2c_init(SMOKE_ADS1015_I2C_PORT, &i2c_settings);
  GpioAddress ready_pin = {
    .port = GPIO_PORT_B,  //
    .pin = 2,             //
  };
  ads1015_init(&s_storage, SMOKE_ADS1015_I2C_PORT, SMOKE_ADS1015_ADDR, &ready_pin);

  soft_timer_start_millis(SMOKE_READING_WAIT_MS, prv_read_and_log, &s_storage, NULL);
  while (true) {
    wait();
  }

  return 0;
}
