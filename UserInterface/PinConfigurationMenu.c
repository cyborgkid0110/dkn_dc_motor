#include <stdint.h>
#include "db/MotorConfig.c"
#include "ButtonHandler.c"

///////////////////////////////////////////////////////////////////////////////////
//                                  Definition                                   //
///////////////////////////////////////////////////////////////////////////////////

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

typedef void (*changePinOption)(OUTPUT_PIN*);

///////////////////////////////////////////////////////////////////////////////////
//                               Global variables                                //
///////////////////////////////////////////////////////////////////////////////////

const OUTPUT_PIN pwm_pin_gr[] = { PWM1_PIN, PWM2_PIN, PWM3_PIN, PWM4_PIN};
const OUTPUT_PIN func_pin_gr[] = { FUNC1_PIN, FUNC2_PIN, FUNC3_PIN, FUNC4_PIN,
                                   FUNC5_PIN, FUNC6_PIN, FUNC7_PIN, FUNC8_PIN, NONE_PIN};

static bool menu_used = false;
static uint8_t cfg_profile = INDICATOR_CFG;
static uint8_t pin_cfg_menu_state = PIN_SELECTION_STATE;
static uint8_t field_state = CFG_PROFILE_STATE;
static uint8_t motor_opt = BRUSHED_1_CFG;

static changePinOption changeOptionFunc = NULL;
static OUTPUT_PIN* pin;
static int pin_opt_index;

MotorPinCfg_t* motor_pin_cfg;
IndicatorPinCfg_t* indicator_pin_cfg;

///////////////////////////////////////////////////////////////////////////////////
//                                Main functions                                 //
///////////////////////////////////////////////////////////////////////////////////

uint8_t PinConfiguration() {
    uint8_t menu_screen;
    if (!menu_used) {
        getTopMotor(&motor_opt);
        getMotorPinConfig(&motor_pin_cfg);
        getIndicatorPinConfig(&indicator_pin_cfg);
        menu_used = true;
    }
    switch (cfg_profile) {
        case MOTOR_PIN_CFG:
            menu_screen = Motor_PinConfigMenu();
            break;
        case INDICATOR_CFG:
            menu_screen = Indicator_PinConfigMenu();
            break;
    }
    return menu_screen;
}

uint8_t Indicator_PinConfigMenu() {
    bool status;
    switch (pin_cfg_menu_state) {
        case PIN_SELECTION_STATE:
            status = Indicator_PinSelection();
            return (status) ? 0 : 3;
            break;
        case OPTION_SELECTION_STATE:
            Indicator_OptionSelection();
            break;
    }
    return 3;
}

uint8_t Motor_PinConfigMenu() {
    bool status;
    switch (pin_cfg_menu_state) {
        case PIN_SELECTION_STATE:
            status = Motor_PinSelection();
            return (status) ? 0 : 3;
            break;
        case OPTION_SELECTION_STATE:
            Motor_OptionSelection();
            break;
    }
    return 3;
}

bool Motor_PinSelection() {
    bool status = false;    // false = NOT QUIT MENU, true = QUIT MENU

    if (CheckButtonStatus(BTN_OK) == BTN_PRESSED &&
        getCOMMode() == COM_MODE_DATA_TRANSFER_ONLY) 
    {
        if (getCOMMode() == COM_MODE_DATA_TRANSFER_ONLY) {
            pin_cfg_menu_state = OPTION_SELECTION_STATE;
            getOptionIndex();
        }
        else {
            // indicate error using Controller Log
        }
        return status;
    }
    else if (CheckButtonStatus(BTN_BACK) == BTN_PRESSED) {
        if (CheckPinIdentical()) {
            field_state = CFG_PROFILE_STATE;
            menu_used = false;
            status = true;
        }
        else {
            // indicate error using Controller Log
        }
        return status;
    }

    switch (field_state) {
        case CFG_PROFILE_STATE:
            if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                field_state = (motor_pin_cfg->motor_type == STEPPER_MOTOR_TYPE)
                            ? STEP_PIN_STATE : EN_PIN_STATE;
            }
            else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                field_state = PWM_A_PIN_STATE;
            }
            break;
        case PWM_A_PIN_STATE:
            setInputCallBack(changePWMPin, &(motor_pin_cfg->PWM_A));
            if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                field_state = CFG_PROFILE_STATE;
            }
            else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                field_state = PWM_B_PIN_STATE;
            }
            break;
        case PWM_B_PIN_STATE:
            setInputCallBack(changePWMPin, &(motor_pin_cfg->PWM_B));
            if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                field_state = PWM_A_PIN_STATE;
            }
            else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                field_state = (motor_pin_cfg->motor_type == BRUSHED_MOTOR_TYPE)
                            ? DIR_PIN_STATE : PWM_C_PIN_STATE;
            }
            break;
        case PWM_C_PIN_STATE:
            setInputCallBack(changePWMPin, &(motor_pin_cfg->PWM_C));
            if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                field_state = PWM_B_PIN_STATE;
            }
            else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                field_state = (motor_pin_cfg->motor_type == STEPPER_MOTOR_TYPE)
                            ? PWM_D_PIN_STATE : DIR_PIN_STATE;
            }
            break;
        case PWM_D_PIN_STATE:
            setInputCallBack(changePWMPin, &(motor_pin_cfg->PWM_D));
            if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                field_state = PWM_C_PIN_STATE;
            }
            else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                field_state = DIR_PIN_STATE;
            }
            break;
        case DIR_PIN_STATE:
            setInputCallBack(changeFuncPin, &(motor_pin_cfg->DIR));
            if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                switch (motor_pin_cfg->motor_type) {
                    case STEPPER_MOTOR_TYPE:
                        field_state = PWM_D_PIN_STATE;
                        break;
                    case BRUSHLESS_MOTOR_TYPE:
                        field_state = PWM_C_PIN_STATE;
                        break;
                    default:
                        field_state = PWM_B_PIN_STATE;
                        break;
                }
            }
            else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                field_state = EN_PIN_STATE;
            }
            break;
        case EN_PIN_STATE:
            setInputCallBack(changeFuncPin, &(motor_pin_cfg->EN));
            if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                field_state = DIR_PIN_STATE;
            }
            else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                field_state = (motor_pin_cfg->motor_type == STEPPER_MOTOR_TYPE)
                            ? STEP_PIN_STATE : CFG_PROFILE_STATE;
            }
            break;
        case STEP_PIN_STATE:
            setInputCallBack(changeFuncPin, &(motor_pin_cfg->STEP));
            if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                field_state = EN_PIN_STATE;
            }
            else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                field_state = CFG_PROFILE_STATE;
            }
            break;
    }
    return status;
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
        default:
            changeOptionFunc(pin);
            break;
    }
}

