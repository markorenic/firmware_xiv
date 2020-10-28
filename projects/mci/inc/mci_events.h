#pragma once

typedef enum {
  MCI_CAN_EVENT_RX = 0,
  MCI_CAN_EVENT_TX,
  MCI_CAN_EVENT_FAULT,
  NUM_MCI_CAN_EVENTS
} MciCanEvent;

typedef enum {
  MCI_DRIVE_FSM_EVENT_OFF = NUM_MCI_CAN_EVENTS + 1,
  MCI_DRIVE_FSM_EVENT_DRIVE,
  MCI_DRIVE_FSM_EVENT_REVERSE,
  MCI_DRIVE_FSM_EVENT_TOGGLE_CRUISE,
  NUM_MCI_DRIVE_FSM_EVENTS
} MciDriveFsmEvent;

typedef enum {
  MCI_PEDAL_RX_EVENT_RX = NUM_MCI_DRIVE_FSM_EVENTS + 1,
  MCI_PEDAL_RX_EVENT_TIMEOUT,
  NUM_MCI_PEDAL_RX_EVENTS,
} MciPedalRxEvent;
