/* Copyright 2021 Simon Arlott
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
#include QMK_KEYBOARD_H

#include <stdio.h>
#include "raw_hid.h"
#include "users/nomis/raw-identify.h"
//#include "users/nomis/unicode-input-mode.h"
#include "users/nomis/usb-events.h"

enum my_layers {
	L_BASE,
	L_AUTO,
	L_REAL,
	L_FUNC,
};

enum custom_keycodes {
	CK_RATE = SAFE_RANGE,
	CK_3_0S,
	CK_6_0S,
	CK_9_0S,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	/* Keymap L_BASE: Base Layer (Default Layer)
	 * ,-------------------.
	 * |Esc |Calc|Func|Bksp|
	 * |-------------------|
	 * |NmLk|  / |  * |  - |
	 * |-------------------|
	 * |  7 |  8 |  9 |    |
	 * |--------------|  + |
	 * |  4 |  5 |  6 |    |
	 * |-------------------|
	 * |  1 |  2 |  3 |    |
	 * |--------------|Ent |
	 * |    0    |  . |    |
	 * `-------------------'
	 */
	[L_BASE] = LAYOUT_numpad_6x4(
		KC_ESC,    KC_CALC,   MO(L_FUNC),KC_BSPC,
		KC_NLCK,   KC_PSLS,   KC_PAST,   KC_PMNS,
		KC_7,      KC_8,      KC_9,
		KC_4,      KC_5,      KC_6,      KC_PPLS,
		KC_1,      KC_2,      KC_3,
		KC_0,                 KC_DOT,    KC_PENT
	),
	/* Keymap L_AUTO: Auto Keypad Layer
	 * ,-------------------.
	 * |    |    |    |    |
	 * |-------------------|
	 * |    |    |    |    |
	 * |-------------------|
	 * |  7 |  8 |  9 |    |
	 * |--------------|    |
	 * |  4 |  5 |  6 |    |
	 * |-------------------|
	 * |  1 |  2 |  3 |    |
	 * |--------------|    |
	 * |    0    |  . |    |
	 * `-------------------'
	 */
	[L_AUTO] = LAYOUT_numpad_6x4(
		_______,   _______,   _______,   _______,
		_______,   _______,   _______,   _______,
		KC_P7,     KC_P8,     KC_P9,
		KC_P4,     KC_P5,     KC_P6,     _______,
		KC_P1,     KC_P2,     KC_P3,
		KC_P0,                KC_PDOT,   _______
	),
	/* Keymap L_REAL: Real Keypad Layer
	 * ,-------------------.
	 * |    |    |    |    |
	 * |-------------------|
	 * |    |    |    |    |
	 * |-------------------|
	 * |  7 |  8 |  9 |    |
	 * |--------------|    |
	 * |  4 |  5 |  6 |    |
	 * |-------------------|
	 * |  1 |  2 |  3 |    |
	 * |--------------|    |
	 * |    0    |  . |    |
	 * `-------------------'
	 */
	[L_REAL] = LAYOUT_numpad_6x4(
		_______,   _______,   _______,   _______,
		_______,   _______,   _______,   _______,
		KC_P7,     KC_P8,     KC_P9,
		KC_P4,     KC_P5,     KC_P6,     _______,
		KC_P1,     KC_P2,     KC_P3,
		KC_P0,                KC_PDOT,   _______
	),
	/* Keymap L_FUNC: Function Layer
	 * ,-------------------.
	 * |Rset|    |Func|    |
	 * |-------------------|
	 * |Real|    |    |    |
	 * |-------------------|
	 * |    |    | 0*9|    |
	 * |--------------|    |
	 * |    |    | 0*6|    |
	 * |-------------------|
	 * |    |    | 0*3|    |
	 * |--------------|Rate|
	 * |         |  , |    |
	 * `-------------------'
	 */
	[L_FUNC] = LAYOUT_numpad_6x4(
		RESET,     _______,   _______,   _______,
		TG(L_REAL),_______,   _______,   _______,
		_______,   _______,   CK_9_0S,
		_______,   _______,   CK_6_0S,   _______,
		_______,   _______,   CK_3_0S,
		_______,              KC_COMM,   CK_RATE
	),
};

