#include <stdint.h>
#include <stdbool.h>
#include "db/PinConfig.c"

#define CTL_START 0
#define CTL_STOP  1
#define CTL_FAULT 2

#define NUM_OF_MOTOR    4
#define BRUSHED_1_CFG   0
#define BRUSHED_2_CFG   1
#define BRUSHLESS_CFG   2
#define STEPPER_CFG     3

#define ONE_BRUSHED_MODE    0
#define TWO_BRUSHED_MODE    1
#define BRUSHLESS_MODE      2
#define STEPPER_MODE        3

#define BRUSHED_MOTOR_TYPE      0
#define BRUSHLESS_MOTOR_TYPE    1
#define STEPPER_MOTOR_TYPE      2

#define SPEED_CONTROL_PURPOSE       0
#define TORQUE_CONTROL_PURPOSE      1
#define POSITION_CONTROL_PURPOSE    2

#define FORWARD_DIRECTION   true
#define REVERSE_DIRECTION   false

#define ENABLED     true
#define DISABLED    false

// Controller status
static uint8_t controller_status = CTL_STOP;

typedef struct motor_cfg {
    uint8_t type_of_motor;
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

// Motor Configuration Database
MotorConfig_t motor_cfg_db[] = {
    {
        .type_of_motor = BRUSHED_MOTOR_TYPE,
        .control_purpose = SPEED_CONTROL_PURPOSE,
        .speed_value = 0,
        .speed_set_point = 0,
        .speed_threshold = 1000,
        .torque_value = 0,
        .torque_set_point = 0,
        .current_threshold = 10,
        .angle_position = 0,
        .angle_set_point = 0,
        .angle_step = 5,
        .direction = FORWARD_DIRECTION,
    },
    //...settings of each motor: motor 1 / motor 2 / brushless / stepper
};

// This indicates which motor is controlled
bool motor_controlled[] = {
    ENABLED,    // BRUSHED 1
    DISABLED,   // BRUSHED 2
    DISABLED,   // BRUSHLESS
    DISABLED,   // STEPPER
};

uint8_t getControllerStatus() {
    return controller_status;
}

// Find previous motor configuration enabled
// This used to trace which motor is controlled
// If reached top or bottom of 'motor_controlled', return false
bool getPreviousMotor(uint8_t* motor_opt) {
    for (int i = *motor_opt - 1; i >= 0; i--) {
        if (motor_controlled[i] == ENABLED) {
            *motor_opt = i;
            return true;
        }
    }
    return false;
}

// Find first next motor configuration enabled from top
bool getNextMotor(uint8_t* motor_opt) {
    for (int i = *motor_opt + 1; i < NUM_OF_MOTOR; i++) {
        if (motor_controlled[i] == ENABLED) {
            *motor_opt = i;
            return true;
        }
    }
    return false;
}

// Find first motor configuration enabled from bottom
void getLastMotor(uint8_t* motor_opt) {
    for (int i = NUM_OF_MOTOR - 1; i >= 0; i--) {
        if (motor_controlled[i] == ENABLED) {
            *motor_opt = i;
            break;
        }
    }
}

void getTopMotor(uint8_t* motor_opt) {
    for (int i = 0; i < NUM_OF_MOTOR; i--) {
        if (motor_controlled[i] == ENABLED) {
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
            motor_controlled[BRUSHED_1_CFG] = ENABLED;
            motor_controlled[BRUSHED_2_CFG] = motor_controlled[BRUSHLESS_CFG] = motor_controlled[STEPPER_CFG] = DISABLED; 
            break;
        case TWO_BRUSHED_MODE:
            motor_controlled[BRUSHED_1_CFG] = ENABLED;
            motor_controlled[BRUSHED_2_CFG] = ENABLED;
            break;
        case BRUSHLESS_MODE:
            motor_controlled[BRUSHLESS_CFG] = ENABLED;
            motor_controlled[BRUSHED_2_CFG] = motor_controlled[BRUSHED_1_CFG] = motor_controlled[STEPPER_CFG] = DISABLED; 
            break;
        case STEPPER_MODE:
            motor_controlled[STEPPER_CFG] = ENABLED;
            motor_controlled[BRUSHED_2_CFG] = motor_controlled[BRUSHLESS_CFG] = motor_controlled[BRUSHED_1_CFG] = DISABLED; 
            break;
    }
}

void saveMotorConfig(uint8_t motor, MotorConfig_t* motor_cfg);

bool checkPinIdentical();

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