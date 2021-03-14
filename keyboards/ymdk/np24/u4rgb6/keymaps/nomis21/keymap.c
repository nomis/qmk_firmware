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
static const HSV red = { 0, 255, BRIGHTNESS };
static const HSV orange = { 10, 255, BRIGHTNESS };
static const HSV green = { 85, 255, BRIGHTNESS };
static const HSV blue = { 170, 255, BRIGHTNESS };
static const HSV cyan = { 128, 255, BRIGHTNESS };
static const HSV white = { 0, 0, BRIGHTNESS };
static const HSV black = { HSV_BLACK };

static void led_sethsv(HSV hsv) {
	rgblight_sethsv_noeeprom(hsv.h, hsv.s, hsv.v);
}

static bool led_hsv_eq(HSV hsv1, HSV hsv2) {
	return hsv1.h == hsv2.h
		&& hsv1.s == hsv2.s
		&& hsv1.v == hsv2.v;
}

static bool num_lock = false;
static bool real_layer = false;
static void update_leds(void) {
	if (real_layer) {
		if (num_lock) {
			led_sethsv(orange);
		} else {
			led_sethsv(red);
		}
	} else {
		if (num_lock) {
			led_sethsv(green);
		} else {
			led_sethsv(black);
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

void keyboard_post_init_user(void) {
	if (!rgblight_is_enabled() || rgblight_get_mode() != RGBLIGHT_MODE_STATIC_LIGHT || !led_hsv_eq(rgblight_get_hsv(), black)) {
		rgblight_enable_noeeprom();
		rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
		rgblight_sethsv(HSV_BLACK);
		led_sethsv(cyan);
		wait_ms(1000);
	}

	if (is_backlight_enabled()) {
		backlight_disable();
		eeconfig_update_backlight_current();
		led_sethsv(cyan);
		wait_ms(1000);
	}

	led_sethsv(white);
	wait_ms(500);

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
			led_sethsv(blue);
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