#define BRIGHTNESS 128
#undef HSV_ORANGE
#define HSV_ORANGE           10, 255, 255
#define HSV_LIGHT_BLUE      145, 255, 255
#define HSV_MID_BLUE1       153, 255, 255
#define HSV_MID_BLUE2       162, 255, 255
#define HSV_GREY              0,   0, BRIGHTNESS/2

#define HSV_DIM(x) x*0 + BRIGHTNESS
#define R_OS_NONE 0
static const rgblight_segment_t PROGMEM os_none_layer[] = RGBLIGHT_LAYER_SEGMENTS(
	{0, 1, HSV_DIM(HSV_CYAN)},
	{1, 1, HSV_DIM(HSV_MAGENTA)},
	{2, 1, HSV_GREY},
	{3, 1, HSV_DIM(HSV_CYAN)},
	{4, 1, HSV_DIM(HSV_MAGENTA)},
	{5, 1, HSV_GREY}
);
#define R_OS_DEFAULT 1
static const rgblight_segment_t PROGMEM os_default_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, RGBLED_NUM, HSV_DIM(HSV_WHITE)});
#define R_OS_LINUX 2
static const rgblight_segment_t PROGMEM os_linux_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, RGBLED_NUM, HSV_DIM(HSV_YELLOW)});
#define R_OS_WINDOWS 3
static const rgblight_segment_t PROGMEM os_windows_layer[] = RGBLIGHT_LAYER_SEGMENTS(
	{0, 1, HSV_DIM(HSV_MID_BLUE1)},
	{1, 1, HSV_DIM(HSV_LIGHT_BLUE)},
	{2, 1, HSV_DIM(HSV_MID_BLUE1)},
	{3, 1, HSV_DIM(HSV_MID_BLUE2)},
	{4, 1, HSV_DIM(HSV_BLUE)},
	{5, 1, HSV_DIM(HSV_MID_BLUE2)}
);
#define R_OS_MAC 4
static const rgblight_segment_t PROGMEM os_apple_layer[] = RGBLIGHT_LAYER_SEGMENTS(
	{0, 1, HSV_DIM(HSV_GREEN)},
	{1, 1, HSV_DIM(HSV_YELLOW)},
	{2, 1, HSV_DIM(HSV_ORANGE)},
	{3, 1, HSV_DIM(HSV_RED)},
	{4, 1, HSV_DIM(HSV_PURPLE)},
	{5, 1, HSV_DIM(HSV_LIGHT_BLUE)}
);

static const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
	[R_OS_NONE] = os_none_layer,
	[R_OS_DEFAULT] = os_default_layer,
	[R_OS_LINUX] = os_linux_layer,
	[R_OS_WINDOWS] = os_windows_layer,
	[R_OS_MAC] = os_apple_layer
);

static inline void led_sethsv(uint16_t hue, uint8_t sat, uint8_t val) {
	rgblight_sethsv_noeeprom(hue, sat, val == 255 ? BRIGHTNESS : 0);
}

static inline void led_sethsv_at(uint16_t hue, uint8_t sat, uint8_t val, uint8_t idx) {
	sethsv(hue, sat, val == 255 ? BRIGHTNESS : 0, (LED_TYPE *)&led[idx]);
}

static bool led_hsv_eq(HSV hsv1, uint16_t hue2, uint8_t sat2, uint8_t val2) {
	return hsv1.h == hue2
		&& hsv1.s == sat2
		&& hsv1.v == val2;
}

static bool num_lock = false;
static bool real_layer = false;
static void update_leds(void) {
	if (real_layer) {
		if (num_lock) {
			led_sethsv(HSV_ORANGE);
		} else {
			led_sethsv(HSV_RED);
		}
	} else {
		if (num_lock) {
			led_sethsv(HSV_GREEN);
		} else {
			led_sethsv(HSV_BLACK);
		}
	}
}

