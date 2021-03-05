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
	L_REAL,
	L_FUNC,
};

enum custom_keycodes {
	CK_RATE = SAFE_RANGE,
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
	 * |    |    |Func|    |
	 * |-------------------|
	 * |Real|    |    |    |
	 * |-------------------|
	 * |    |    |    |    |
	 * |--------------|    |
	 * |    |    |    |    |
	 * |-------------------|
	 * |    |    |    |    |
	 * |--------------|    |
	 * |         |Rset|    |
	 * `-------------------'
	 */
	[L_FUNC] = LAYOUT_numpad_6x4(
		_______,   _______,   _______,   _______,
		TG(L_REAL),_______,   _______,   _______,
		_______,   _______,   _______,
		_______,   _______,   _______,   _______,
		_______,   _______,   _______,
		_______,              RESET,     CK_RATE
	),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case CK_RATE:
			if (record->event.pressed) {
				char scan_rate[15];
				snprintf(scan_rate, sizeof(scan_rate), "%lu", get_matrix_scan_rate());
				send_string(scan_rate);
				SEND_STRING(SS_TAP(X_ENTER));
			}
			return false;
	}
	return true;
}

uint32_t layer_state_set_user(uint32_t state) {
	static uint32_t prev_state = L_BASE;

	if (layer_state_cmp(state, L_REAL) != layer_state_cmp(prev_state, L_REAL)) {
		backlight_set(layer_state_cmp(state, L_REAL) ? 1 : 0);
	}

	return prev_state = state;
}
