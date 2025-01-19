#include <stdint.h>
#include <stdbool.h>
#include "db/Definitions.h"
#include "db/MotorConfig.h"
#include "ButtonHandler.h"
#include "UserInterface.h"

///////////////////////////////////////////////////////////////////////////////////
//                                Main functions                                 //
///////////////////////////////////////////////////////////////////////////////////

uint8_t menu_screen = HOME_SCREEN_MENU;
void UserInterface() {
    HMI_Display_Frame();
    ResetButtonFlag();
    StartStopHandler();
    switch (menu_screen) {
        case HOME_SCREEN_MENU:
            menu_screen = HomeScreen();
            break;
        case MODE_SELECTION_MENU:
            menu_screen = ModeSelectionMenu();
            break;
        case CONTROLLER_SETTINGS_MENU:
            menu_screen = ControllerMenu();
            break;
        case PIN_CONFIGURATION_MENU:
            menu_screen = PinConfigurationMenu();
            break;
        case COMMUNICATION_MENU:
            menu_screen = CommunicationMenu();
            break;
    }
}

void StartStopHandler() {
    bool status;
    if (CheckButtonStatus(BTN_START) == BTN_PRESSED) {
        status = setControllerStatus(CTL_START);
        if (!status) {
            HMI_Warning("Start controller failed");
        }
        else {
            HMI_Log("Start controller");
        }
    }
    else if (CheckButtonStatus(BTN_STOP) == BTN_PRESSED) {
        if (!status) {
            HMI_Warning("Stop controller failed");
        }
        else {
            HMI_Log("Stop controller");
        }
    }
}