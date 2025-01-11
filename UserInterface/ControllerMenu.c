#include <stdint.h>
#include <stdbool.h>
#include "ButtonHandler.c"
#include "db/MotorConfig.c"

///////////////////////////////////////////////////////////////////////////////////
//                                  Definition                                   //
///////////////////////////////////////////////////////////////////////////////////

#define FIELD_SELECT_STATE  0
#define INPUT_STATE         1

#define MOTOR_TYPE_FIELD_STATE      0
#define CONTROL_PURPOSE_FIELD_STATE 1
#define CURRENT_VALUE_FIELD_STATE   2
#define SET_POINT_FIELD_STATE       3
#define THRESHOLD_FIELD_STATE       4
#define ANGLE_STEP_FIELD_STATE      5
#define DIRECTION_FIELD_STATE       6

#define SPEED_CHANGE_STEP       10.0
#define TORQUE_CHANGE_STEP      10.0
#define CURRENT_CHANGE_STEP     1
#define ANGLE_STEP_CHANGE_STEP  5

///////////////////////////////////////////////////////////////////////////////////
//                               Global variables                                //
///////////////////////////////////////////////////////////////////////////////////

static bool menu_used = false;
static uint8_t controller_menu_state = FIELD_SELECT_STATE;
static uint8_t field_state = MOTOR_TYPE_FIELD_STATE;
static uint8_t motor_opt = BRUSHED_1_CFG;
static void (*updateInputFunc)(void) = updateMotorField;
MotorConfig_t motor_cfg;

///////////////////////////////////////////////////////////////////////////////////
//                                Main functions                                 //
///////////////////////////////////////////////////////////////////////////////////

uint8_t ControllerMenu() {
    bool status;
    if (!menu_used) {
        getTopMotor(&motor_opt);
        getMotorConfig(motor_opt);
        menu_used = true;
    }
    switch (controller_menu_state) {
        case FIELD_SELECT_STATE:
            status = FieldSelect();
            return (status) ? 0 : 2;
            break;
        case INPUT_STATE:
            InputHandler();
            break;
    }
    return 2;
}

bool FieldSelect() {
    if (CheckButtonStatus(BTN_BACK) == BTN_PRESSED) {
        field_state = MOTOR_TYPE_FIELD_STATE;
        menu_used = false;
        saveMotorConfig(motor_opt, &motor_cfg);
        return true;
    }
    else if (CheckButtonStatus(BTN_OK) == BTN_PRESSED && 
             field_state != CURRENT_VALUE_FIELD_STATE &&
             getCOMMode() == COM_MODE_DATA_TRANSFER_ONLY) 
    {
        controller_menu_state = INPUT_STATE;
        return false;
    }
    switch (field_state) {
        case MOTOR_TYPE_FIELD_STATE:
            updateInputFunc = updateMotorField;
            if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                field_state = DIRECTION_FIELD_STATE;
            }
            else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                field_state = CONTROL_PURPOSE_FIELD_STATE;
            }
            break;
        //...other cases are similar, except these 2 cases:
        case THRESHOLD_FIELD_STATE:
            updateInputFunc = updateThresholdField;
            if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                field_state = (motor_cfg.motor_type != STEPPER_CFG)
                            ? DIRECTION_FIELD_STATE : ANGLE_STEP_FIELD_STATE;
            }
            else if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                field_state = SET_POINT_FIELD_STATE;
            }
            break;
        case DIRECTION_FIELD_STATE:
            updateInputFunc = updateDirectionField;
            if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                field_state = (motor_cfg.motor_type != STEPPER_CFG)
                            ? THRESHOLD_FIELD_STATE : ANGLE_STEP_FIELD_STATE;
            }
            else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                field_state = MOTOR_TYPE_FIELD_STATE;
            }
            break;
    }
    return false;
}

void InputHandler() {
    if (CheckButtonStatus(BTN_BACK) == BTN_PRESSED ||
        CheckButtonStatus(BTN_OK) == BTN_PRESSED) 
    {
        controller_menu_state = FIELD_SELECT_STATE;
    }
    else {
        updateInputFunc();
    }
}

///////////////////////////////////////////////////////////////////////////////////
//                               Other functions                                 //
///////////////////////////////////////////////////////////////////////////////////

void updateMotorField() {
    if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
        saveMotorConfig(motor_opt, &motor_cfg);
        getPreviousMotor(&motor_opt);
        motor_cfg = getMotorConfig(motor_opt);
    }
    else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
        saveMotorConfig(motor_opt, &motor_cfg);
        getNextMotor(&motor_opt);
        motor_cfg = getMotorConfig(motor_opt);
    }
}

