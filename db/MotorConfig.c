#include <stdint.h>
#include <stdbool.h>
#include "PinConfig.c"
#include "db_config.h"

///////////////////////////////////////////////////////////////////////////////////
//                                  Definition                                   //
///////////////////////////////////////////////////////////////////////////////////

typedef struct motor_cfg {
    uint8_t motor_type;
    uint8_t control_purpose;
    double speed_value;
    double speed_set_point;
    double speed_threshold;
    double torque_value;
    double torque_set_point;
    double current_threshold;
    uint16_t angle_position;
    uint16_t angle_set_point;
    uint16_t angle_step;
    bool direction;
} MotorConfig_t;

///////////////////////////////////////////////////////////////////////////////////
//                               Global variables                                //
///////////////////////////////////////////////////////////////////////////////////

// Controller status
static uint8_t controller_status = CTL_STOP;

// Communication mode
static uint8_t com_mode = COM_MODE_DATA_TRANSFER_ONLY;

// Motor Configuration Database
MotorConfig_t motor_cfg_db[] = CONTROLLER_DEFAULT_CONTROL;

// This indicates which motor is controlled
bool motor_enabled[] = MOTOR_ENABLED_DEFAULT;

///////////////////////////////////////////////////////////////////////////////////
//                               Other functions                                 //
///////////////////////////////////////////////////////////////////////////////////

uint8_t getControllerStatus() {
    return controller_status;
}

uint8_t getCOMMode() {
    return com_mode;
}

uint8_t setCOMMode(uint8_t mode) {
    com_mode = mode;
}

// Find previous motor configuration enabled
// This used to trace which motor is controlled
// If reached top or bottom of 'motor_enabled', return false
bool getPreviousMotor(uint8_t* motor_opt) {
    for (int i = *motor_opt - 1; i >= 0; i--) {
        if (motor_enabled[i] == ENABLED) {
            *motor_opt = i;
            return true;
        }
    }
    return false;
}

// Find first next motor configuration enabled from top
bool getNextMotor(uint8_t* motor_opt) {
    for (int i = *motor_opt + 1; i < NUM_OF_MOTOR; i++) {
        if (motor_enabled[i] == ENABLED) {
            *motor_opt = i;
            return true;
        }
    }
    return false;
}

// Find first motor configuration enabled from bottom
void getLastMotor(uint8_t* motor_opt) {
    for (int i = NUM_OF_MOTOR - 1; i >= 0; i--) {
        if (motor_enabled[i] == ENABLED) {
            *motor_opt = i;
            break;
        }
    }
}

void getTopMotor(uint8_t* motor_opt) {
    for (int i = 0; i < NUM_OF_MOTOR; i--) {
        if (motor_enabled[i] == ENABLED) {
            *motor_opt = i;
            break;
        }
    }
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

void saveMotorConfig(uint8_t motor, MotorConfig_t* motor_cfg);

bool CheckPinIdentical();

void updateSetPoint(uint8_t motor,
                    uint8_t control_purpose,
                    double new_set_point);

void updateThreshold(uint8_t motor,
                    uint8_t control_purpose,
                    double new_threshold);

void updateAngleStep(uint8_t motor,
                     uint16_t new_value);

void updateDirection(uint8_t motor,
                     bool direction);