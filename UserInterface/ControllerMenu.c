#include <stdint.h>
#include <stdbool.h>
#include "ButtonHandler.c"
#include "db/MotorConfig.c"

#define FIELD_SELECT_STATE  0
#define INPUT_STATE         1

#define MOTOR_TYPE_INPUT_STATE      1
#define CONTROL_PURPOSE_INPUT_STATE 2
#define SET_POINT_INPUT_STATE       3
#define THRESHOLD_INPUT_STATE       4
#define ANGLE_STEP_INPUT_STATE      5
#define DIRECTION_INPUT_STATE       6

#define MOTOR_TYPE_FIELD        0
#define CONTROL_PURPOSE_FIELD   1
#define CURRENT_VALUE_FIELD     2
#define SET_POINT_FIELD         3
#define THRESHOLD_FIELD         4
#define ANGLE_STEP_FIELD        5
#define DIRECTION_FIELD         6

#define SPEED_CHANGE_STEP       10.0
#define TORQUE_CHANGE_STEP      10.0
#define CURRENT_CHANGE_STEP     1
#define ANGLE_STEP_CHANGE_STEP  5

// controller_menu -> UI
// type_of_motor

uint8_t controller_menu_state = FIELD_SELECT_STATE;
uint8_t input_state = FIELD_SELECT_STATE;
uint8_t current_field = MOTOR_TYPE_FIELD;
uint8_t motor_option = BRUSHED_1_CFG;
MotorConfig_t motor_cfg;    // when init, this will be brushed 1 motor config

uint8_t ControllerMenu() {
    switch (controller_menu_state) {
        case FIELD_SELECT_STATE:
            FieldSelect();
            if (CheckButtonStatus(BTN_BACK) == BTN_PRESSED) {
                return 0;
            }
            break;
        case INPUT_STATE:
            InputHandler();
            break;
    }
    return 2;
}

void FieldSelect() {
    if (CheckButtonStatus(BTN_BACK) == BTN_PRESSED)
    {
        controller_menu_state = FIELD_SELECT_STATE;
        current_field = MOTOR_TYPE_FIELD;
        saveMotorConfig(motor_option, &motor_cfg);
    }
    else if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
        changeFieldUp();
    }
    else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
        changeFieldDown();
    }
    else if (CheckButtonStatus(BTN_OK) == BTN_PRESSED) {
        controller_menu_state = INPUT_STATE;
        switch (current_field) {
            case MOTOR_TYPE_FIELD:
                input_state = MOTOR_TYPE_INPUT_STATE;
                break;
            case CONTROL_PURPOSE_FIELD:
                input_state = CONTROL_PURPOSE_INPUT_STATE;
                break;
            case SET_POINT_FIELD:
                input_state = SET_POINT_INPUT_STATE;
                break;
            case THRESHOLD_FIELD:
                input_state = THRESHOLD_INPUT_STATE;
                break;
            case ANGLE_STEP_FIELD:
                input_state = ANGLE_STEP_INPUT_STATE;
                break;
            case DIRECTION_FIELD:
                input_state = DIRECTION_INPUT_STATE;
                break;
        }
    }
}

void InputHandler() {
    if (CheckButtonStatus(BTN_BACK) == BTN_PRESSED ||
        CheckButtonStatus(BTN_OK) == BTN_PRESSED) 
    {
        controller_menu_state = FIELD_SELECT_STATE;
        current_field = MOTOR_TYPE_FIELD;
        return;
    }
    switch (input_state) {
        case MOTOR_TYPE_INPUT_STATE:
            updateMotorField();
            break;
        case CONTROL_PURPOSE_INPUT_STATE:
            updateControlPurposeField();
            break;
        case SET_POINT_INPUT_STATE:
            updateSetPointField();
            break;
        case THRESHOLD_INPUT_STATE:
            updateThresholdField();
            break;
        case ANGLE_STEP_INPUT_STATE:
            updateAngleStepField();
            break;
        case DIRECTION_INPUT_STATE:
            updateDirectionField();
            break;
    }
}

void changeFieldUp() {
    if (current_field == DIRECTION_FIELD) {
            current_field = MOTOR_TYPE_FIELD;
    }
    else if (current_field == THRESHOLD_FIELD) {
        if (motor_option == STEPPER_CFG) {
            current_field = ANGLE_STEP_FIELD;
        }
        else {
            current_field = DIRECTION_FIELD;
        }
    }
    else {
        current_field += 1;
    }
}

void changeFieldDown() {
    if (current_field == MOTOR_TYPE_FIELD) {
        current_field = DIRECTION_FIELD;
    }
    else if (current_field == DIRECTION_FIELD) {
        if (motor_option == STEPPER_CFG) {
            current_field = ANGLE_STEP_FIELD;
        }
        else {
            current_field = THRESHOLD_FIELD;
        }
    }
    else {
        current_field -= 1;
    }
}

void updateMotorField() {
    if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
        saveMotorConfig(motor_option, &motor_cfg);
        motor_option = goToPreviousMotor(motor_option);
        motor_cfg = getMotorConfig(motor_option);
    }
    else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
        saveMotorConfig(motor_option, &motor_cfg);
        motor_option = goToNextMotor(motor_option);
        motor_cfg = getMotorConfig(motor_option);
    }
}

void updateControlPurposeField() {
    if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
        if (motor_cfg.type_of_motor == STEPPER_MOTOR_TYPE) {
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
        if (motor_cfg.type_of_motor == STEPPER_MOTOR_TYPE) {
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