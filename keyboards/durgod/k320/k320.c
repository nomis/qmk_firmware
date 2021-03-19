/* Copyright 2021 kuenhlee and Don Kjer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "k320.h"
#include <ch.h>
#include <hal.h>

/* Private Functions */
void off_all_leds(void) {
    writePinHigh(LED_CAPS_LOCK_PIN);
    writePinHigh(LED_SCROLL_LOCK_PIN);
    writePinHigh(LED_WIN_LOCK_PIN);
    writePinHigh(LED_MR_LOCK_PIN);
}

void on_all_leds(void) {
    writePinLow(LED_CAPS_LOCK_PIN);
    writePinLow(LED_SCROLL_LOCK_PIN);
    writePinLow(LED_WIN_LOCK_PIN);
    writePinLow(LED_MR_LOCK_PIN);
}

/* WinLock and MR LEDs are non-standard. Need to override led init */
void led_init_ports(void) {
    setPinOutput(LED_CAPS_LOCK_PIN);
    setPinOutput(LED_SCROLL_LOCK_PIN);
    setPinOutput(LED_WIN_LOCK_PIN);
    setPinOutput(LED_MR_LOCK_PIN);
    off_all_leds();
}

#ifndef HW_RESET_PIN_DISABLED
static void hardware_reset_cb(void *arg) {
    chSysLockFromISR();
    bootloader_jump();
    chSysUnlockFromISR();
}
#endif

void keyboard_pre_init_kb(void) {
    setPinInputHigh(HARDWARE_RESET_PIN);

#ifndef HW_RESET_PIN_DISABLED
    /* Jump to bootloader when the hardware reset button is pressed */
    palEnablePadEvent(PAL_PORT(HARDWARE_RESET_PIN), PAL_PAD(HARDWARE_RESET_PIN), PAL_EVENT_MODE_FALLING_EDGE);
    palSetPadCallback(PAL_PORT(HARDWARE_RESET_PIN), PAL_PAD(HARDWARE_RESET_PIN), hardware_reset_cb, NULL);
#endif
}
