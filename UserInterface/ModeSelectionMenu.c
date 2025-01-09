#include <stdint.h>
#include "ButtonHandler.c"
#include "db/MotorConfig.c"

uint8_t mode_selection_state = ONE_BRUSHED_MODE;

uint8_t ModeSelectionMenu() {
    switch (mode_selection_state) {
        case ONE_BRUSHED_MODE:
            mode_selection_state = (CheckButtonStatus(BTN_UP) == BTN_PRESSED) 
                                   ? STEPPER_MODE : mode_selection_state;
            mode_selection_state = (CheckButtonStatus(BTN_DOWN) == BTN_PRESSED) 
                                   ? TWO_BRUSHED_MODE : mode_selection_state;
            break;
        //... similar to other cases
    }
    if (CheckButtonStatus(BTN_OK) == BTN_PRESSED) {
        updateControlMode(mode_selection_state);
        return 2;
    }
    return 1;
}