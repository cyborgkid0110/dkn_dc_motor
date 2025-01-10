#include <stdint.h>
#include "ButtonHandler.c"

#define MODE_SELECTION_MENU_OPT         1
#define CONTROLLER_SETTINGS_MENU_OPT    2
#define PIN_CONFIGURATION_MENU_OPT      3
#define COMMUNICATION_MENU_OPT          4

uint8_t menu_opt = MODE_SELECTION_MENU_OPT;

uint8_t HomeScreen() {
    if (CheckButtonStatus(BTN_OK) == BTN_PRESSED) {
        return menu_opt;
    }
    switch (menu_opt) {
        case MODE_SELECTION_MENU_OPT:
            menu_opt = (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED)
                    ? CONTROLLER_SETTINGS_MENU_OPT : MODE_SELECTION_MENU_OPT;
            menu_opt = (CheckButtonStatus(BTN_UP) == BTN_PRESSED)
                    ? COMMUNICATION_MENU_OPT : MODE_SELECTION_MENU_OPT;
            break;
        //... similar to other cases;
    }
    return 0;
}