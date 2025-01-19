#ifndef DB_CONFIG
#define DB_CONFIG

// controller status state
#define CTL_START 0
#define CTL_STOP  1
#define CTL_FAULT 2

// communication mode define
#define COM_MODE_DATA_TRANSFER_ONLY 0
#define COM_MODE_CONTROLLER         1

// motor configuration option
// this is used to access database by index
#define NUM_OF_MOTOR    4
#define BRUSHED_1_CFG   0
#define BRUSHED_2_CFG   1
#define BRUSHLESS_CFG   2
#define STEPPER_CFG     3

// mode selector for controller
#define ONE_BRUSHED_MODE    0
#define TWO_BRUSHED_MODE    1
#define BRUSHLESS_MODE      2
#define STEPPER_MODE        3

// motor type define
#define BRUSHED_MOTOR_TYPE      0
#define BRUSHLESS_MOTOR_TYPE    1
#define STEPPER_MOTOR_TYPE      2

// control purpose define
#define SPEED_CONTROL_PURPOSE       0
#define TORQUE_CONTROL_PURPOSE      1
#define POSITION_CONTROL_PURPOSE    2

// direction define
#define FORWARD_DIRECTION   true
#define REVERSE_DIRECTION   false

// motor controlled state define
#define ENABLED     true        // motor is being controlled
#define DISABLED    false       // motor is not controlled

// controller default settings
#define BRUSHED_1_DEFAULT_CONTROL { \
                                    .motor_type = BRUSHED_MOTOR_TYPE, \
                                    .control_purpose = SPEED_CONTROL_PURPOSE, \
                                    .speed_value = 0, \
                                    .speed_set_point = 0, \
                                    .speed_threshold = 1000, \
                                    .torque_value = 0, \
                                    .torque_set_point = 0, \
                                    .current_threshold = 10, \
                                    .angle_position = 0, \
                                    .angle_set_point = 0, \
                                    .angle_step = 5, \
                                    .direction = FORWARD_DIRECTION, \
                                }
#define BRUSHED_2_DEFAULT_CONTROL {...}     // similar to BRUSHED_1_DEFAULT_CONTROL
#define BRUSHLESS_DEFAULT_CONTROL {...}
#define STEPPER_DEFAULT_CONTROL {...}

#define CONTROLLER_DEFAULT_CONTROL  { \
                                        BRUSHED_1_DEFAULT_CONTROL, \
                                        BRUSHED_2_DEFAULT_CONTROL, \
                                        BRUSHLESS_DEFAULT_CONTROL, \ 
                                        STEPPER_DEFAULT_CONTROL, \
                                    }
#define MOTOR_ENABLED_DEFAULT   { \
                                    ENABLED, \      // BRUSHED 1
                                    DISABLED, \     // BRUSHED 2
                                    DISABLED, \     // BRUSHLESS
                                    DISABLED, \     // STEPPER
                                }

// pin index define
#define NONE_PIN    0
#define PWM1_PIN    1
#define PWM2_PIN    2
#define PWM3_PIN    3
#define PWM4_PIN    4
#define FUNC1_PIN   5
#define FUNC2_PIN   6
#define FUNC3_PIN   7
#define FUNC4_PIN   8
#define FUNC5_PIN   9
#define FUNC6_PIN   10
#define FUNC7_PIN   11
#define FUNC8_PIN   12

// motor pin default settings
#define CONTROLLER_INDICATOR_DEFAULT_PIN    { FUNC1_PIN, FUNC2_PIN, FUNC3_PIN }
#define BRUSHED_1_DEFAULT_PIN               { PWM1_PIN, PWM2_PIN, NONE_PIN, NONE_PIN, FUNC4_PIN, FUNC5_PIN, NONE_PIN }
#define BRUSHED_2_DEFAULT_PIN               { PWM3_PIN, PWM4_PIN, NONE_PIN, NONE_PIN, FUNC6_PIN, FUNC7_PIN, NONE_PIN }
#define STEPPER_DEFAULT_PIN                 { PWM1_PIN, PWM2_PIN, PWM3_PIN, PWM4_PIN, \
                                              FUNC4_PIN, FUNC5_PIN, FUNC6_PIN }
#define BRUSHLESS_DEFAULT_PIN               { PWM1_PIN, PWM2_PIN, PWM3_PIN, NONE_PIN, FUNC4_PIN, FUNC5_PIN, NONE_PIN }

#define CONTROLLER_DEFAULT_PIN  { \
                                    BRUSHED_1_DEFAULT_PIN, \
                                    BRUSHED_2_DEFAULT_PIN, \
                                    BRUSHLESS_DEFAULT_PIN, \ 
                                    STEPPER_DEFAULT_PIN, \
                                }

#endif // DB_CONFIG