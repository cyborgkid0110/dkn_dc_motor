#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

///////////////////////////////////////////////////////////////////////////////////
//                                  Definition                                   //
///////////////////////////////////////////////////////////////////////////////////

#define BTN_START   0
#define BTN_STOP    1
#define BTN_UP      2
#define BTN_DOWN    3
#define BTN_OK      4
#define BTN_BACK    5

#define BTN_PRESSED     0
#define BTN_RELEASED    1

///////////////////////////////////////////////////////////////////////////////////
//                             Function prototypes                               //
///////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Check whether the button is pressed or released. 
 * If pressed, return `BTN_PRESSED`, otherwise `BTN_RELEASED`.
 * 
 * This function use a status flag and getButtonState() function to ensure the 
 * button is considered as pressed once only when being pressed.
 * 
 * - Each button has a status flag and is saved in `btn_up_flag` array. This flag
 * indicates the 'past' state of the button. When a button is just being pressed 
 * and the status is checked by CheckButtonStatus() first time, the flag will be 
 * set as `BTN_PRESSED`. If the button status is checked by CheckButtonStatus()
 * while still pressed, the button is not considered as pressed because the past 
 * state is `BTN_PRESSED`, thus the function will return `BTN_RELEASED`.
 * 
 * - getButtonState(): get the status of the button in real-time (similar to 
 * digitalRead() in Arduino).
 */
uint8_t CheckButtonStatus(uint8_t button);

/**
 * @brief Clear the button flag when the button is released.
 */
void ResetButtonFlag();

#endif