bool Indicator_PinSelection() {
    bool status = false;    // false = NOT QUIT MENU, true = QUIT MENU
    if (CheckButtonStatus(BTN_OK) == BTN_PRESSED &&) 
    {
        if (getCOMMode() == COM_MODE_DATA_TRANSFER_ONLY) {
            pin_cfg_menu_state = OPTION_SELECTION_STATE;
        }
        else {
            // indicate error using Controller Log
        }
        return status;
    }
    else if (CheckButtonStatus(BTN_BACK) == BTN_PRESSED) {
        if (CheckPinIdentical()) {
            field_state = CFG_PROFILE_STATE;
            menu_used = false;
            status = true;
        }
        else {
            // indicate error using Controller Log
        }
        return status;
    }
    switch (field_state) {
        case CFG_PROFILE_STATE:
            // setInputCallBack(changeFuncPin, &(motor_pin_cfg->[pin])); // with other cases
            if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                field_state = FAULT_PIN_STATE;
            }
            else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                field_state = START_PIN_STATE;
            }
            break;
        // ... other cases are similar
    }
    return status;
}

void Indicator_OptionSelection() {
    if (CheckButtonStatus(BTN_OK) == BTN_PRESSED) {
        pin_cfg_menu_state = PIN_SELECTION_STATE;
    }
    switch(field_state) {
        case CFG_PROFILE_STATE:
            changeConfigProfile(getLastMotor, getTopMotor);
            break;
        default:
            changeOptionFunc(pin);
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////////
//                               Other functions                                 //
///////////////////////////////////////////////////////////////////////////////////

void changeConfigProfile(bool (*getMotorEnabled1)(uint8_t*), bool (*getMotorEnabled2)(uint8_t*)) {
    bool status;

    if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
        status = getMotorEnabled1(&motor_opt);
        if (!status) {
            cfg_profile = INDICATOR_CFG;
        }
    } else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
        status = getMotorEnabled2(&motor_opt);
        if (!status) {
            cfg_profile = INDICATOR_CFG;
        }
    }
}

static void getOptionIndex() {
    OUTPUT_PIN* list;
    uint8_t len;
    bool pwm_pin_type = false;  // false = FUNC_PIN, true = PWM_PIN

    switch (field_state) {
        case PWM_A_PIN_STATE:
        case PWM_B_PIN_STATE:
        case PWM_C_PIN_STATE:
        case PWM_D_PIN_STATE:
            pin = motor_pin_cfg->PWM_D;
            pwm_pin_type = true;
            break;
    }
    list = (pwm_pin_type) ? pwm_pin_gr : func_pin_gr;
    list = (pwm_pin_type) ? PWM_PIN_OPTIONS : FUNC_PIN_OPTIONS;

    for (int i = 0; i < len; i++) {
        if (pin == list[i]) {
            pin_opt_index = i;
            break;
        }
    }
}

static void changePWMPin(OUTPUT_PIN* pin) {
    if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
        pin_opt_index++;
        if (pin_opt_index == PWM_PIN_OPTIONS) {
            pin_opt_index = 0;
        } 
    }
    else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
        pin_opt_index--;
        if (pin_opt_index < 0) {
            pin_opt_index = PWM_PIN_OPTIONS - 1;
        } 
    }
    else if ((CheckButtonStatus(BTN_OK) == BTN_PRESSED) ||
             (CheckButtonStatus(BTN_BACK) == BTN_PRESSED))
    {
        *pin = pwm_pin_gr[pin_opt_index];
    }
}

static void changeFuncPin(OUTPUT_PIN* pin) {
    if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
        pin_opt_index++;
        if (pin_opt_index == FUNC_PIN_OPTIONS) {
            pin_opt_index = 0;
        } 
    }
    else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
        pin_opt_index--;
        if (pin_opt_index < 0) {
            pin_opt_index = FUNC_PIN_OPTIONS - 1;
        } 
    }
    else if ((CheckButtonStatus(BTN_OK) == BTN_PRESSED) ||
             (CheckButtonStatus(BTN_BACK) == BTN_PRESSED))
    {
        *pin = func_pin_gr[pin_opt_index];
    }
}

static setInputCallBack(changePinOption func, OUTPUT_PIN* pin_ptr) {
    changeOptionFunc = func;
    pin = pin_ptr;
}