#ifndef MOTOR_CONFIG_H
#define MOTOR_CONFIG_H

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
//                             Function prototypes                               //
///////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Get the Controller Status state.
 * 
 * @return uint8_t
 * - CTL_START: controller is operating
 * - CTL_STOP: controller stopped
 * - CTL_FAULT: fault occurred
 */
uint8_t getControllerStatus();

/**
 * @brief Set the Controller Status state. If successful, return `true`, else
 * `false`.
 */
bool setControllerStatus(uint8_t state);

/**
 * @brief Get the COM mode.
 * 
 * @return uint8_t
 * - COM_MODE_DATA_TRANSFER_ONLY: COM is used for data transfer only
 * - COM_MODE_CONTROLLER: COM is used for remote control
 */
uint8_t getCOMMode();
uint8_t setCOMMode(uint8_t mode);

/**
 * @brief These functions are used to trace which motor is currently controlled
 * (enabled) in the `motor_enabled` array. There are 4 functions with different 
 * way to do this:
 * - getPreviousMotor: Get the previous motor index from old `motor_opt` value
 * - getNextMotor: Get the next motor index from old `motor_opt` value
 * - getLastMotor: Get the first motor index in the `motor_enabled` array
 * - getTopMotor: Get the last motor index in the `motor_enabled` array
 * 
 * @return If it can find the enabled motor, the
 * `motor_opt` value will be updated, then it will return `true`, otherwise 
 * `false`.
 */
// 
bool getPreviousMotor(uint8_t* motor_opt);
bool getNextMotor(uint8_t* motor_opt);
bool getLastMotor(uint8_t* motor_opt);
bool getTopMotor(uint8_t* motor_opt);

/**
 * @brief Get the Motor Configuration w.r.t motor index
 */
MotorConfig_t getMotorConfig(uint8_t motor);

/**
 * @brief This function will update the `motor_enabled` array corresponding to
 * controller mode.
 */
void updateControlMode(uint8_t mode);

/**
 * @brief This function overwrite (save) the motor config by a customized config
 * `motor_cfg`. The motor index indicates which motor's configuration is saved. 
 * 
 * @param motor motor index
 * @param motor_cfg motor configuration to save
 */
void saveMotorConfig(uint8_t motor, MotorConfig_t* motor_cfg);

/**
 * @brief Get the setpoint value in motor configuration according to control
 * purpose.
 */
double getCurrentValue(uint8_t motor, uint8_t control_purpose);

/**
 * @brief This function will update the setpoint of motor corresponding to
 * control purpose and motor index.
 */
void updateSetPoint(uint8_t motor,
                    uint8_t control_purpose,
                    double new_set_point);

/**
 * @brief This function will update the threshold value of motor corresponding to
 * control purpose and motor index.
 */
void updateThreshold(uint8_t motor,
                    uint8_t control_purpose,
                    double new_threshold);

/**
 * @brief This function will update the angle step value of stepper motor 
 * corresponding to motor index.
 */
void updateAngleStep(uint8_t motor,
                     uint16_t new_value);

/**
 * @brief This function will update the direction config of motor corresponding to
 * control purpose and motor index.
 */
void updateDirection(uint8_t motor,
                     bool direction);

#endif