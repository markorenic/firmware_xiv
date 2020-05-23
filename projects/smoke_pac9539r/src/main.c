/* Smoke test for pca9539r gpio expander. i2c and other settings based off of test_pca9539r_gpio_expander.c 
    Program sets all pins as inputs, and low state, then outputs with high state, and checks configuration registers against expected results
    Then toggles gpio state on all pins every second */

#include "i2c.h"
#include "gpio.h"
#include "log.h"
#include "pca9539r_gpio_expander.h"
#include "test_helpers.h"
#include "soft_timer.h"
#include "wait.h"
#include "interrupt.h"

//this used to set test mode. If set to 1, will toggle gpio states for outputs
#define TEST_MODE_OUTPUT 1


//these i2c settings may need adjustment - taken from test_pca9539r_gpio_expander.c
#define TEST_I2C_PORT I2C_PORT_2
#define TEST_CONFIG_PIN_I2C_SCL \
  { GPIO_PORT_B, 10 }
#define TEST_CONFIG_PIN_I2C_SDA \
  { GPIO_PORT_B, 11 }
#define VALID_I2C_ADDRESS 0x74



void setup_test(void) {
  I2CSettings i2c_settings = {
    .speed = I2C_SPEED_FAST,         //
    .sda = TEST_CONFIG_PIN_I2C_SDA,  //
    .scl = TEST_CONFIG_PIN_I2C_SCL,  //     
  };
  i2c_init(TEST_I2C_PORT, &i2c_settings);
  pca9539r_gpio_init(TEST_I2C_PORT, VALID_I2C_ADDRESS);
}

//initialize all pins to in/out - must be called after pca9539r_gpio_init
StatusCode pca9539r_init_all_pins(Pca9539rGpioDirection direction) {
    Pca9539rGpioSettings gpio_settings = { .direction = direction, };
    Pca9539rGpioAddress address = { .i2c_address = VALID_I2C_ADDRESS, };

    if(direction == PCA9539R_GPIO_DIR_OUT) {
        gpio_settings.state = PCA9539R_GPIO_STATE_HIGH;
    } else {
        gpio_settings.state = PCA9539R_GPIO_STATE_LOW;
    } 
    for(Pca9539rPinAddress pin = PCA9539R_PIN_IO0_0; pin < NUM_PCA9539R_GPIO_PINS; pin++) {
        address.pin = pin;
        status_ok_or_return(pca9539r_gpio_init_pin(&address, &gpio_settings));
    } 
    return STATUS_CODE_OK;
}

//reads back configuration registers and compares them to input state
StatusCode pca9539r_check_all_pin_states(Pca9539rGpioState state) {
    Pca9539rGpioState in_state;
    Pca9539rGpioAddress address = { .i2c_address = VALID_I2C_ADDRESS};
    for(Pca9539rPinAddress pin = PCA9539R_PIN_IO0_0; pin < NUM_PCA9539R_GPIO_PINS; pin++) {
        address.pin = pin;
        status_ok_or_return(pca9539r_gpio_get_state(&address, &in_state));
        if(state == in_state) {
            LOG_DEBUG("State for pin %d_%d set and read correctly\n", address.pin / 8, address.pin % 8);
        } 
        else {
            LOG_DEBUG("State for pin %d_%d incorrect\n", address.pin / 8, address.pin % 8);
            return STATUS_CODE_INTERNAL_ERROR;
        }
    }
    return STATUS_CODE_OK;
}


//used to test proper gpio toggling. A multi-meter will be needed
static void prv_soft_timer_callback_output(SoftTimerId timer_id, void *context) {
    Pca9539rGpioState *state = (Pca9539rGpioState*)context; 
    Pca9539rGpioState get_state;
    Pca9539rGpioAddress address = { .i2c_address = VALID_I2C_ADDRESS, };

    for(Pca9539rPinAddress pin = PCA9539R_PIN_IO0_0; pin < NUM_PCA9539R_GPIO_PINS; pin++) {
        address.pin = pin;
        pca9539r_gpio_toggle_state(&address);
        pca9539r_gpio_get_state(&address, &get_state);
        if(get_state == *state) {
            LOG_DEBUG("State for pin %d_%d set and read correctly\n", address.pin / 8, address.pin % 8);
        } 
        else {
            LOG_DEBUG("State for pin %d_%d incorrect\n", address.pin / 8, address.pin % 8);
        }
    }
    *state = (*state == PCA9539R_GPIO_STATE_HIGH ? PCA9539R_GPIO_STATE_LOW : PCA9539R_GPIO_STATE_HIGH);
    LOG_DEBUG("GPIO state = %d\n", *state);
    soft_timer_start_millis(1000, prv_soft_timer_callback_output, (void*)state, NULL);
}


//add input/output
//look at triggering interrupt on input

int main() {
    interrupt_init();
    soft_timer_init();
    setup_test();

    Pca9539rGpioState test_state;
    LOG_DEBUG("Testing GPIO initialization...\n");
    LOG_DEBUG("Initializing all pins in...\n");
    pca9539r_init_all_pins(PCA9539R_GPIO_DIR_IN);
    pca9539r_check_all_pin_states(PCA9539R_GPIO_STATE_LOW);
    LOG_DEBUG("Initializing all pins out...\n");
    pca9539r_init_all_pins(PCA9539R_GPIO_DIR_OUT);
    pca9539r_check_all_pin_states(PCA9539R_GPIO_STATE_HIGH);
    LOG_DEBUG("GPIO initialization complete. Now beginning toggling of GPIO states. First read will be listed as incorrect...\n");

    Pca9539rGpioState state;
    //Toggles gpio state on every pin every second, and compares expected values against registers. First read will be listed as incorrect and can be ignored
    soft_timer_start_millis(100, prv_soft_timer_callback_output, (void*)&state, NULL);

    while(true) {
       wait();
    }

    return 0;
}
