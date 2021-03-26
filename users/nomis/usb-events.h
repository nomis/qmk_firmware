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
 * void housekeeping_task_user(void) {
 *     usb_event_check(); // calls usb_event_user()
 * }
 */

enum usb_event {
    USB_EVT_DISCONNECTED,
    USB_EVT_CONNECTED,
    USB_EVT_SUSPENDED,
};

static void usb_event_user(enum usb_event event);

static inline void usb_event_check(void);

#ifdef PROTOCOL_LUFA
#    include <LUFA/Drivers/USB/USB.h>

static inline void usb_event_check(void) {
    static enum USB_Device_States_t previous_state = ~0;
    enum USB_Device_States_t current_state = USB_DeviceState;

    if (previous_state != current_state) {
        switch (current_state) {
        case DEVICE_STATE_Unattached ... DEVICE_STATE_Addressed:
            switch (previous_state) {
            case DEVICE_STATE_Unattached ... DEVICE_STATE_Addressed:
                /* Avoid sending multiple events */
                break;

            default:
                usb_event_user(USB_EVT_DISCONNECTED);
            }
            break;

        case DEVICE_STATE_Configured:
            usb_event_user(USB_EVT_CONNECTED);
            break;

        case DEVICE_STATE_Suspended:
            usb_event_user(USB_EVT_SUSPENDED);
            break;
        }
    }

    previous_state = current_state;
}
#endif
