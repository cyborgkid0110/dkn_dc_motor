#include <stdint.h>
#include "db/MotorConfig.c"
#include "ButtonHandler.c"

///////////////////////////////////////////////////////////////////////////////////
//                                  Definition                                   //
///////////////////////////////////////////////////////////////////////////////////

#define DATA_TRANSFER_MODE  0
#define CONTROLLER_MODE     1   

///////////////////////////////////////////////////////////////////////////////////
//                                Main functions                                 //
///////////////////////////////////////////////////////////////////////////////////

uint8_t communication_menu_state = DATA_TRANSFER_MODE;
uint8_t CommunicationMenu() {
    if (CheckButtonStatus(BTN_BACK) == BTN_PRESSED) {
        communication_menu_state = DATA_TRANSFER_MODE;
        return 0;
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
                return 0;
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
                return 0;
            }
            break;
    }
    return 4;
}