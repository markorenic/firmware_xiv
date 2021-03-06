#include "publish_data_config.h"
#include "can_transmit.h"

static StatusCode prv_publish_front_current_measurement(PowerDistributionCurrent current_id,
                                                        uint16_t current_data) {
  return CAN_TRANSMIT_FRONT_CURRENT_MEASUREMENT((uint16_t)current_id, current_data);
}

static StatusCode prv_publish_rear_current_measurement(PowerDistributionCurrent current_id,
                                                       uint16_t current_data) {
  return CAN_TRANSMIT_REAR_CURRENT_MEASUREMENT((uint16_t)current_id, current_data);
}

const PowerDistributionPublishConfig FRONT_POWER_DISTRIBUTION_PUBLISH_DATA_CONFIG = {
  .transmitter = prv_publish_front_current_measurement,
  .currents_to_publish =
      (PowerDistributionCurrent[]){
          FRONT_POWER_DISTRIBUTION_CURRENT_CENTRE_CONSOLE,
          FRONT_POWER_DISTRIBUTION_CURRENT_PEDAL,
          FRONT_POWER_DISTRIBUTION_CURRENT_STEERING,
          FRONT_POWER_DISTRIBUTION_CURRENT_LEFT_CAMERA,
          FRONT_POWER_DISTRIBUTION_CURRENT_RIGHT_CAMERA,
          FRONT_POWER_DISTRIBUTION_CURRENT_MAIN_DISPLAY,
          FRONT_POWER_DISTRIBUTION_CURRENT_REAR_DISPLAY,
          FRONT_POWER_DISTRIBUTION_CURRENT_DRIVER_DISPLAY,
          FRONT_POWER_DISTRIBUTION_CURRENT_LEFT_DRIVER_DISPLAY,
          FRONT_POWER_DISTRIBUTION_CURRENT_RIGHT_DRIVER_DISPLAY,
          FRONT_POWER_DISTRIBUTION_CURRENT_MAIN_PI,
          FRONT_POWER_DISTRIBUTION_CURRENT_REAR_PI,
          FRONT_POWER_DISTRIBUTION_CURRENT_LEFT_FRONT_TURN_LIGHT,
          FRONT_POWER_DISTRIBUTION_CURRENT_RIGHT_FRONT_TURN_LIGHT,
          FRONT_POWER_DISTRIBUTION_CURRENT_DAYTIME_RUNNING_LIGHTS,
          FRONT_POWER_DISTRIBUTION_CURRENT_PARKING_BRAKE,
          FRONT_POWER_DISTRIBUTION_CURRENT_SPEAKER,
          FRONT_POWER_DISTRIBUTION_CURRENT_HORN,
          FRONT_POWER_DISTRIBUTION_CURRENT_5V_SPARE_1,
          FRONT_POWER_DISTRIBUTION_CURRENT_5V_SPARE_2,
          FRONT_POWER_DISTRIBUTION_CURRENT_SPARE_1,
          FRONT_POWER_DISTRIBUTION_CURRENT_SPARE_2,
      },
  .num_currents_to_publish = 22,
};

const PowerDistributionPublishConfig REAR_POWER_DISTRIBUTION_PUBLISH_DATA_CONFIG = {
  .transmitter = prv_publish_rear_current_measurement,
  .currents_to_publish =
      (PowerDistributionCurrent[]){
          REAR_POWER_DISTRIBUTION_CURRENT_BMS_CARRIER,
          REAR_POWER_DISTRIBUTION_CURRENT_MCI,
          REAR_POWER_DISTRIBUTION_CURRENT_CHARGER,
          REAR_POWER_DISTRIBUTION_CURRENT_SOLAR_SENSE,
          REAR_POWER_DISTRIBUTION_CURRENT_TELEMETRY,
          REAR_POWER_DISTRIBUTION_CURRENT_REAR_CAMERA,
          REAR_POWER_DISTRIBUTION_CURRENT_LEFT_REAR_TURN_LIGHT,
          REAR_POWER_DISTRIBUTION_CURRENT_RIGHT_REAR_TURN_LIGHT,
          REAR_POWER_DISTRIBUTION_CURRENT_LEFT_BRAKE_LIGHT,
          REAR_POWER_DISTRIBUTION_CURRENT_CENTRE_BRAKE_LIGHT,
          REAR_POWER_DISTRIBUTION_CURRENT_RIGHT_BRAKE_LIGHT,
          REAR_POWER_DISTRIBUTION_CURRENT_STROBE,
          REAR_POWER_DISTRIBUTION_CURRENT_SPARE_1,
          REAR_POWER_DISTRIBUTION_CURRENT_SPARE_2,
          REAR_POWER_DISTRIBUTION_CURRENT_SPARE_3,
          REAR_POWER_DISTRIBUTION_CURRENT_SPARE_4,
          REAR_POWER_DISTRIBUTION_CURRENT_SPARE_5,
          REAR_POWER_DISTRIBUTION_CURRENT_SPARE_6,
          REAR_POWER_DISTRIBUTION_CURRENT_SPARE_7,
          REAR_POWER_DISTRIBUTION_CURRENT_SPARE_8,
          REAR_POWER_DISTRIBUTION_CURRENT_SPARE_9,
          REAR_POWER_DISTRIBUTION_CURRENT_SPARE_10,
      },
  .num_currents_to_publish = 22,
};
