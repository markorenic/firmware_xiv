#include "bts_7200_current_sense.h"
#include "delay.h"
#include "interrupt.h"
#include "log.h"
#include "test_helpers.h"
#include "unity.h"

static volatile uint16_t times_callback_called = 0;
static void *received_context;

static void prv_callback_increment(uint16_t reading0, uint16_t reading1, void *context) {
  times_callback_called++;
  received_context = context;
}

void setup_test(void) {
  gpio_init();
  interrupt_init();
  soft_timer_init();
  adc_init(ADC_MODE_SINGLE);
  times_callback_called = 0;
}
void teardown_test(void) {}

// Comprehensive happy-path test.
void test_bts_7200_current_sense_timer_works(void) {
  // these don't matter (adc isn't reading anything) but can't be null
  GpioAddress test_select_pin = { .port = GPIO_PORT_A, .pin = 0 };
  GpioAddress test_sense_pin = { .port = GPIO_PORT_A, .pin = 0 };
  uint32_t interval_us = 500;  // 0.5 ms
  Bts7200Settings settings = {
    .select_pin = &test_select_pin,
    .sense_pin = &test_sense_pin,
    .interval_us = interval_us,
    .callback = &prv_callback_increment,
  };
  Bts7200Storage storage = { 0 };

  TEST_ASSERT_OK(bts_7200_init(&storage, &settings));

  // make sure we don't start anything in init
  TEST_ASSERT_EQUAL(times_callback_called, 0);
  delay_us(2 * interval_us);
  TEST_ASSERT_EQUAL(times_callback_called, 0);

  TEST_ASSERT_OK(bts_7200_start(&storage));

  // we call the callback and get good values before setting the timer
  TEST_ASSERT_EQUAL(times_callback_called, 1);

  // wait in a busy loop for the callback to be called
  while (times_callback_called == 1) {
  }

  TEST_ASSERT_EQUAL(times_callback_called, 2);

  TEST_ASSERT_EQUAL(true, bts_7200_stop(&storage));

  // make sure that stop actually stops it
  delay_us(2 * interval_us);
  TEST_ASSERT_EQUAL(times_callback_called, 2);
}

// Test that we can init, start, stop, and start again and it works.
// Essentially the previous test done twice.
void test_bts_7200_current_sense_restart(void) {
  // these don't matter (adc isn't reading anything) but can't be null
  GpioAddress test_select_pin = { .port = GPIO_PORT_A, .pin = 0 };
  GpioAddress test_sense_pin = { .port = GPIO_PORT_A, .pin = 0 };
  uint32_t interval_us = 500;  // 0.5 ms
  Bts7200Settings settings = {
    .select_pin = &test_select_pin,
    .sense_pin = &test_sense_pin,
    .interval_us = interval_us,
    .callback = &prv_callback_increment,
  };
  Bts7200Storage storage = { 0 };

  TEST_ASSERT_OK(bts_7200_init(&storage, &settings));
  TEST_ASSERT_OK(bts_7200_start(&storage));

  // we call the callback and get good values before setting the timer
  TEST_ASSERT_EQUAL(times_callback_called, 1);

  // wait in a busy loop for the callback to be called
  while (times_callback_called == 1) {
  }

  TEST_ASSERT_EQUAL(times_callback_called, 2);

  TEST_ASSERT_EQUAL(true, bts_7200_stop(&storage));

  // make sure it's stopped
  delay_us(2 * interval_us);
  TEST_ASSERT_EQUAL(times_callback_called, 2);

  // start again
  TEST_ASSERT_OK(bts_7200_start(&storage));

  // we call the callback and get good values before setting the timer
  TEST_ASSERT_EQUAL(times_callback_called, 3);

  // wait in a busy loop for the callback to be called
  while (times_callback_called == 3) {
  }

  TEST_ASSERT_EQUAL(times_callback_called, 4);

  TEST_ASSERT_EQUAL(true, bts_7200_stop(&storage));

  // make sure it's stopped
  delay_us(2 * interval_us);
  TEST_ASSERT_EQUAL(times_callback_called, 4);
}

