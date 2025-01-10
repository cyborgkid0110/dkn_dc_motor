#include <stdint.h>
#include "db/MotorConfig.c"
#include "ButtonHandler.c"

#define MOTOR_PIN_CFG   0
#define INDICATOR_CFG   1

#define PIN_SELECTION_STATE     0
#define OPTION_SELECTION_STATE  1

#define CFG_PROFILE_STATE   0
#define PWM_A_PIN_STATE     1
#define PWM_B_PIN_STATE     2
#define PWM_C_PIN_STATE     3
#define PWM_D_PIN_STATE     4
#define DIR_PIN_STATE       5
#define EN_PIN_STATE        6
#define STEP_PIN_STATE      7
#define START_PIN_STATE     8
#define STOP_PIN_STATE      9
#define FAULT_PIN_STATE     10

#define PWM_PIN_OPTIONS     4
#define FUNC_PIN_OPTIONS    9
const OUTPUT_PIN pwm_pin_gr[] = { PWM1_PIN, PWM2_PIN, PWM3_PIN, PWM4_PIN};
const OUTPUT_PIN func_pin_gr[] = { FUNC1_PIN, FUNC2_PIN, FUNC3_PIN, FUNC4_PIN,
                                 FUNC5_PIN, FUNC6_PIN, FUNC7_PIN, FUNC8_PIN, NONE_PIN};

static bool menu_used = false;
static uint8_t cfg_profile = INDICATOR_CFG;
static uint8_t pin_cfg_menu_state = PIN_SELECTION_STATE;
static uint8_t field_state = CFG_PROFILE_STATE;
static uint8_t motor_opt = BRUSHED_1_CFG;
static void (*changeOptionFunc)(void) = NULL;
static uint8_t option_index;
MotorPinCfg_t* motor_pin_cfg;
IndicatorPinCfg_t* indicator_pin_cfg;

uint8_t PinConfiguration() {
    if (!menu_used) {
        getTopMotor(&motor_opt);
        getMotorPinConfig(&motor_pin_cfg);
        getIndicatorPinConfig(&indicator_pin_cfg);
        menu_used = true;
    }
    uint8_t menu_screen = 3;
    switch (cfg_profile) {
        case MOTOR_PIN_CFG:
            menu_screen = MotorPinConfig();
            break;
        case INDICATOR_CFG:
            menu_screen = IndicatorPinConfig();
            break;
    }
    return menu_screen;
}

uint8_t IndicatorPinConfig() {
    switch (pin_cfg_menu_state) {
        case PIN_SELECTION_STATE:
            Indicator_PinSelection();
            if (CheckButtonStatus(BTN_BACK) == BTN_PRESSED) {
                if (checkPinIdentical()) {
                    field_state = CFG_PROFILE_STATE;
                    menu_used = false;
                    return 0;
                }
                else {
                    // indicate error using Controller Log
                }
            }
            break;
        case OPTION_SELECTION_STATE:
            Indicator_OptionSelection();
            break;
    }
    return 3;
}

uint8_t MotorPinConfig() {
    switch (pin_cfg_menu_state) {
        case PIN_SELECTION_STATE:
            Motor_PinSelection();
            if (CheckButtonStatus(BTN_BACK) == BTN_PRESSED) {
                if (checkPinIdentical()) {
                    field_state = CFG_PROFILE_STATE;
                    menu_used = false;
                    return 0;
                }
                else {
                    // indicate error using Controller Log
                }
            }
            break;
        case OPTION_SELECTION_STATE:
            Motor_OptionSelection();
            break;
    }
    return 3;
}

void Motor_PinSelection() {
    // switch (field_state) {
    //     case CFG_PROFILE_STATE:
    //         changeOptionFunc = updateMotorField;
    //         field_state = (CheckButtonStatus(BTN_UP) == BTN_PRESSED)
    //                         ? DIRECTION_FIELD_STATE : MOTOR_TYPE_FIELD_STATE;
    //         field_state = (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED)
    //                         ? CONTROL_PURPOSE_FIELD_STATE : MOTOR_TYPE_FIELD_STATE;
    //         break;
    //     //...other cases are similar, except these 2 cases:
    //     case THRESHOLD_FIELD_STATE:
    //         changeOptionFunc = updateThresholdField;
    //         if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
    //             field_state = (motor_cfg.type_of_motor != STEPPER_CFG)
    //                         ? DIRECTION_FIELD_STATE : ANGLE_STEP_FIELD_STATE;
    //         }
    //         else if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
    //             field_state = SET_POINT_FIELD_STATE;
    //         }
    //         break;
    //     case DIRECTION_FIELD_STATE:
    //         changeOptionFunc = updateDirectionField;
    //         if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
    //             field_state = (motor_cfg.type_of_motor != STEPPER_CFG)
    //                         ? THRESHOLD_FIELD_STATE : ANGLE_STEP_FIELD_STATE;
    //         }
    //         else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
    //             field_state = MOTOR_TYPE_FIELD_STATE;
    //         }
    //         break;
    // }

    if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
        changeMotorPinFieldUp();
    }
    else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
        changeMotorPinFieldDown();
    }
    else if (CheckButtonStatus(BTN_OK) == BTN_PRESSED) {
        pin_cfg_menu_state = OPTION_SELECTION_STATE;
        getOptionIndex();
    }
}

