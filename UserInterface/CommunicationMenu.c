#include <stdint.h>
#include "db/MotorConfig.h"
#include "ButtonHandler.h"
#include "UserInterface.h"

///////////////////////////////////////////////////////////////////////////////////
//                                Main functions                                 //
///////////////////////////////////////////////////////////////////////////////////

uint8_t communication_menu_state = COM_MODE_DATA_TRANSFER_ONLY;
uint8_t CommunicationMenu() {
    HMI_Menu_CommunicationMenu();
    if (CheckButtonStatus(BTN_BACK) == BTN_PRESSED) {
        return HOME_SCREEN_MENU;
    }
    switch (menu_opt) {
        case DATA_TRANSFER_MODE:
            if ((CheckButtonStatus(BTN_UP) == BTN_PRESSED) ||
                (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED)) 
            {
                communication_menu_state = CONTROLLER_MODE;
            }
            else if (CheckButtonStatus(BTN_OK) == BTN_PRESSED) {
                setCOMMode(COM_MODE_DATA_TRANSFER_ONLY);
                HMI_Log("Set COM Mode: Data transfer");
                return HOME_SCREEN_MENU;
            }
            break;
        case CONTROLLER_MODE:
            if ((CheckButtonStatus(BTN_UP) == BTN_PRESSED) ||
                (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED)) 
            {
                communication_menu_state = CONTROLLER_MODE;
            }
            else if (CheckButtonStatus(BTN_OK) == BTN_PRESSED) {
                communication_menu_state = CONTROLLER_MODE;
                setCOMMode(COM_MODE_CONTROLLER);
                HMI_Log("Set COM Mode: Controller");
                return HOME_SCREEN_MENU;
            }
            break;
    }
    return COMMUNICATION_MENU;
}