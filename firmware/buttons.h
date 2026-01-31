/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#define TOTAL_BUTTONS 9

// -------- PIN DEFINITIONS --------
#define BTN_PORT1      4
#define BTN_STBD1      5
#define BTN_PORT10     6
#define BTN_STBD10     7
#define BTN_MODE       15
#define BTN_INFO       18
#define BTN_AUTO       17
#define BTN_STANDBY    16
#define BTN_BACKLIGHT  8

// -------- BUTTON EVENTS --------
typedef enum {
    BTN_EVENT_NONE = 0,
    BTN_EVENT_PORT1,
    BTN_EVENT_STBD1,
    BTN_EVENT_PORT10,
    BTN_EVENT_STBD10,
    BTN_EVENT_MODE,
    BTN_EVENT_INFO,
    BTN_EVENT_AUTO,
    BTN_EVENT_AUTO_LONG,   // <--- NEW: long press for AUTO (3s)
    BTN_EVENT_STANDBY,
    BTN_EVENT_BACKLIGHT,

    // <--- NEW: long press for BACKLIGHT
    BTN_EVENT_BACKLIGHT_LONG,

    // <--- NEW: long press for BACKLIGHT + MODE together
    BTN_EVENT_BACKLIGHT_MODE_LONG

} ButtonEvent;

// Handler signature (kept for compatibility)
typedef void (*ButtonHandler)();

// Button state container
typedef struct {
    uint8_t pin;
    bool lastState;
    unsigned long lastChangeTime;
    ButtonEvent eventID;     // which event this button generates (short press)

    // --- Long-press support (used only for BTN_AUTO) ---
    bool isDown;             // true while button is held down
    unsigned long pressStartTime; // when it was pressed (ms)
    bool longPressSent;      // true once long-press event has been sent
} ButtonState;

extern ButtonState buttons[TOTAL_BUTTONS];

// Queue API
void setupButtons();
void pollButtons();
void processButtonEvents();   // call this in loop()
void enterInfoScreen();
void exitInfoScreen();
void updateInfoScreen();

// Queue internals
bool eventAvailable();
ButtonEvent popButtonEvent();

#endif