void Motor_OptionSelection() {
    if (CheckButtonStatus(BTN_OK) == BTN_PRESSED) {
        pin_cfg_menu_state = PIN_SELECTION_STATE;
        return;
    }
    switch(field_state) {
        case CFG_PROFILE_STATE:
            changeConfigProfile(getPreviousMotor, getLastMotor);
            break;
        case PWM_A_PIN_STATE:
        case PWM_B_PIN_STATE:
        case PWM_C_PIN_STATE:
        case PWM_D_PIN_STATE:
            if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                changePWMPinUp();
            }
            else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                changePWMPinDown();
            }
            break;
        case DIR_PIN_STATE:
        case EN_PIN_STATE:
        case STEP_PIN_STATE:
            if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                changeFuncPinUp();
            }
            else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                changeFuncPinDown();
            }
            break;
    }
}

void Indicator_PinSelection() {
    if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
        changeIndicatorPinFieldUp();
    }
    else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
        changeIndicatorPinFieldDown();
    }
    else if (CheckButtonStatus(BTN_OK) == BTN_PRESSED) {
        pin_cfg_menu_state = OPTION_SELECTION_STATE;
    }
}

void Indicator_OptionSelection() {
    if (CheckButtonStatus(BTN_OK) == BTN_PRESSED) {
        pin_cfg_menu_state = PIN_SELECTION_STATE;
    }
    switch(field_state) {
        case CFG_PROFILE_STATE:
            changeConfigProfile(getLastMotor, getTopMotor);
            break;
        case START_PIN_STATE:
        case STOP_PIN_STATE:
        case FAULT_PIN_STATE:
            if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                changeFuncPinUp();
            }
            else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                changeFuncPinDown();
            }
            break;
    }
}

void changeConfigProfile(bool (*getMotorCfgProfile1)(uint8_t*), bool (*getMotorCfgProfile2)(uint8_t*)) {
    bool status;

    if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
        status = getMotorCfgProfile1(&motor_opt);
        if (!status) {
            cfg_profile = INDICATOR_CFG;
        }
    } else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
        status = getMotorCfgProfile2(&motor_opt);
        if (!status) {
            cfg_profile = INDICATOR_CFG;
        }
    }
}

static void getOptionIndex() {
    OUTPUT_PIN pin;
    OUTPUT_PIN* list;
    uint8_t len;
    bool pwm_pin_type = false;

    switch (field_state) {
        case PWM_A_PIN_STATE:
            pin = motor_pin_cfg->PWM_A;
            pwm_pin_type = true;
            break;
        case PWM_B_PIN_STATE:
            pin = motor_pin_cfg->PWM_B;
            pwm_pin_type = true;
            break;
        case PWM_C_PIN_STATE:
            pin = motor_pin_cfg->PWM_C;
            pwm_pin_type = true;
            break;
        case PWM_D_PIN_STATE:
            pin = motor_pin_cfg->PWM_D;
            pwm_pin_type = true;
            break;
        case DIR_PIN_STATE:
            pin = motor_pin_cfg->DIR;
            break;
        case EN_PIN_STATE:
            pin = motor_pin_cfg->EN;
            break;
        case STEP_PIN_STATE:
            pin = motor_pin_cfg->STEP;
            break;
        case START_PIN_STATE:
            pin = indicator_pin_cfg->START;
            break;
        case STOP_PIN_STATE:
            pin = indicator_pin_cfg->STOP;
            break;
        case FAULT_PIN_STATE:
            pin = indicator_pin_cfg->FAULT;
            break;
            break;
    }
    list = (pwm_pin_type) ? pwm_pin_gr : func_pin_gr;
    list = (pwm_pin_type) ? PWM_PIN_OPTIONS : FUNC_PIN_OPTIONS;

    for (uint8_t i = 0; i < len; i++) {
        if (pin == list[i]) {
            option_index = i;
            break;
        }
    }
}

static void changePWMPinUp();
static void changePWMPinDown();
static void changeFuncPinUp();
static void changeFuncPinDown();

static void changeMotorPinFieldUp();
static void changeMotorPinFieldDown();
static void changeIndicatorPinFieldUp();
static void changeIndicatorPinFieldDown();