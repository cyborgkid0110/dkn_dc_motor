#include <stdint.h>

typedef uint8_t OUTPUT_PIN;

#define NONE_PIN    0
#define PWM1_PIN 1
#define PWM2_PIN 2
#define PWM3_PIN 3
#define PWM4_PIN 4
#define FUNC1_PIN  5
#define FUNC2_PIN  6
#define FUNC3_PIN  7
#define FUNC4_PIN  8
#define FUNC5_PIN  9
#define FUNC6_PIN  10
#define FUNC7_PIN  11
#define FUNC8_PIN  12

typedef struct indicator_pin_cfg {
    OUTPUT_PIN START;
    OUTPUT_PIN STOP;
    OUTPUT_PIN FAULT;
} IndicatorPinCfg_t;

typedef struct motor_pin_cfg {
    OUTPUT_PIN PWM_A;
    OUTPUT_PIN PWM_B;
    OUTPUT_PIN PWM_C;
    OUTPUT_PIN PWM_D;
    OUTPUT_PIN DIR;
    OUTPUT_PIN EN;
    OUTPUT_PIN STEP;
} MotorPinCfg_t;

#define CONTROLLER_INDICATOR_DEFAULT_PIN    { FUNC1_PIN, FUNC2_PIN, FUNC3_PIN }
#define BRUSHED_1_DEFAULT_PIN               { PWM1_PIN, PWM2_PIN, NONE_PIN, NONE_PIN, FUNC4_PIN, FUNC5_PIN, NONE_PIN }
#define BRUSHED_2_DEFAULT_PIN               { PWM3_PIN, PWM4_PIN, NONE_PIN, NONE_PIN, FUNC6_PIN, FUNC7_PIN, NONE_PIN }
#define STEPPER_DEFAULT_PIN                 { PWM1_PIN, PWM2_PIN, PWM3_PIN, PWM4_PIN, \
                                              FUNC4_PIN, FUNC5_PIN, FUNC6_PIN }
#define BRUSHLESS_DEFAULT_PIN               { PWM1_PIN, PWM2_PIN, PWM3_PIN, NONE_PIN, FUNC4_PIN, FUNC5_PIN, NONE_PIN }

IndicatorPinCfg_t indicator_pin_db = CONTROLLER_INDICATOR_DEFAULT_PIN;
MotorPinCfg_t motor_pin_db[] = {
    BRUSHED_1_DEFAULT_PIN,
    BRUSHED_2_DEFAULT_PIN,
    BRUSHLESS_DEFAULT_PIN,
    STEPPER_DEFAULT_PIN,
};

void getMotorPinConfig(MotorPinCfg_t** pin_cfg);
void getIndicatorPinConfig(IndicatorPinCfg_t** pin_cfg);