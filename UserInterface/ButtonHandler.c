#include <stdint.h>
#include <stdbool.h>

#define BTN_START   0
#define BTN_STOP    1
#define BTN_UP      2
#define BTN_DOWN    3
#define BTN_OK      4
#define BTN_BACK    5

#define BTN_PRESSED     0
#define BTN_RELEASED    1

uint8_t CheckButtonStatus(uint8_t button) {
    if (getButtonState(button) == BTN_PRESSED && btn_up_flag[button] == BTN_RELEASED) {
        btn_up_flag[button] = BTN_PRESSED;
        return BTN_PRESSED;
    }
    return BTN_RELEASED;
}

uint8_t btn_up_flag[6] = {
    BTN_RELEASED,       // BTN_START
    BTN_RELEASED,       // BTN_STOP
    BTN_RELEASED,       // BTN_UP
    BTN_RELEASED,       // BTN_DOWN
    BTN_RELEASED,       // BTN_OK
    BTN_RELEASED,       // BTN_BACK
};

void ResetButtonFlag() {
    if (getButtonState(BTN_START) == BTN_RELEASED) {
        btn_up_flag[BTN_START] == BTN_RELEASED;
    }
    //... other buttons
}