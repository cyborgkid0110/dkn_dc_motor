#include <stdint.h>
#include "db/MotorConfig.c"
#include "ButtonHandler.c"

///////////////////////////////////////////////////////////////////////////////////
//                                  Definition                                   //
///////////////////////////////////////////////////////////////////////////////////

#define MODE_SELECTION_MENU_OPT         1
#define CONTROLLER_SETTINGS_MENU_OPT    2
#define PIN_CONFIGURATION_MENU_OPT      3
#define COMMUNICATION_MENU_OPT          4

///////////////////////////////////////////////////////////////////////////////////
//                                Main functions                                 //
///////////////////////////////////////////////////////////////////////////////////

uint8_t menu_opt = MODE_SELECTION_MENU_OPT;
uint8_t HomeScreen() {
    switch (menu_opt) {
        // with menus can be accessed in data transfer only COM mode:
        case MODE_SELECTION_MENU_OPT:
            if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                menu_opt = CONTROLLER_SETTINGS_MENU_OPT;
            }
            else if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                menu_opt = COMMUNICATION_MENU_OPT;
            }
            else if (CheckButtonStatus(BTN_OK) == BTN_PRESSED) {
                if (getCOMMode() == COM_MODE_DATA_TRANSFER_ONLY) {
                    return menu_opt;
                }
                else {
                    // Controller Log function
                }
            }
            break;
        // with menus can be accessed in any COM mode:
        case CONTROLLER_SETTINGS_MENU_OPT:
            if (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) {
                menu_opt = PIN_CONFIGURATION_MENU_OPT;
            }
            else if (CheckButtonStatus(BTN_UP) == BTN_PRESSED) {
                menu_opt = MODE_SELECTION_MENU_OPT;
            }
            else if (CheckButtonStatus(BTN_OK) == BTN_PRESSED) {
                return menu_opt;
            }
            break;
        //... similar with other cases
    }
    return 0;
}