// Test init failure when the settings are invalid.
void test_bts_7200_current_sense_init_invalid_settings(void) {
  // start with invalid select pin
  GpioAddress select_pin = { .port = NUM_GPIO_PORTS, .pin = 0 };  // invalid
  GpioAddress sense_pin = { .port = 0, .pin = 0 };                // valid
  Bts7200Settings settings = {
    .select_pin = &select_pin,
    .sense_pin = &sense_pin,
    .interval_us = 500,  // 0.5 ms
    .callback = &prv_callback_increment,
  };
  Bts7200Storage storage = { 0 };

  TEST_ASSERT_NOT_OK(bts_7200_init(&storage, &settings));

  // invalid sense pin
  select_pin.port = 0;
  sense_pin.port = NUM_GPIO_PORTS;
  TEST_ASSERT_NOT_OK(bts_7200_init(&storage, &settings));
}

// Test that having a NULL callback works
void test_bts_7200_current_sense_null_callback(void) {
  // these don't matter (adc isn't reading anything) but can't be null
  GpioAddress test_select_pin = { .port = GPIO_PORT_A, .pin = 0 };
  GpioAddress test_sense_pin = { .port = GPIO_PORT_A, .pin = 0 };
  uint32_t interval_us = 500;  // 0.5 ms
  Bts7200Settings settings = {
    .select_pin = &test_select_pin,
    .sense_pin = &test_sense_pin,
    .interval_us = interval_us,
    .callback = NULL,
  };
  Bts7200Storage storage = { 0 };

  TEST_ASSERT_OK(bts_7200_init(&storage, &settings));
  TEST_ASSERT_OK(bts_7200_start(&storage));
  TEST_ASSERT_EQUAL(true, bts_7200_stop(&storage));
}

// Test that bts_7200_get_measurement returns ok.
void test_bts_7200_current_sense_get_measurement_valid(void) {
  // these don't matter (adc isn't reading anything) but can't be null
  GpioAddress test_select_pin = { .port = GPIO_PORT_A, .pin = 0 };
  GpioAddress test_sense_pin = { .port = GPIO_PORT_A, .pin = 0 };
  uint32_t interval_us = 500;  // 0.5 ms
  Bts7200Settings settings = {
    .select_pin = &test_select_pin,
    .sense_pin = &test_sense_pin,
    .interval_us = interval_us,
    .callback = &prv_callback_increment,
  };
  Bts7200Storage storage = { 0 };

  TEST_ASSERT_OK(bts_7200_init(&storage, &settings));

  uint16_t reading0 = 0, reading1 = 0;
  TEST_ASSERT_OK(bts_7200_get_measurement(&storage, &reading0, &reading1));
  LOG_DEBUG("Readings: %d, %d\r\n", reading0, reading1);
}

// Test that bts_7200_stop returns true only when it stops a timer
void test_bts_7200_current_sense_stop_return_behaviour(void) {
  // these don't matter (adc isn't reading anything) but can't be null
  GpioAddress test_select_pin = { .port = GPIO_PORT_A, .pin = 0 };
  GpioAddress test_sense_pin = { .port = GPIO_PORT_A, .pin = 0 };
  uint32_t interval_us = 500;  // 0.5 ms
  Bts7200Settings settings = {
    .select_pin = &test_select_pin,
    .sense_pin = &test_sense_pin,
    .interval_us = interval_us,
    .callback = &prv_callback_increment,
  };
  Bts7200Storage storage = { 0 };

  TEST_ASSERT_OK(bts_7200_init(&storage, &settings));
  TEST_ASSERT_EQUAL(false, bts_7200_stop(&storage));
  TEST_ASSERT_OK(bts_7200_start(&storage));
  TEST_ASSERT_EQUAL(true, bts_7200_stop(&storage));
  TEST_ASSERT_EQUAL(false, bts_7200_stop(&storage));
}

// Test that the context is actually passed to the function
void test_bts_7200_current_sense_context_passed(void) {
  // these don't matter (adc isn't reading anything) but can't be null
  GpioAddress test_select_pin = { .port = GPIO_PORT_A, .pin = 0 };
  GpioAddress test_sense_pin = { .port = GPIO_PORT_A, .pin = 0 };
  uint32_t interval_us = 500;            // 0.5 ms
  void *context_pointer = &interval_us;  // arbitrary pointer
  Bts7200Settings settings = {
    .select_pin = &test_select_pin,
    .sense_pin = &test_sense_pin,
    .interval_us = interval_us,
    .callback = &prv_callback_increment,
    .callback_context = context_pointer,
  };
  Bts7200Storage storage = { 0 };

  TEST_ASSERT_OK(bts_7200_init(&storage, &settings));
  TEST_ASSERT_OK(bts_7200_start(&storage));
  TEST_ASSERT_EQUAL(true, bts_7200_stop(&storage));
  TEST_ASSERT_EQUAL(received_context, context_pointer);
}
