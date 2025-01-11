#include <stdint.h>
#include "db_config.h"

///////////////////////////////////////////////////////////////////////////////////
//                                  Definition                                   //
///////////////////////////////////////////////////////////////////////////////////

typedef uint8_t OUTPUT_PIN;

typedef struct indicator_pin_cfg {
    OUTPUT_PIN START;
    OUTPUT_PIN STOP;
    OUTPUT_PIN FAULT;
} IndicatorPinCfg_t;

typedef struct motor_pin_cfg {
    uint8_t motor_type;
    OUTPUT_PIN PWM_A;
    OUTPUT_PIN PWM_B;
    OUTPUT_PIN PWM_C;
    OUTPUT_PIN PWM_D;
    OUTPUT_PIN DIR;
    OUTPUT_PIN EN;
    OUTPUT_PIN STEP;
} MotorPinCfg_t;

///////////////////////////////////////////////////////////////////////////////////
//                               Global variables                                //
///////////////////////////////////////////////////////////////////////////////////

IndicatorPinCfg_t indicator_pin_db = CONTROLLER_INDICATOR_DEFAULT_PIN;
MotorPinCfg_t motor_pin_db[] = CONTROLLER_DEFAULT_PIN;

///////////////////////////////////////////////////////////////////////////////////
//                               Other functions                                 //
///////////////////////////////////////////////////////////////////////////////////

void getMotorPinConfig(MotorPinCfg_t** pin_cfg);
void getIndicatorPinConfig(IndicatorPinCfg_t** pin_cfg);