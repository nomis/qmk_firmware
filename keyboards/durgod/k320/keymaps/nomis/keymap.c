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
	L_FUNC,
	L_NUM,
};

static bool win_key_locked = false;

enum custom_keycodes {
	CK_TGUI = SAFE_RANGE,   // Toggle between GUI Lock or Unlock
	CK_PAUS,                // KC_MPLY (or KC_PAUS when modifiers held)
	CK_RATE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	/* Keymap L_BASE: Base Layer (Default Layer)
	 * ,-----------------------------------------------------------.  ,--------------.
	 * |Esc  |f1| f2| f3| f4|  | f5| f6| f7| f8|   | f9|f10|f11|f12|  |Prnt|ScLk|Play|
	 * |-----------------------------------------------------------|  |--------------|
	 * | ` | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |  0| - | = |Backsp |  | Ins|Home|PgUp|
	 * |-----------------------------------------------------------|  |--------------|
	 * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|Enter|  | Del| End|PgDn|
	 * |------------------------------------------------------.    |  `--------------'
	 * |CAPS  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  #|    |
	 * |-----------------------------------------------------------|       ,----.
	 * |Shft|  \|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift     |       | Up |
	 * |-----------------------------------------------------------|  ,--------------.
	 * |Ctrl|Gui |Alt |      Space            |Alt |Gui  |Func|Ctrl|  |Left| Dn |Rght|
	 * `-----------------------------------------------------------'  `--------------'
	 */
	[L_BASE] = LAYOUT_tkl_iso(
		KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,             KC_PSCR,  KC_SLCK,  CK_PAUS,
		KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,   KC_HOME,  KC_PGUP,
		KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,            KC_DEL,   KC_END,   KC_PGDN,
		KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,
		KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,            KC_UP,
		KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                       KC_RALT,  KC_RGUI,  MO(L_FUNC),KC_RCTL, KC_LEFT,  KC_DOWN,  KC_RGHT
	),
	/* Keymap L_FUNC: Function Layer
	 * ,-----------------------------------------------------------.  ,--------------.
	 * |Reset|  |   |   |   |  |   |   |   |   |   |   |   |   |   |  |    |    |Paus|
	 * |-----------------------------------------------------------|  |--------------|
	 * |RcS|Rc1|Rc2|   |   |   |   |   |   |   |   |   |   |       |  |Mcr1|Musi|Vol+|
	 * |-----------------------------------------------------------|  |--------------|
	 * |     |   |Wak|Eml|   |   |   |   |   |   |Pwr|   |   |     |  |Mcr2|Mute|Vol-|
	 * |------------------------------------------------------.    |  `--------------'
	 * |      |   |Slp|   |   |   |   |   |   |   |   |   |   |    |
	 * |-----------------------------------------------------------|       ,----.
	 * |    |   |   |   |Cal|   |   |NlK|   |   |   |   |          |       |    |
	 * |-----------------------------------------------------------|  ,--------------.
	 * |    |Lock|    |                       |    |App  |Func|    |  |Prev|    |Next|
	 * `-----------------------------------------------------------'  `--------------'
	 */
	[L_FUNC] = LAYOUT_tkl_iso(
		RESET,    CK_RATE,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,  KC_PAUS,
		DM_RSTP,  DM_REC1,  DM_REC2,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  DM_PLY1,  KC_MSEL,  KC_VOLU,
		_______,  _______,  KC_WAKE,  KC_MAIL,  _______,  _______,  _______,  _______,  _______,  _______,  KC_PWR,   _______,  _______,            DM_PLY2,  KC_MUTE,  KC_VOLD,
		_______,  _______,  KC_SLEP,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  TG(L_NUM), _______,
		_______,  _______,  _______,  _______,  KC_CALC,  _______,  _______,  KC_NLCK,  _______,  _______,  _______,  _______,            _______,            _______,
		_______,  CK_TGUI,  _______,                                _______,                                _______,  KC_APP,   _______,  _______,  KC_MPRV,  _______,  KC_MNXT
	),
	/* Keymap L_NUM: Numeric Keypad Layer
	 * ,-----------------------------------------------------------.  ,--------------.
	 * |     |  |   |   |   |  |   |   |   |   |   |   |   |   |   |  | 7  | 8  | 9  |
	 * |-----------------------------------------------------------|  |--------------|
	 * |   |   |   |   |   |   |   |   | * |   |   | - | + |       |  | 4  | 5  | 6  |
	 * |-----------------------------------------------------------|  |--------------|
	 * |     |   |   |   |   |   |   |   |   |   |   |   |   |     |  | 1  | 2  | 3  |
	 * |------------------------------------------------------.    |  `--------------'
	 * |      |   |   |   |   |   |   |   |   |   |   |   |   |    |
	 * |-----------------------------------------------------------|       ,----.
	 * |    |   |   |   |   |   |   |   |   |   |   | / |          |       | 0  |
	 * |-----------------------------------------------------------|  ,--------------.
	 * |    |    |    |                       |    |     |Func|    |  |    | .  |    |
	 * `-----------------------------------------------------------'  `--------------'
	 */
	[L_NUM] = LAYOUT_tkl_iso(
		_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_7,     KC_8,     KC_9,
		_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_PAST,  _______,  _______,  KC_PMNS,  KC_PPLS,  _______,  KC_4,     KC_5,     KC_6,
		_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_1,     KC_2,     KC_3,
		_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
		_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_PSLS,            _______,            KC_0,
		_______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  KC_DOT,   _______
	),
#if 0
	/* Keymap L_EMPTY: Empty Layer
	 * ,-----------------------------------------------------------.  ,--------------.
	 * |     |  |   |   |   |  |   |   |   |   |   |   |   |   |   |  |    |    |    |
	 * |-----------------------------------------------------------|  |--------------|
	 * |   |   |   |   |   |   |   |   |   |   |   |   |   |       |  |    |    |    |
	 * |-----------------------------------------------------------|  |--------------|
	 * |     |   |   |   |   |   |   |   |   |   |   |   |   |     |  |    |    |    |
	 * |------------------------------------------------------.    |  `--------------'
	 * |      |   |   |   |   |   |   |   |   |   |   |   |   |    |
	 * |-----------------------------------------------------------|       ,----.
	 * |    |   |   |   |   |   |   |   |   |   |   |   |          |       |    |
	 * |-----------------------------------------------------------|  ,--------------.
	 * |    |    |    |                       |    |     |Func|    |  |    |    |    |
	 * `-----------------------------------------------------------'  `--------------'
	 */
	[L_EMPTY] = LAYOUT_tkl_iso(
		_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,  _______,
		_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
		_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,  _______,
		_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
		_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
		_______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______,  _______
	),
#endif
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	static uint16_t paus_keycode = KC_NO;
	action_t action;

	switch (keycode) {
		case CK_TGUI:
			if (!record->event.pressed) {
				// Toggle GUI lock on key release
				win_key_locked = !win_key_locked;
				writePin(LED_WIN_LOCK_PIN, !win_key_locked);
			}
			break;
		case KC_LGUI:
		case KC_RGUI:
			if (win_key_locked) { return false; }
			break;

		case CK_PAUS:
			if (record->event.pressed) {
				if (get_mods() & MOD_MASK_CSAG) {
					paus_keycode = KC_PAUS;
				} else {
					paus_keycode = KC_MPLY;
				}
			}

			action.code = ACTION_KEY(paus_keycode);
			process_action(record, action);
			return false;

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

	if (layer_state_cmp(state, L_NUM) != layer_state_cmp(prev_state, L_NUM)) {
		writePin(LED_MR_LOCK_PIN, !layer_state_cmp(state, L_NUM));
	}

	return prev_state = state;
}