void updateControlPurposeField() {
    if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
        if (motor_cfg.motor_type == STEPPER_MOTOR_TYPE) {
            if (motor_cfg.control_purpose == POSITION_CONTROL_PURPOSE) {
                motor_cfg.control_purpose == SPEED_CONTROL_PURPOSE;
            }
            else {
                motor_cfg.control_purpose += 1;
            }
        }
        else {
            motor_cfg.control_purpose = (motor_cfg.control_purpose == SPEED_CONTROL_PURPOSE)
                                        ? TORQUE_CONTROL_PURPOSE : SPEED_CONTROL_PURPOSE; 
        }
    }
    else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
        if (motor_cfg.motor_type == STEPPER_MOTOR_TYPE) {
            if (motor_cfg.control_purpose == SPEED_CONTROL_PURPOSE) {
                motor_cfg.control_purpose == POSITION_CONTROL_PURPOSE;
            }
            else {
                motor_cfg.control_purpose -= 1;
            }
        }
        else {
            motor_cfg.control_purpose = (motor_cfg.control_purpose == SPEED_CONTROL_PURPOSE)
                                        ? TORQUE_CONTROL_PURPOSE : SPEED_CONTROL_PURPOSE; 
        }
    }
}

void updateSetPointField() {
    if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
        if (motor_cfg.control_purpose == SPEED_CONTROL_PURPOSE) {
            motor_cfg.speed_set_point += SPEED_CHANGE_STEP;
        }
        else if (motor_cfg.control_purpose == TORQUE_CONTROL_PURPOSE) {
            motor_cfg.torque_set_point += TORQUE_CHANGE_STEP;
        }
        else if (motor_cfg.control_purpose == POSITION_CONTROL_PURPOSE) {
            motor_cfg.angle_set_point += motor_cfg.angle_step;
            motor_cfg.angle_set_point = (motor_cfg.angle_set_point < 361) ? motor_cfg.angle_set_point : 360;
        }
    }
    else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
        if (motor_cfg.control_purpose == SPEED_CONTROL_PURPOSE) {
            motor_cfg.speed_set_point -= SPEED_CHANGE_STEP;
            motor_cfg.speed_set_point = (motor_cfg.speed_set_point > 0) ? motor_cfg.speed_set_point : 0;
        }
        else if (motor_cfg.control_purpose == TORQUE_CONTROL_PURPOSE) {
            motor_cfg.torque_set_point -= TORQUE_CHANGE_STEP;
            motor_cfg.torque_set_point = (motor_cfg.torque_set_point > 0) ? motor_cfg.torque_set_point : 0;
        }
        else if (motor_cfg.control_purpose == POSITION_CONTROL_PURPOSE) {
            motor_cfg.angle_set_point -= motor_cfg.angle_step;
            motor_cfg.angle_set_point = (motor_cfg.angle_set_point > 0 & motor_cfg.angle_set_point < 361) 
                                        ? motor_cfg.angle_set_point : 0;
        }
    }
}

void updateThresholdField() {
    if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
        if (motor_cfg.control_purpose == SPEED_CONTROL_PURPOSE) {
            motor_cfg.speed_threshold += SPEED_CHANGE_STEP;
        }
        else if (motor_cfg.control_purpose == TORQUE_CONTROL_PURPOSE) {
            motor_cfg.current_threshold += TORQUE_CHANGE_STEP;
        }
    }
    else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
        if (motor_cfg.control_purpose == SPEED_CONTROL_PURPOSE) {
            motor_cfg.speed_threshold -= SPEED_CHANGE_STEP;
            motor_cfg.speed_threshold = (motor_cfg.speed_threshold > 0) ? motor_cfg.speed_threshold : 0;
        }
        else if (motor_cfg.control_purpose == TORQUE_CONTROL_PURPOSE) {
            motor_cfg.current_threshold -= CURRENT_CHANGE_STEP;
            motor_cfg.current_threshold = (motor_cfg.current_threshold > 0) ? motor_cfg.current_threshold : 0;
        }
    }
}

void updateAngleStepField() {
    if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
        motor_cfg.angle_step += ANGLE_STEP_CHANGE_STEP;
        motor_cfg.angle_step = (motor_cfg.angle_step < 361) ? motor_cfg.angle_step : 360;
    }
    else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
        motor_cfg.angle_step = motor_cfg.angle_step + ANGLE_STEP_CHANGE_STEP;
        motor_cfg.angle_step = (motor_cfg.angle_step > 0 & motor_cfg.angle_step < 361) 
                                ? motor_cfg.angle_step : 0;
    }
}

void updateDirectionField() {
    if (CheckButtonStatus(BTN_UP) == BTN_PRESSED ||
        CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) 
    {
        motor_cfg.direction = (motor_cfg.direction == FORWARD_DIRECTION)
                              ? FORWARD_DIRECTION : REVERSE_DIRECTION;
    }
}