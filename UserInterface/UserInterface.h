#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

///////////////////////////////////////////////////////////////////////////////////
//                                  Definition                                   //
///////////////////////////////////////////////////////////////////////////////////

#define HOME_SCREEN_MENU            0
#define MODE_SELECTION_MENU         1
#define CONTROLLER_SETTINGS_MENU    2
#define PIN_CONFIGURATION_MENU      3
#define COMMUNICATION_MENU          4

///////////////////////////////////////////////////////////////////////////////////
//                             Function prototypes                               //
///////////////////////////////////////////////////////////////////////////////////

/**
 * @brief This function will handle all event from user when interacting with HMI.
 * The UserInterface will contain 5 main menus corresponding to features from the
 * requirements: HomeScreen - ModeSelectionMenu - ControllerMenu - PinConfiguration
 * - CommunicationMenu. Each menu has a subprocess to handle the events. A
 * subprocess consists of display the menu action and event handler when 
 * interacting with buttons.
 * 
 * Overall, there are 3 actions in this function.
 * - ResetButtonFlag(): Clear button flag if the button is released.
 * - StartStopHandler(): Handle events when pressing START/STOP button.
 * - FSM: A FSM with state variable `menu_screen` is implemented, each state
 * consists of a menu (a subprocess).
 * 
 * Each function representing the subprocess must return a value, this value refers
 * to the next menu state that UserInterface() will go to in the next cycle. For
 * example, if user are in HOME_SCREEN_MENU state and HomeScreen() function returns
 * MODE_SELECTION_MENU, the FSM will go to state MODE_SELECTION_MENU and execute
 * ModeSelectionMenu().
 */
void UserInterface();

/**
 * @brief This function will handle events when pressing START/STOP button.
 * When START button is pressed, required conditions will be checked. If satisfied,
 * the controller status will be set as CTL_START. Otherwise, the controller log
 * will notify which condition is failed.
 * 
 * When STOP button is pressed, the controller status will be set as CTL_STOP.
 */
void StartStopHandler();

/**
 * @brief A simple FSM is implemented with state variable `menu_opt` in this 
 * subprocess. When user presses UP/DOWN buttons, the state is changed according to
 * menu option pointed on HMI. When OK button is pressed, the function might 
 * executes action if menu need requirements:
 * - getCOMMode(): get communication mode from database. Some menu option requires
 * communication mode is COM_MODE_DATA_TRANSFER_ONLY.
 */
uint8_t HomeScreen();

/**
 * @brief A simple FSM with state variable `mode_selection_state` is implemented in
 * this subprocess. The state is changed according to which option pointed in HMI.
 * When OK button is pressed, the function executes 2 actions:
 * - updateControlMode(): update controller mode corresponding to 
 * `mode_selection_state`.
 * - resetPinConfig(): reset the pin controller to default option corresponding to 
 * `mode_selection_state`.
 */
uint8_t ModeSelectionMenu();

/**
 * @brief A hierarchy FSM with state variable `controller_menu_state` is 
 * implemented in this subprocess. There are 2 states called FieldSelection, which
 * includes event handler when choosing fields displayed in HMI, and InputField,
 * which includes event handler when user change value of the field. When user
 * presses OK to change field value, the FSM will jump to InputField state, and
 * when user presses OK / BACK to confirm, the FSM will jump to FieldSelection 
 * state. 
 * 
 * A temporary motor configuration is used in this subprocess. This temporary
 * config is used for save the desired value when configuring, and only when the
 * user is confirmed by pressing OK, then the config is actually saved in database.
 * This config is also used for displaying in HMI.
 * 
 * A `menu_used` flag is used to know that if the menu is just opened. When 
 * open the menu first time, the menu find the first motor enabled index in
 * database, then the temporary config is overwriten from configuration in database
 * according to motor index, and `menu_used` is set as `true`. When user exits the 
 * menu, `menu_used` is cleared as `false`.  
 * 
 * When opening this menu, the current value in temporary config will be updated
 * continuously.
 * 
 * In FieldSelection state, a simple FSM with state variable `field_state` is 
 * implemented. The state `field_state` is changed depending on which field is 
 * pointed in HMI. FSM includes all possible states for all different motors.
 * 
 * The `field_state` state is related to callback function `updateInputFunc`. When
 * `field_state` changes, `updateInputFunc` is also updated accordingly. Each state
 * has a corresponding callback function (except Current Value field). Therefore, 
 * in InputField state, `updateInputFunc` is executed instead of using traditional
 * condition method.
 * 
 * Most field values are real value, or an option in a option list. Thus, callback
 * function `updateInputFunc` is implemented in simple way, such as if/else
 * condition for increment/decrement step value or switch-cases to change option.
 * 
 * A special case in this FSM is Motor Field state. When user changes the motor in 
 * 'Motor' Field, the temporary config is overwriten again from database for other
 * motor controller settings. This makes the menu displayed in HMI can be changed
 * corresponding to option while still in InputField state.
 */
