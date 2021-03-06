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

enum my_layers {
	L_BASE,
};

enum custom_keycodes {
	CK_O_PORTAL = SAFE_RANGE,
	CK_B_PORTAL,
	CK_HALFLIFE,
	CK_STIENCE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	/* Keymap L_BASE: Base Layer (Default Layer)
	 * ,---------------------------------------.
	 * | Portal  | Stience |   (λ)   | Portal  |
	 * | Orange  |  Blue   | Orange  |  Blue   |
	 * |         |         |         |         |
	 * |---------------------------------------|
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |---------------------------------------|
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |---------------------------------------|
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |---------------------------------------|
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |---------------------------------------|
	 * |  Teams  |  Teams  |  Zoom   |  Zoom   |
	 * | Toggle  | Toggle  | Toggle  | Toggle  |
	 * |  Mute   |  Video  |  Video  |  Mute   |
	 * `---------------------------------------'
	 */
	[L_BASE] = LAYOUT_ortho_6x4(
		CK_O_PORTAL,           CK_STIENCE,            CK_HALFLIFE,           CK_B_PORTAL,
		KC_NO,                 KC_NO,                 KC_NO,                 KC_NO,
		KC_NO,                 KC_NO,                 KC_NO,                 KC_NO,
		KC_NO,                 KC_NO,                 KC_NO,                 KC_NO,
		KC_NO,                 KC_NO,                 KC_NO,                 KC_NO,
		LCTL(LSFT(KC_M)),      LCTL(LSFT(KC_O)),      LALT(KC_V),            LALT(KC_A)
	),
#if 0
	/* Keymap L_EMPTY: Empty Layer
	 * ,---------------------------------------.
	 * | Portal  | Stience |   (λ)   | Portal  |
	 * | Orange  |  Blue   | Orange  |  Blue   |
	 * |         |         |         |         |
	 * |---------------------------------------|
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |---------------------------------------|
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |---------------------------------------|
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |---------------------------------------|
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |---------------------------------------|
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * |         |         |         |         |
	 * `---------------------------------------'
	 */
	[L_FUNC] = LAYOUT_ortho_6x4(
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______
	),
#endif
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

	led_sethsv(green);
	wait_ms(500);
	led_sethsv(black);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	static bool orange_portal = false;
	static bool blue_portal = false;

	switch (keycode) {
		case CK_O_PORTAL:
			orange_portal = record->event.pressed;
			break;

		case CK_B_PORTAL:
			blue_portal = record->event.pressed;
			break;

		case CK_STIENCE:
			if (record->event.pressed) {
				SEND_STRING(\
					"This was a triumph." SS_TAP(X_ENTER) \
					"I'm making a note here:" SS_TAP(X_ENTER) \
					"HUGE SUCCESS." SS_TAP(X_ENTER) \
					"It's hard to overstate" SS_TAP(X_ENTER) \
					"my satisfaction." SS_TAP(X_ENTER) \
					" " SS_TAP(X_ENTER) \
					"Aperture Science." SS_TAP(X_ENTER) \
					"We do what we must" SS_TAP(X_ENTER) \
					"because we can." SS_TAP(X_ENTER) \
					"For the good of all of us." SS_TAP(X_ENTER) \
					"Except the ones who are dead." SS_TAP(X_ENTER) \
					" " SS_TAP(X_ENTER) \
					"But there's no sense crying" SS_TAP(X_ENTER) \
					"over every mistake." SS_TAP(X_ENTER) \
					"You just keep on trying" SS_TAP(X_ENTER) \
					"till you run out of cake." SS_TAP(X_ENTER) \
					"And the Science gets done." SS_TAP(X_ENTER) \
					"And you make a neat gum." SS_TAP(X_ENTER) \
					"For the people who are" SS_TAP(X_ENTER) \
					"still alive." SS_TAP(X_ENTER) \
					" " SS_TAP(X_ENTER) \
					"I'm not even angry." SS_TAP(X_ENTER) \
					"I'm being so sincere right now." SS_TAP(X_ENTER) \
					"Even though you broke my heart." SS_TAP(X_ENTER) \
					"And killed me." SS_TAP(X_ENTER) \
					"And tore me to pieces." SS_TAP(X_ENTER) \
					"And threw every piece into a fire." SS_TAP(X_ENTER) \
					"As they burned it hurt because" SS_TAP(X_ENTER) \
					"I was so happy for you!" SS_TAP(X_ENTER) \
					" " SS_TAP(X_ENTER) \
					"Now these points of data" SS_TAP(X_ENTER) \
					"make a beautiful line." SS_TAP(X_ENTER) \
					"And we're out of beta." SS_TAP(X_ENTER) \
					"We're releasing on time." SS_TAP(X_ENTER) \
					"So I'm GLaD. I got burned." SS_TAP(X_ENTER) \
					"Think of all the things we learned" SS_TAP(X_ENTER) \
					"for the people who are" SS_TAP(X_ENTER) \
					"still alive." SS_TAP(X_ENTER) \
					" " SS_TAP(X_ENTER) \
					"Go ahead and leave me." SS_TAP(X_ENTER) \
					"I think I prefer to stay inside." SS_TAP(X_ENTER) \
					"Maybe you'll find someone else" SS_TAP(X_ENTER) \
					"to help you." SS_TAP(X_ENTER) \
					"Maybe Black Mesa..." SS_TAP(X_ENTER) \
					"THAT WAS A JOKE. Haha. FAT CHANCE." SS_TAP(X_ENTER) \
					"Anyway, this cake is great." SS_TAP(X_ENTER) \
					"It's so delicious and moist." SS_TAP(X_ENTER) \
					" " SS_TAP(X_ENTER) \
					"Look at me still talking" SS_TAP(X_ENTER) \
					"when there's science to do." SS_TAP(X_ENTER) \
					"When I look out there," SS_TAP(X_ENTER) \
					"it makes me GLaD I'm not you." SS_TAP(X_ENTER) \
					"I've experiments to run." SS_TAP(X_ENTER) \
					"There is research to be done." SS_TAP(X_ENTER) \
					"On the people who are" SS_TAP(X_ENTER) \
					"still alive." SS_TAP(X_ENTER) \
					" " SS_TAP(X_ENTER) \
					"And believe me I am" SS_TAP(X_ENTER) \
					"still alive." SS_TAP(X_ENTER) \
					"I'm doing science and I'm" SS_TAP(X_ENTER) \
					"still alive." SS_TAP(X_ENTER) \
					"I feel FANTASTIC and I'm" SS_TAP(X_ENTER) \
					"still alive." SS_TAP(X_ENTER) \
					"While you're dying I'll be" SS_TAP(X_ENTER) \
					"still alive." SS_TAP(X_ENTER) \
					"And when you're dead I will be" SS_TAP(X_ENTER) \
					"still alive." SS_TAP(X_ENTER) \
					" " SS_TAP(X_ENTER) \
					"STILL ALIVE" SS_TAP(X_ENTER) \
					"Still alive" SS_TAP(X_ENTER) \
				);
			}
			break;
	}

	if (orange_portal && blue_portal) {
		led_sethsv(white);
		reset_keyboard();
		return false;
	}

	return true;
}