static void update_auto_layer(void) {
	if (layer_state_is(L_AUTO) != num_lock) {
		if (num_lock) {
			layer_on(L_AUTO);
		} else {
			layer_off(L_AUTO);
		}
	}
}

static void set_os(enum raw_identify id) {
	switch (id) {
	case ID_LINUX:
		//set_unicode_input_mode_noeeprom(UC_LNX);
		break;

	case ID_WINDOWS:
		//set_unicode_input_mode_noeeprom(UC_WINC);
		break;

	case ID_MAC:
		//set_unicode_input_mode_noeeprom(UC_MAC);
		break;

	default:
		//set_unicode_input_mode_noeeprom(UC_LNX);
		break;
	}
}

static void blink_os(enum raw_identify id) {
	switch (id) {
	case ID_LINUX:
		rgblight_blink_layer(R_OS_LINUX, 1000);
		break;

	case ID_WINDOWS:
		rgblight_blink_layer(R_OS_WINDOWS, 1000);
		break;

	case ID_MAC:
		rgblight_blink_layer(R_OS_MAC, 1000);
		break;

	default:
		rgblight_blink_layer(R_OS_DEFAULT, 1000);
		break;
	}
}


static void raw_identify_user(enum raw_identify id) {
	set_os(id);
	blink_os(id);
}

static void usb_event_user(enum usb_event event) {
	switch (event) {
	case USB_EVT_CONNECTED:
		blink_os(ID_DEFAULT);
		break;

	case USB_EVT_DISCONNECTED:
		set_os(ID_DEFAULT);
		rgblight_blink_layer(R_OS_NONE, 1000);
		break;

	case USB_EVT_SUSPENDED:
		break;
	}
}

void keyboard_post_init_user(void) {
	if (!rgblight_is_enabled() || rgblight_get_mode() != RGBLIGHT_MODE_STATIC_LIGHT || !led_hsv_eq(rgblight_get_hsv(), HSV_BLACK)) {
		rgblight_enable_noeeprom();
		rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
		rgblight_sethsv(HSV_BLACK);
		led_sethsv(HSV_CYAN);
		wait_ms(1000);
	}

	if (is_backlight_enabled()) {
		backlight_disable();
		eeconfig_update_backlight_current();
		led_sethsv(HSV_CYAN);
		wait_ms(1000);
	}

	rgblight_layers = my_rgb_layers;
	set_os(ID_DEFAULT);

	num_lock = host_keyboard_led_state().num_lock;
	real_layer = layer_state_is(L_REAL);
	update_auto_layer();
	update_leds();
}

bool led_update_user(led_t led_state) {
	if (num_lock != led_state.num_lock) {
		num_lock = led_state.num_lock;
		update_auto_layer();
		update_leds();
	}

	return true;
}

uint32_t layer_state_set_user(uint32_t state) {
	if (real_layer != layer_state_cmp(state, L_REAL)) {
		real_layer = layer_state_cmp(state, L_REAL);
		update_leds();
	}

	return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {
		switch (keycode) {
		case RESET:
			rgblight_layers = NULL;
			led_sethsv(HSV_PURPLE);
			wait_ms(1);
			break;

		case CK_RATE:
			{
				char scan_rate[15];
				snprintf(scan_rate, sizeof(scan_rate), " %lu", get_matrix_scan_rate());
				send_string(scan_rate);
				SEND_STRING(SS_TAP(X_ENTER));
			}
			return false;

		case CK_9_0S:
		case CK_6_0S:
		case CK_3_0S:
			for (uint8_t i = (keycode - CK_3_0S) + 1; i > 0; i--) {
				SEND_STRING("000");
			}
			return false;
		}
	}
	return true;
}

void housekeeping_task_user(void) {
	usb_event_check();
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
	raw_hid_receive_identify(data, length);
}
