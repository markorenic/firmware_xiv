#include "drv120_relay.h"

// Storage for EN pin on DRV120 to open/close relay
static GpioAddress s_relay_pin;

StatusCode drv120_relay_init(Drv120RelaySettings *settings) {
  s_relay_pin = *settings->pin;

  // Pin low on initialization
  GpioSettings drv120_pin_settings = {
    .direction = GPIO_DIR_OUT,
    .state = GPIO_STATE_LOW,
    .resistor = GPIO_RES_NONE,
    .alt_function = GPIO_ALTFN_NONE,
  };

  if (settings->status != NULL) {
    InterruptSettings it_settings = {
      .type = INTERRUPT_TYPE_INTERRUPT,
      .priority = INTERRUPT_PRIORITY_NORMAL,
    };
    status_ok_or_return(gpio_it_register_interrupt(settings->status, &it_settings,
                                                   INTERRUPT_EDGE_FALLING, settings->handler,
                                                   settings->context));
  }
  return gpio_init_pin(&s_relay_pin, &drv120_pin_settings);
}

StatusCode drv120_relay_close(void) {
  return gpio_set_state(&s_relay_pin, GPIO_STATE_HIGH);
}

StatusCode drv120_relay_open(void) {
  return gpio_set_state(&s_relay_pin, GPIO_STATE_LOW);
}

StatusCode drv120_relay_get_is_closed(bool *closed) {
  GpioState state;
  status_ok_or_return(gpio_get_state(&s_relay_pin, &state));
  *closed = (state == GPIO_STATE_HIGH);
  return STATUS_CODE_OK;
}
