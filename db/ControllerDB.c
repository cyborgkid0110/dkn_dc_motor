#include <stdint.h>
#include <stdbool.h>
#include "PinConfig.h"
#include "MotorConfig.h"
#include "Definitions.h"

///////////////////////////////////////////////////////////////////////////////////
//                               Global variables                                //
///////////////////////////////////////////////////////////////////////////////////

// Controller status
static uint8_t controller_status = CTL_STOP;
// Communication mode
static uint8_t com_mode = COM_MODE_DATA_TRANSFER_ONLY;
// Motor Configuration Database
MotorConfig_t motor_cfg_db[] = CONTROLLER_DEFAULT_CONTROL;
// This indicates which motor is currently controlled
bool motor_enabled[] = MOTOR_ENABLED_DEFAULT;
// Pin Configuration
IndicatorPinCfg_t indicator_pin_db = CONTROLLER_INDICATOR_DEFAULT_PIN;
MotorPinCfg_t motor_pin_db[] = CONTROLLER_DEFAULT_PIN;

///////////////////////////////////////////////////////////////////////////////////
//                               Other functions                                 //
///////////////////////////////////////////////////////////////////////////////////

uint8_t getControllerStatus() {
    return controller_status;
}

bool setControllerStatus(uint8_t controller_state) {

}

uint8_t getCOMMode() {
    return com_mode;
}

uint8_t setCOMMode(uint8_t mode) {
    com_mode = mode;
}

bool getPreviousMotor(uint8_t* motor_opt) {
    for (int i = *motor_opt - 1; i >= 0; i--) {
        if (motor_enabled[i] == ENABLED) {
            *motor_opt = i;
            return true;
        }
    }
    return false;
}

bool getNextMotor(uint8_t* motor_opt) {
    for (int i = *motor_opt + 1; i < NUM_OF_MOTOR; i++) {
        if (motor_enabled[i] == ENABLED) {
            *motor_opt = i;
            return true;
        }
    }
    return false;
}

bool getLastMotor(uint8_t* motor_opt) {
    for (int i = NUM_OF_MOTOR - 1; i >= 0; i--) {
        if (motor_enabled[i] == ENABLED) {
            *motor_opt = i;
            return true;
        }
    }
    return false;
}

bool getTopMotor(uint8_t* motor_opt) {
    for (int i = 0; i < NUM_OF_MOTOR; i--) {
        if (motor_enabled[i] == ENABLED) {
            *motor_opt = i;
            return true;
        }
    }
    return false;
}

// Get motor configuration
MotorConfig_t getMotorConfig(uint8_t motor) {
    return motor_cfg_db[motor];
}

void updateControlMode(uint8_t mode) {
    switch (mode) {
        case ONE_BRUSHED_MODE:
            motor_enabled[BRUSHED_1_CFG] = ENABLED;
            motor_enabled[BRUSHED_2_CFG] = motor_enabled[BRUSHLESS_CFG] = motor_enabled[STEPPER_CFG] = DISABLED; 
            break;
        case TWO_BRUSHED_MODE:
            motor_enabled[BRUSHED_1_CFG] = ENABLED;
            motor_enabled[BRUSHED_2_CFG] = ENABLED;
            break;
        case BRUSHLESS_MODE:
            motor_enabled[BRUSHLESS_CFG] = ENABLED;
            motor_enabled[BRUSHED_2_CFG] = motor_enabled[BRUSHED_1_CFG] = motor_enabled[STEPPER_CFG] = DISABLED; 
            break;
        case STEPPER_MODE:
            motor_enabled[STEPPER_CFG] = ENABLED;
            motor_enabled[BRUSHED_2_CFG] = motor_enabled[BRUSHLESS_CFG] = motor_enabled[BRUSHED_1_CFG] = DISABLED; 
            break;
    }
}