uint8_t ControllerMenu();

/**
 * @brief A hierarchy FSM with state variable `pin_cfg_menu_state` is implemented
 * in this subprocess. Overall, the implementation is quite similar to in
 * ControllerMenu, including two states PinSelection and OptionSelection (like 
 * FieldSelection and Input Field), `menu_used` flag, and a FSM with state variable
 * `field_state` which saves possible fields. However, this FSM has more complexity
 * than ControllerMenu in some points.
 * 
 * From description, Pin Configuration has 2 different types of pin config: 
 * Controller indicator and Motor. If FSM is implemented in the same way as in
 * ControllerMenu, the all possible states needed in 1 FSM is very large. Thus, a
 * simple FSM with state variable `cfg_profile` is used to separate 2 config types,
 * each state includes 1 FSM like above. Menu and process for 2 types are nearly
 * independent.
 * - Motor pin config: related to Motor_PinConfigMenu(), Motor_PinSelection(), 
 * Motor_OptionSelection()
 * - Controller indicator pin config: related to Indicator_PinConfigMenu(), 
 * Indicator_PinSelection(), Indicator_OptionSelection()
 * 
 * Both these 2 FSM use `pin_cfg_menu_state` and `field_state` (similar in 
 * CommunicationMenu). In CFG profile Field, when option is changed from Motor to
 * Controller Indicator, `cfg_profile` state is also changed from MOTOR_PIN_CFG to
 * INDICATOR_CFG, while `pin_cfg_menu_state` and `field_state` are unchanged. This
 * makes the menu displayed in HMI can be changed corresponding to option while
 * still in OptionSelection state.
 * 
 * Another point is that all pin fields have finite option values to choose. So:
 * - 2 pin arrays `pwm_pin_gr` (PWM pins group) and `func_pin_gr` (FUNC pins group)
 * - Pin index `pin_opt_index` for finding the corresponding pin index in the array
 * - A pointer `pin` refering to which output pin is chosen currently in config
 * - A callback function pointer `changeOptionFunc` for changing pin field option
 * are used to optimize the option changing action in OptionSelection state.
 * 
 * Each pin state related to `changeOptionFunc` and `pin`. These values are updated
 * by setInputCallback(pin, changeOptionFunc) action in the corresponding state.
 * 
 * When FSM jump to OptionSelection state, a function called getOptionIndex() will
 * find the pin index based on what kind of motor type, which current pin option in
 * the config, then update `pin_opt_index`. Pin index is according to PWM pins 
 * group or FUNC pins group based on `field_state`. So when user change the option 
 * by pressing UP / DOWN, it actually change the pin index.
 * 
 * Because pin configuration can only be changed when the controller stops, so
 * instead of using temporary config, a pointer to pin configuration is used. This
 * means that the config in database will change directly when user changes field
 * value. Moreover, user can only quit the menu or controller starts if pin config 
 * is valid, so this still ensures the controller work in right conditions.
 */
uint8_t PinConfigurationMenu();

/**
 * @brief A simple FSM with state variable `communication_menu_state` is 
 * implemented in this subprocess, similar to ModeSelectionMenu().
 * When OK button is pressed, the function executes action:
 * - setCOMMode(): update communication mode corresponding to 
 * `communication_menu_state` state.
 */
uint8_t CommunicationMenu();

/**
 * @brief This prototype function displays the general UI, including Controller Log
 * Each menu will be displayed in this menu by using HMI_Menu_[menu]() function.
 */
void HMI_Display_Frame();

///////////////////////////////////////////////////////////////////////////////////
//                      Controller log function prototypes                       //
///////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Display controller log functions
 */
void HMI_Log(char* log);
void HMI_Warning(char* log);
void HMI_Error(char* log);

#endif