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
#pragma once

/* Usage:
 *
 * void raw_hid_receive(uint8_t *data, uint8_t length) {
 *     raw_hid_receive_identify(data, length); // calls raw_identify_user()
 * }
 */

enum raw_identify {
    ID_DEFAULT = 0x00000000,
    ID_LINUX   = 0x4C4E5800,
    ID_WINDOWS = 0x57494E00,
    ID_APPLE   = 0x41504C00,
    ID_BSD     = 0x42534400,
};

static void raw_identify_user(enum raw_identify id);

static inline void raw_hid_receive_identify(uint8_t *data, uint8_t length) {
    if (length >= 2) {
        if (data[0] == 0x00 && data[1] == 0x01) { /* Identify OS */
            if (length >= 6) {
                raw_identify_user(((uint32_t)data[2] << 24) | ((uint32_t)data[3] << 16) | ((uint32_t)data[4] << 8) | data[5]);
            }
        }
    }
}
