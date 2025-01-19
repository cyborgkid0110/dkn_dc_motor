#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

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
//                             Function prototypes                               //
///////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Get the motor pin configuration corresponding to motor index.
 */
void getMotorPinConfig(MotorPinCfg_t** pin_cfg);

/**
 * @brief Get the indicator pin configuration corresponding to motor index.
 */
void getIndicatorPinConfig(IndicatorPinCfg_t** pin_cfg);

/**
 * @brief Reset the pin controller to default pin configuration according to `mode`
 */
void resetPinConfig(uint8_t mode);

/**
 * @brief Check the pins are identical in the configuration. If yes, return `true`,
 * else `false`.
 */
bool CheckPinIdentical();

#endif