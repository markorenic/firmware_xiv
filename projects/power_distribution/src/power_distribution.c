// CAN RX event mapper and GPIO testing main

// If pins 30 and 31 haven't been shorted to ground on the rear power distribution board yet,
// uncomment this and it'll force firmware to configure as rear.
//#define FORCE_REAR_POWER_DISTRIBUTION

#include "power_distribution_can_rx_event_mapper.h"
#include "power_distribution_can_rx_event_mapper_config.h"
#include "power_distribution_gpio.h"
#include "power_distribution_gpio_config.h"
#include "power_distribution_pin_defs.h"
#include "power_distribution_events.h"
#include "gpio.h"
#include "interrupt.h"
#include "i2c.h"
#include "log.h"

#define I2C_PORT I2C_PORT_2
#define I2C_SCL_PIN { GPIO_PORT_B, 10 }
#define I2C_SDA_PIN { GPIO_PORT_B, 11 }

#define CAN_TX_PIN { GPIO_PORT_A, 12 }
#define CAN_RX_PIN { GPIO_PORT_A, 11 }

#define CAN_DEVICE_ID 0x5

static CanStorage s_can_storage;

static bool s_is_front_power_distribution;

static bool prv_detect_is_front_power_distribution(void) {
#ifdef FORCE_REAR_POWER_DISTRIBUTION
  return false;
#else
  // initialize pin 30 (PC13) as pull-up, it's shorted on rear
  GpioAddress board_test_pin = { .port = GPIO_PORT_C, .pin = 13 };
  GpioSettings board_test_settings = {
    .direction = GPIO_DIR_IN,
    .resistor = GPIO_RES_PULLUP,
    .alt_function = GPIO_ALTFN_NONE,
  };
  gpio_init_pin(&board_test_pin, &board_test_settings);
  
  // we're on front if it's high and rear if it's low
  GpioState state = GPIO_STATE_HIGH; // default to front for x86 since we can force rear
  gpio_get_state(&board_test_pin, &state);
  
  return state == GPIO_STATE_HIGH;
#endif
}

int main(void) {
  // initialize everything
  event_queue_init();
  gpio_init();
  interrupt_init();
  soft_timer_init();
  
  I2CSettings i2c_settings = {
    .speed = I2C_SPEED_FAST,
    .scl = I2C_SCL_PIN,
    .sda = I2C_SDA_PIN,
  };
  i2c_init(I2C_PORT, &i2c_settings);
  
  pca9539r_gpio_init(I2C_PORT, POWER_DISTRIBUTION_I2C_ADDRESS_0);
  pca9539r_gpio_init(I2C_PORT, POWER_DISTRIBUTION_I2C_ADDRESS_1);
  
  CanSettings can_settings = {
    .device_id = CAN_DEVICE_ID,
    .loopback = true,
    .bitrate = CAN_HW_BITRATE_500KBPS,
    .rx_event = POWER_DISTRIBUTION_CAN_EVENT_RX,
    .tx_event = POWER_DISTRIBUTION_CAN_EVENT_TX,
    .fault_event = POWER_DISTRIBUTION_CAN_EVENT_FAULT,
    .tx = CAN_TX_PIN,
    .rx = CAN_RX_PIN,
  };
  can_init(&s_can_storage, &can_settings);
  
  // test if it's front or rear power distribution
  s_is_front_power_distribution = prv_detect_is_front_power_distribution();
  
  // initialize can rx and gpio
  power_distribution_can_rx_event_mapper_init(s_is_front_power_distribution
    ? FRONT_POWER_DISTRIBUTION_CAN_RX_CONFIG : REAR_POWER_DISTRIBUTION_CAN_RX_CONFIG);
  power_distribution_gpio_init(s_is_front_power_distribution
    ? FRONT_POWER_DISTRIBUTION_GPIO_CONFIG : REAR_POWER_DISTRIBUTION_GPIO_CONFIG);
  
  // process events
  Event e = { 0 };
  while (true) {
    while (event_process(&e) != STATUS_CODE_OK) {
      can_process_event(&e);
      power_distribution_gpio_process_event(&e);
    }
  }
  
  return 0;
}