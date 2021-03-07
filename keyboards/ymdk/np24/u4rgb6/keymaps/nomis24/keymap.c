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
	L_TOP,
	L_LEFT,
	L_RIGHT,
	L_BOTTOM,
	L_TOP_LEFT,
	L_TOP_RIGHT,
	L_BOTTOM_LEFT,
	L_BOTTOM_RIGHT,
	L_BLOCK,
	L_ALT_CLIPBOARD,
};

enum custom_keycodes {
	CK_OS = SAFE_RANGE,
	CK_CLIP,
};

enum unicode_names {
	UNICODE_SAFE_RANGE,
};

#define MAKE_EMOJI(__id, \
	__top_left,    __x0, __top,    __x1, __top_right,    __x2, \
	__left,        __x3, __middle, __x4, __right,        __x5, \
	__bottom_left, __x6, __bottom, __x7, __bottom_right, __x8) \
	[UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 0] = 0x ## __top_left, \
	[UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 1] = 0x ## __top, \
	[UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 2] = 0x ## __top_right, \
	[UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 3] = 0x ## __left, \
	[UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 4] = 0x ## __middle, \
	[UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 5] = 0x ## __right, \
	[UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 6] = 0x ## __bottom_left, \
	[UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 7] = 0x ## __bottom, \
	[UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 8] = 0x ## __bottom_right \

#define E_TL(__id) X(UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 0)
#define E_T(__id)  X(UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 1)
#define E_TR(__id) X(UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 2)
#define E_L(__id)  X(UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 3)
#define E_M(__id)  X(UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 4)
#define E_R(__id)  X(UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 5)
#define E_BL(__id) X(UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 6)
#define E_B(__id)  X(UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 7)
#define E_BR(__id) X(UNICODE_SAFE_RANGE + ((__id) - 1) * 9 + 8)

