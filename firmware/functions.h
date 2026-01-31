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
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <lvgl.h>
#include "ui/ui.h"
#include <Arduino.h>

enum APMode {
    MODE_COMPASS,
    MODE_NAV,
	MODE_GPS,
    MODE_WIND,
    MODE_TRUE_WIND,
    MODE_UNKNOWN
};


// Declare your functions
void updateCompassPosition();
void updateCmd();
void setWifiIcon();
void engagedText();
void activeMode() ;
void setRudderSlider();
void displayLog(const String &msg);

void ui_color_swap_init(void);
void ui_light_theme(void);
void ui_dark_theme(void);
void ui_apply_swap_globally_once(void);

#endif
