/* Copyright 2025 Simon Arlott
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

#define MEET_TOG_MIC    LCTL(KC_D)
#define MEET_TOG_VIDEO  LCTL(KC_E)
#define MEET_TOG_HAND   LCTL(LALT(KC_H))
#define TEAMS_TOG_MIC   LCTL(LSFT(KC_M))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT( /* Base */
    //MEET_TOG_MIC, MEET_TOG_VIDEO, MEET_TOG_HAND, TEAMS_TOG_MIC
    TEAMS_TOG_MIC, MEET_TOG_HAND, MEET_TOG_VIDEO, MEET_TOG_MIC
  ),
};
