#include "relay_fsm.h"

#include <stdbool.h>

#include "event_queue.h"
#include "fault_handler.h"
#include "fsm.h"
#include "log.h"
#include "solar_config.h"
#include "solar_events.h"
#include "status.h"

static void prv_relay_err_cb(void *context) {
  LOG_DEBUG("RELAY_ERROR CALLBACK\n");
  fault_handler_raise_fault(EE_SOLAR_FAULT_DRV120, 0);
}

FSM_DECLARE_STATE(state_relay_open);
FSM_DECLARE_STATE(state_relay_closed);

FSM_STATE_TRANSITION(state_relay_open) {
  FSM_ADD_TRANSITION(SOLAR_RELAY_EVENT_CLOSE, state_relay_closed);
}

FSM_STATE_TRANSITION(state_relay_closed) {
  FSM_ADD_TRANSITION(SOLAR_RELAY_EVENT_OPEN, state_relay_open);
}

static void prv_open_relay(Fsm *fsm, const Event *e, void *context) {
  LOG_DEBUG("Opening relay\n");
  drv120_relay_open();
}

static void prv_close_relay(Fsm *fsm, const Event *e, void *context) {
  LOG_DEBUG("Closing relay\n");
  drv120_relay_close();
}

StatusCode relay_fsm_init(RelayFsmStorage *storage) {
  if (storage == NULL) {
    return status_code(STATUS_CODE_INVALID_ARGS);
  }
  // Init drv120
  Drv120RelaySettings drv120_settings = {
    .enable_pin = config_get_drv120_enable_pin(),
    .status_pin = config_get_drv120_status_pin(),
    .error_handler = prv_relay_err_cb,
    .context = NULL,
  };
  status_ok_or_return(drv120_relay_init(&drv120_settings));

  fsm_init(&storage->fsm, "Relay FSM", &state_relay_closed, storage);
  fsm_state_init(state_relay_open, prv_open_relay);
  fsm_state_init(state_relay_closed, prv_close_relay);

  // begin with the relay closed
  drv120_relay_close();

  return STATUS_CODE_OK;
}

bool relay_fsm_process_event(RelayFsmStorage *storage, const Event *e) {
  if (storage == NULL || e == NULL) {
    return false;
  }
  return fsm_process_event(&storage->fsm, e);
}

StatusCode relay_fsm_close(void) {
  return event_raise_priority(RELAY_EVENT_PRIORITY, SOLAR_RELAY_EVENT_CLOSE, 0);
}

StatusCode relay_fsm_open(void) {
  return event_raise_priority(RELAY_EVENT_PRIORITY, SOLAR_RELAY_EVENT_OPEN, 0);
}
