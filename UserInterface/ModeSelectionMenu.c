#include <stdint.h>
#include "ButtonHandler.h"
#include "UserInterface.h"
#include "db/MotorConfig.h"

///////////////////////////////////////////////////////////////////////////////////
//                                Main functions                                 //
///////////////////////////////////////////////////////////////////////////////////

uint8_t mode_selection_state = ONE_BRUSHED_MODE;
uint8_t ModeSelectionMenu() {
    HMI_Menu_ModeSelectionMenu();
    if (CheckButtonStatus(BTN_OK) == BTN_PRESSED) {
        updateControlMode(mode_selection_state);
        resetPinConfig(mode_selection_state);
        return PIN_CONFIGURATION_MENU;
    }
    if (CheckButtonStatus(BTN_BACK) == BTN_PRESSED) {
        return HOME_SCREEN_MENU;
    }
    switch (mode_selection_state) {
        case ONE_BRUSHED_MODE:
            if (CheckButtonStatus(BTN_UP) == BTN_PRESSED)  {
                mode_selection_state = STEPPER_MODE;
            }
            else if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED)  {
                mode_selection_state = TWO_BRUSHED_MODE;
            }
            break;
        //... similar to other cases
    }
    return MODE_SELECTION_MENU;
}