const uint32_t PROGMEM unicode_map[] = {
	MAKE_EMOJI(1,
		1F973, 🥳, 1F601, 😁, 1F92A, 🤪,
		1F600, 😀, 1F642, 🙂, 1F602, 😂,
		1F610, 😐, 1F643, 🙃, 1F61B, 😛),
	MAKE_EMOJI(2,
		1F92D, 🤭, 1F928, 🤨, 1F914, 🤔,
		1F60F, 😏, 1F644, 🙄, 1F612, 😒,
		1F92B, 🤫, 1F609, 😉, 1F92F, 🤯),
	MAKE_EMOJI(3,
		1F607, 😇, 1F924, 🤤, 1F608, 😈,
		1F62C, 😬, 1F62E, 😮, 1F60D, 😍,
		1F975, 🥵, 1F97A, 🥺, 1F976, 🥶),
	MAKE_EMOJI(4,
		1F614, 😔, 1F62D, 😭, 1F60C, 😌,
		1F621, 😡,  2639, ☹️, 1F62B, 😫,
		1F631, 😱, 1F623, 😣, 1F92E, 🤮),
	MAKE_EMOJI(5,
		1F920, 🤠, 1F44D, 👍, 1F60B, 😋,
		1F937, 🤷, 1F926, 🤦, 1F481, 💁,
		1F921, 🤡, 1F44E, 👎, 1F624, 😤),
	MAKE_EMOJI(6,
		 2622, ☢️, 1F389, 🎉,  2623, ☣️,
		1F648, 🙈, 1F649, 🙉, 1F64A, 🙊,
		1F382, 🎂, 1F38A, 🎊, 00000, XX),
	MAKE_EMOJI(7,
		1F1EC, 🇬🇧, 1F3F4, 🏴󠁧󠁢󠁳󠁣󠁴󠁿, 1F1EA, 🇪🇺,
		1F3F4, 🏴‍☠️, 1F3F3, 🏳️‍🌈, 1F3F3, 🏳️‍⚧️,
		1F1FA, 🇺🇳, 1F1E6, 🇦🇺, 1F1F3, 🇳🇿),
	MAKE_EMOJI(8,
		 2196, ↖️,  2B06, ⬆️,  2197, ↗️,
		 2B05, ⬅️, 1F4A9, 💩,  27A1, ➡️,
		 2199, ↙️,  2B07, ⬇️,  2198, ↘️),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	/* Keymap L_BASE: Base Layer (Default Layer)
	 * ,-----------------------------------.
	 * | Portal |   ⬆️   |   (λ)  | Portal |
	 * |⬅️      | Stience| Orange |      ➡️|
	 * | Orange |  Blue  |   ⬇️   |  Blue  |
	 * |-----------------------------------|
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |   ⭕   |   ⭕   |   ⭕   |   ⭕   |
	 * |   1    |   2    |   3    |   4    |
	 * |-----------------------------------|
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |   ⭕   |   ⭕   |   ⭕   |   ⭕   |
	 * |   5    |   6    |   7    |   8    |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * | Toggle |        |        |        |
	 * |Alt Clip|  Cut   |  Copy  | Paste  |
	 * | Layer  |  C-x   |  C-c   | C-v    |
	 * |-----------------------------------|
	 * |  Teams |  Teams |  Zoom  |  Zoom  |
	 * | Toggle | Toggle | Toggle | Toggle |
	 * |  Mute  |  Video |  Video |  Mute  |
	 * `-----------------------------------'
	 */
	[L_BASE] = LAYOUT_ortho_6x4(
		MO(L_LEFT),                MO(L_TOP),             MO(L_BOTTOM),          MO(L_RIGHT),
		E_M(1),                    E_M(2),                E_M(3),                E_M(4),
		E_M(5),                    E_M(6),                E_M(7),                E_M(8),
		KC_NO,                     KC_NO,                 KC_NO,                 KC_NO,
		CK_CLIP,                   LCTL(KC_X),            LCTL(KC_C),            LCTL(KC_V),
		LCTL(LSFT(KC_M)),          LCTL(LSFT(KC_O)),      LALT(KC_V),            LALT(KC_A)
	),
	/* Keymap L_TOP: Stience
	 * ,-----------------------------------.
	 * |↖️      |   ⬆️   |        |      ↗️|
	 * | Portal | Stience| Switch | Portal |
	 * | Orange |  Blue  |   OS   |  Blue  |
	 * |-----------------------------------|
	 * |   ⬆️   |   ⬆️   |   ⬆️   |   ⬆️   |
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |   1    |   2    |   3    |   4    |
	 * |-----------------------------------|
	 * |   ⬆️   |   ⬆️   |   ⬆️   |   ⬆️   |
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |   5    |   6    |   7    |   8    |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * `-----------------------------------'
	 */
	[L_TOP] = LAYOUT_ortho_6x4(
		_______,               _______,               CK_OS,                 _______,
		E_T(1),                E_T(2),                E_T(3),                E_T(4),
		E_T(5),                E_T(6),                E_T(7),                E_T(8),
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______
	),
	/* Keymap L_LEFT: Orange Portal
	 * ,-----------------------------------.
	 * | Portal |↖️      |   (λ)  |        |
	 * |⬅️      | Stience| Orange |        |
	 * | Orange |  Blue  |↙️      |        |
	 * |-----------------------------------|
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |⬅️      |⬅️      |⬅️      |⬅️      |
	 * |   1    |   2    |   3    |   4    |
	 * |-----------------------------------|
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |⬅️      |⬅️      |⬅️      |⬅️      |
	 * |   5    |   6    |   7    |   8    |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * `-----------------------------------'
	 */
	[L_LEFT] = LAYOUT_ortho_6x4(
		_______,               _______,               _______,               _______,
		E_L(1),                E_L(2),                E_L(3),                E_L(4),
		E_L(5),                E_L(6),                E_L(7),                E_L(8),
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______
	),
	/* Keymap L_RIGHT: Blue Portal
	 * ,-----------------------------------.
	 * |        |      ↗️|   (λ)  | Portal |
	 * |        | Stience| Orange |      ➡️|
	 * |        |  Blue  |      ↘️|  Blue  |
	 * |-----------------------------------|
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |      ➡️|      ➡️|      ➡️|      ➡️|
	 * |   1    |   2    |   3    |   4    |
	 * |-----------------------------------|
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |      ➡️|      ➡️|      ➡️|      ➡️|
	 * |   5    |   6    |   7    |   8    |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * `-----------------------------------'
	 */
	[L_RIGHT] = LAYOUT_ortho_6x4(
		_______,               _______,               _______,               _______,
		E_R(1),                E_R(2),                E_R(3),                E_R(4),
		E_R(5),                E_R(6),                E_R(7),                E_R(8),
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______
	),
	/* Keymap L_BOTTOM: Half Life
	 * ,-----------------------------------.
	 * | Portal |        |   (λ)  | Portal |
	 * | Orange | Switch | Orange |  Blue  |
	 * |↙️      |   OS   |   ⬇️   |      ↘️|
	 * |-----------------------------------|
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |   1    |   2    |   3    |   4    |
	 * |   ⬇️   |   ⬇️   |   ⬇️   |   ⬇️   |
	 * |-----------------------------------|
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |   5    |   6    |   7    |   8    |
	 * |   ⬇️   |   ⬇️   |   ⬇️   |   ⬇️   |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * `-----------------------------------'
	 */
	[L_BOTTOM] = LAYOUT_ortho_6x4(
		_______,               CK_OS,                 _______,               _______,
		E_B(1),                E_B(2),                E_B(3),                E_B(4),
		E_B(5),                E_B(6),                E_B(7),                E_B(8),
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______
	),
	/* Keymap L_TOP_LEFT: Orange Portal + Stience
	 * ,-----------------------------------.
	 * |↖️      |↖️      |                 |
	 * | Portal | Stience|      Reset      |
	 * | Orange |  Blue  |                 |
	 * |-----------------------------------|
	 * |↖️      |↖️      |↖️      |↖️      |
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |   1    |   2    |   3    |   4    |
	 * |-----------------------------------|
	 * |↖️      |↖️      |↖️      |↖️      |
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |   5    |   6    |   7    |   8    |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * `-----------------------------------'
	 */
	[L_TOP_LEFT] = LAYOUT_ortho_6x4(
		_______,               _______,               MO(L_BOTTOM),          MO(L_RIGHT),
		E_TL(1),               E_TL(2),               E_TL(3),               E_TL(4),
		E_TL(5),               E_TL(6),               E_TL(7),               E_TL(8),
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______
	),
	/* Keymap L_TOP_RIGHT: Stience + Blue Portal
	 * ,-----------------------------------.
	 * |        |      ↗️|        |      ↗️|
	 * | Reset+ | Stience| +Reset | Portal |
	 * |        |  Blue  |        |  Blue  |
	 * |-----------------------------------|
	 * |      ↗️|      ↗️|      ↗️|      ↗️|
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |   1    |   2    |   3    |   4    |
	 * |-----------------------------------|
	 * |      ↗️|      ↗️|      ↗️|      ↗️|
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |   5    |   6    |   7    |   8    |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * `-----------------------------------'
	 */
	[L_TOP_RIGHT] = LAYOUT_ortho_6x4(
		MO(L_LEFT),            _______,               MO(L_BOTTOM),          _______,
		E_TR(1),               E_TR(2),               E_TR(3),               E_TR(4),
		E_TR(5),               E_TR(6),               E_TR(7),               E_TR(8),
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______
	),
	/* Keymap L_BOTTOM_LEFT: Orange Portal + Half Life
	 * ,-----------------------------------.
	 * | Portal |        |   (λ)  |        |
	 * | Orange | Reset+ | Orange | +Reset |
	 * |↙️      |        |↙️      |        |
	 * |-----------------------------------|
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |   1    |   2    |   3    |   4    |
	 * |↙️      |↙️      |↙️      |↙️      |
	 * |-----------------------------------|
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |   5    |   6    |   7    |   8    |
	 * |↙️      |↙️      |↙️      |↙️      |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * `-----------------------------------'
	 */
	[L_BOTTOM_LEFT] = LAYOUT_ortho_6x4(
		_______,               MO(L_TOP),             _______,               MO(L_RIGHT),
		E_BL(1),               E_BL(2),               E_BL(3),               E_BL(4),
		E_BL(5),               E_BL(6),               E_BL(7),               E_BL(8),
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______
	),
	/* Keymap L_BOTTOM_RIGHT: Half Life + Blue Portal
	 * ,-----------------------------------.
	 * |                 |   (λ)  | Portal |
	 * |      Reset      | Orange |  Blue  |
	 * |                 |      ↘️|      ↘️|
	 * |-----------------------------------|
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |   1    |   2    |   3    |   4    |
	 * |      ↘️|      ↘️|      ↘️|      ↘️|
	 * |-----------------------------------|
	 * | Emoji  | Emoji  | Emoji  | Emoji  |
	 * |   5    |   6    |   7    |   8    |
	 * |      ↘️|      ↘️|      ↘️|      ↘️|
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * `-----------------------------------'
	 */
	[L_BOTTOM_RIGHT] = LAYOUT_ortho_6x4(
		MO(L_LEFT),            MO(L_TOP),             _______,               _______,
		E_BR(1),               E_BR(2),               E_BR(3),               E_BR(4),
		E_BR(5),               E_BR(6),               E_BR(7),               E_BR(8),
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______
	),
	/* Keymap L_BLOCK: Blocking Layer
	 * ,-----------------------------------.
	 * | Portal | Stience|   (λ)  | Portal |
	 * |        |        |        |        |
	 * | Orange |  Blue  | Orange |  Blue  |
	 * |-----------------------------------|
	 * |XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|
	 * |XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|
	 * |XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|
	 * |-----------------------------------|
	 * |XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|
	 * |XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|
	 * |XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * `-----------------------------------'
	 */
	[L_BLOCK] = LAYOUT_ortho_6x4(
		_______,               _______,               _______,               _______,
		XXXXXXX,               XXXXXXX,               XXXXXXX,               XXXXXXX,
		XXXXXXX,               XXXXXXX,               XXXXXXX,               XXXXXXX,
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______
	),
	/* Keymap L_ALT_CLIPBOARD: Alt Clipboard Layer
	 * ,-----------------------------------.
	 * | Portal | Stience|   (λ)  | Portal |
	 * |        |        |        |        |
	 * | Orange |  Blue  | Orange |  Blue  |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * | Toggle |        |        |        |
	 * |Alt Clip|  Cut   |  Copy  | Paste  |
	 * | Layer  |  S-Del |  C-Ins | S-Ins  |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * `-----------------------------------'
	 */
	[L_ALT_CLIPBOARD] = LAYOUT_ortho_6x4(
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______,
		_______,               _______,               _______,               _______,
		_______,               LSFT(KC_DEL),          LCTL(KC_INS),          LSFT(KC_INS),
		_______,               _______,               _______,               _______
	),
#if 0
	/* Keymap L_EMPTY: Empty Layer
	 * ,-----------------------------------.
	 * | Portal | Stience|   (λ)  | Portal |
	 * |        |        |        |        |
	 * | Orange |  Blue  | Orange |  Blue  |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |-----------------------------------|
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * |        |        |        |        |
	 * `-----------------------------------'
	 */
	[L_EMPTY] = LAYOUT_ortho_6x4(
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

static void led_sethsv_at(HSV hsv, uint8_t idx) {
	sethsv(hsv.h, hsv.s, hsv.v, (LED_TYPE *)&led[idx]);
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

	led_sethsv(white);
	wait_ms(500);
	led_sethsv(black);
}

uint32_t layer_state_set_user(uint32_t state) {
	uint8_t layers = 0;

	state = update_tri_layer_state(state, L_TOP, L_LEFT, L_TOP_LEFT);
	state = update_tri_layer_state(state, L_TOP, L_RIGHT, L_TOP_RIGHT);
	state = update_tri_layer_state(state, L_BOTTOM, L_LEFT, L_BOTTOM_LEFT);
	state = update_tri_layer_state(state, L_BOTTOM, L_RIGHT, L_BOTTOM_RIGHT);

	layers += layer_state_cmp(state, L_LEFT) ? 1 : 0;
	layers += layer_state_cmp(state, L_TOP) ? 1 : 0;
	layers += layer_state_cmp(state, L_BOTTOM) ? 1 : 0;
	layers += layer_state_cmp(state, L_RIGHT) ? 1 : 0;

	if (layers >= 3) {
		state |= (1UL << L_BLOCK);
	} else {
		state &= ~(1UL << L_BLOCK);
	}

	if (layers == 4) {
		led_sethsv(green);
		reset_keyboard();
	} else if (layers == 3) {
		led_sethsv(red);
	} else if (layer_state_cmp(state, L_TOP_LEFT)) {
		led_sethsv_at(red, 0);
		led_sethsv_at(white, 1);
		led_sethsv_at(black, 2);
		led_sethsv_at(black, 3);
		led_sethsv_at(black, 4);
		led_sethsv_at(white, 5);
		rgblight_set();
	} else if (layer_state_cmp(state, L_TOP_RIGHT)) {
		led_sethsv_at(white, 0);
		led_sethsv_at(red, 1);
		led_sethsv_at(white, 2);
		led_sethsv_at(black, 3);
		led_sethsv_at(black, 4);
		led_sethsv_at(black, 5);
		rgblight_set();
	} else if (layer_state_cmp(state, L_BOTTOM_LEFT)) {
		led_sethsv_at(black, 0);
		led_sethsv_at(black, 1);
		led_sethsv_at(black, 2);
		led_sethsv_at(white, 3);
		led_sethsv_at(red, 4);
		led_sethsv_at(white, 5);
		rgblight_set();
	} else if (layer_state_cmp(state, L_BOTTOM_RIGHT)) {
		led_sethsv_at(black, 0);
		led_sethsv_at(black, 1);
		led_sethsv_at(white, 2);
		led_sethsv_at(red, 3);
		led_sethsv_at(white, 4);
		led_sethsv_at(black, 5);
		rgblight_set();
	} else if (layer_state_cmp(state, L_TOP)) {
		led_sethsv_at(white, 0);
		led_sethsv_at(white, 1);
		led_sethsv_at(black, 2);
		led_sethsv_at(black, 3);
		led_sethsv_at(black, 4);
		led_sethsv_at(black, 5);
		rgblight_set();
	} else if (layer_state_cmp(state, L_LEFT)) {
		led_sethsv_at(white, 0);
		led_sethsv_at(black, 1);
		led_sethsv_at(black, 2);
		led_sethsv_at(black, 3);
		led_sethsv_at(white, 4);
		led_sethsv_at(white, 5);
		rgblight_set();
	} else if (layer_state_cmp(state, L_RIGHT)) {
		led_sethsv_at(black, 0);
		led_sethsv_at(white, 1);
		led_sethsv_at(white, 2);
		led_sethsv_at(white, 3);
		led_sethsv_at(black, 4);
		led_sethsv_at(black, 5);
		rgblight_set();
	} else if (layer_state_cmp(state, L_BOTTOM)) {
		led_sethsv_at(black, 0);
		led_sethsv_at(black, 1);
		led_sethsv_at(black, 2);
		led_sethsv_at(white, 3);
		led_sethsv_at(white, 4);
		led_sethsv_at(black, 5);
		rgblight_set();
	} else {
		led_sethsv(black);
	}

	return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {
		switch (keycode) {
		case CK_OS:
			switch(get_unicode_input_mode()) {
			case UC_LNX:
				set_unicode_input_mode(UC_WINC);
				led_sethsv(blue);
				break;

			case UC_WINC:
				set_unicode_input_mode(UC_LNX);
				led_sethsv(orange);
				break;
			}
			break;

		case CK_CLIP:
			if (layer_state_is(L_ALT_CLIPBOARD)) {
				layer_off(L_ALT_CLIPBOARD);
				led_sethsv(green);
			} else {
				layer_on(L_ALT_CLIPBOARD);
				led_sethsv(red);
			}
			break;
		}
	} else {
		switch (keycode) {
		case CK_OS:
		case CK_CLIP:
			led_sethsv(black);
			break;
		}
	}

	return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
		if (record->event.pressed) {
		switch (keycode) {
		case E_M(4): // ☹️
		case E_TL(6): // ☢️
		case E_TR(6): // ☣️
		case E_TL(8) ... E_L(8): // ↖️⬆️↗️⬅️
		case E_R(8) ... E_BR(8): // ➡️↙️⬇️↘️
			send_unicode_hex_string("FE0F");
			break;

		case E_TL(7): // 🇬🇧
			send_unicode_hex_string("1F1E7");
			break;

		case E_T(7): // 🏴󠁧󠁢󠁳󠁣󠁴󠁿
			send_unicode_hex_string("E0067 E0062 E0073 E0063 E0074 E007F");
			break;

		case E_TR(7): // 🇪🇺
		case E_B(7): // 🇦🇺
			send_unicode_hex_string("1F1FA");
			break;

		case E_L(7): // 🏴‍☠️
			send_unicode_hex_string("200D 2620 FE0F");
			break;

		case E_M(7): // 🏳️‍🌈
			send_unicode_hex_string("FE0F 200D 1F308");
			break;

		case E_R(7): // 🏳️‍⚧️
			send_unicode_hex_string("FE0F 200D 26A7 FE0F");
			break;

		case E_BL(7): // 🇺🇳
			send_unicode_hex_string("1F1F3");
			break;

		case E_BR(7): // 🇳🇿
			send_unicode_hex_string("1F1FF");
			break;
		}
	}
}
