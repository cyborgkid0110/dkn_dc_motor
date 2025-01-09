#include <stdint.h>
#include <stdbool.h>
#include "ButtonHandler.c"
#include "HomeScreen.c"

#define HOME_SCREEN_MENU            0
#define MODE_SELECTION_MENU         1
#define CONTROLLER_SETTINGS_MENU    2
#define PIN_CONFIGURATION_MENU      3
#define COMMUNICATION_MENU          4

uint8_t menu_screen = HOME_SCREEN_MENU;

void UserInterface() {
    ResetButtonFlag();
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