#include <stdint.h>
#include "ButtonHandler.c"

#define MODE_SELECTION_MENU_OPT         1
#define CONTROLLER_SETTINGS_MENU_OPT    2
#define PIN_CONFIGURATION_MENU_OPT      3
#define COMMUNICATION_MENU_OPT          4

uint8_t menu_opt = MODE_SELECTION_MENU_OPT;

uint8_t HomeScreen() {
    if (CheckButtonPressed(BTN_OK) == BTN_PRESSED) {
        return menu_opt;
    }

    if (CheckButtonPressed(BTN_DOWN) == BTN_PRESSED) {
        menu_opt = (menu_opt == COMMUNICATION_MENU_OPT)
                    ? COMMUNICATION_MENU_OPT
                    : menu_opt + 1;
    }
    if (CheckButtonPressed(BTN_UP) == BTN_PRESSED) {
        menu_opt = (menu_opt == MODE_SELECTION_MENU_OPT)
                    ? MODE_SELECTION_MENU_OPT
                    : menu_opt - 1;
    }
    return 0;
}