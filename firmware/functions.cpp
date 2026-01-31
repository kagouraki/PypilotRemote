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
#include "functions.h"
#include <lvgl.h>
#include "ui/ui.h"     // required to see ui objects like ui_compass, ui_lbl_degree
#include <Arduino.h>
#include <cstdio>
#include "pypilot_client.h"
#include "wireless.h"
#include <math.h>

void updateCompassPosition() {
	int imuReading = round(readNumber("imu.heading"));
	const int indicatorX = 360;   // your center indicator position
	const int pxPerDeg = 2;
	// Compute X position
	int newX = indicatorX - (imuReading * pxPerDeg);

	// Apply to the strip
	lv_obj_set_x(ui_compassStrip, newX);

}

APMode getAPMode() {

	String mode = readString("ap.mode");

	if (mode == "compass")
		return MODE_COMPASS;
	if (mode == "nav")
		return MODE_NAV;
	if (mode == "gps")
		return MODE_GPS;
	if (mode == "wind")
		return MODE_WIND;
	if (mode == "true wind")
		return MODE_TRUE_WIND;

	return MODE_UNKNOWN;
}

void activeMode() {
	static APMode lastMode = MODE_UNKNOWN;
	APMode mode = getAPMode();

	if (lastMode == mode) {
		return;
	}
	lastMode = mode;

	switch (mode) {

	case MODE_COMPASS:
		lv_label_set_text(ui_activeMode, "Compass");
		break;
	case MODE_NAV:
		lv_label_set_text(ui_activeMode, "Navigation");
		break;
	case MODE_GPS:
		lv_label_set_text(ui_activeMode, "Gps");
		break;
	case MODE_WIND:
		lv_label_set_text(ui_activeMode, "Wind");
		break;
	case MODE_TRUE_WIND: {
		lv_label_set_text(ui_activeMode, "True Wind");
		break;
	}

	default:
		lv_label_set_text(ui_activeMode, "Unknown");
		break;
	}

}

void updateCmd() {
	static APMode lastMode = MODE_UNKNOWN;
	static int lastHeadingCommand = 9999;

	char buf[16];

	// ------------------------------
	// Determine mode and heading source
	// ------------------------------
	APMode mode = getAPMode();

	int headingCommand;
	bool enabled = readBool("ap.enabled");

	if (enabled) {
		headingCommand = round(readNumber("ap.heading_command"));
	} else {
		headingCommand = round(readNumber("imu.heading"));
	}

	// ------------------------------
	// Change detection (performance)
	// ------------------------------
	if (lastMode == mode && lastHeadingCommand == headingCommand) {
		return;
	}
	lastMode = mode;
	lastHeadingCommand = headingCommand;

	// ------------------------------
	// AP disabled → IMU display only
	// ------------------------------
	if (!enabled) {
		lv_obj_set_style_text_color(ui_cmd, lv_color_hex(0xFFFFFF),
				LV_PART_MAIN);

		snprintf(buf, sizeof(buf), "%03d", headingCommand);
		lv_label_set_text(ui_cmd, buf);
		return;
	}
	// ------------------------------
	// AP enabled → Mode-dependent display
	// ------------------------------
	switch (mode) {

	// ----- COMPASS & NAV -----
	case MODE_COMPASS:
	case MODE_NAV:
	case MODE_GPS:
		lv_obj_set_style_text_color(ui_cmd, lv_color_hex(0xFFFFFF),
				LV_PART_MAIN);

		snprintf(buf, sizeof(buf), "%03d", headingCommand);
		lv_label_set_text(ui_cmd, buf);
		break;

		// ----- WIND & TRUE WIND -----
	case MODE_WIND:
	case MODE_TRUE_WIND: {
		int displayValue = abs(headingCommand);

		if (headingCommand < 0) {
			lv_obj_set_style_text_color(ui_cmd, lv_color_hex(0xFF0000),
					LV_PART_MAIN);  // red
		} else if (headingCommand > 0) {
			lv_obj_set_style_text_color(ui_cmd, lv_color_hex(0x00FF00),
					LV_PART_MAIN);  // green
		} else {
			lv_obj_set_style_text_color(ui_cmd, lv_color_hex(0xFFFFFF),
					LV_PART_MAIN);  // white
		}

		snprintf(buf, sizeof(buf), "%03d", displayValue);
		lv_label_set_text(ui_cmd, buf);
		break;
	}

		// ----- UNKNOWN MODE -----
	default:
		lv_obj_set_style_text_color(ui_cmd, lv_color_hex(0xFFFFFF),
				LV_PART_MAIN);
		lv_label_set_text(ui_cmd, "999");
		break;
	}
}

void setWifiIcon() {

	static WiFiState lastWifiState = WIFI_IDLE;
	static bool lastIsTcpConnected = false;

	if ((lastWifiState == wifiState)
			&& (lastIsTcpConnected == tcp.isTcpConnected)) {
		return;
	}
	lastWifiState = wifiState;
	lastIsTcpConnected = tcp.isTcpConnected;

	lv_anim_del_all();
	lv_obj_set_style_opa(ui_wifiRed, 0, LV_PART_MAIN); //set opacity for image
	lv_obj_set_style_opa(ui_wifiYellow, 0, LV_PART_MAIN); //set opacity for image
	lv_obj_set_style_opa(ui_wifiGreen, 0, LV_PART_MAIN); //set opacity for image

	switch (wifiState) {

	case WIFI_PORTAL:
		flashWifi_Animation(ui_wifiYellow, 0); //flash Yellow
		break;

	case WIFI_CONNECTED:
		if (tcp.isTcpConnected) {
			lv_obj_set_style_opa(ui_wifiGreen, 255, LV_PART_MAIN); //solid Green
			break;
		}
		flashWifi_Animation(ui_wifiGreen, 0); //flash Green
		break;

	case WIFI_TRY_CONNECT:
	case WIFI_RETRY_WAIT:
		flashWifi_Animation(ui_wifiRed, 0); //flash Red
		break;
	}
}

void engagedText() {
	bool engaged = readBool("ap.enabled");
	if (engaged) {
		lv_obj_clear_flag(ui_engaged, LV_OBJ_FLAG_HIDDEN);

	} else {
		lv_obj_add_flag(ui_engaged, LV_OBJ_FLAG_HIDDEN);
	}
}

void setRudderSlider() {
	static int lastAngle = 999;
	static int lastRange = 999;
	static int lastSign = 0;   // -1 red, +1 green, 0 unknown

	if (!ui_rudder)
		return;    // Safety: widget not ready yet

	int rudderRange = round(readNumber("rudder.range"));
	if (rudderRange <= 0)
		return;   // Prevent LVGL crash

	// Update range when necessary
	if (rudderRange != lastRange) {
		lv_slider_set_range(ui_rudder, -rudderRange, rudderRange);
		lastRange = rudderRange;
	}

	int rudderAngle = round(readNumber("rudder.angle"));

	// Clamp to avoid LVGL panic
	if (rudderAngle > rudderRange)
		rudderAngle = rudderRange;
	if (rudderAngle < -rudderRange)
		rudderAngle = -rudderRange;

	// Change detection
	if (rudderAngle == lastAngle)
		return;
	lastAngle = rudderAngle;

	// Update slider value
	lv_slider_set_value(ui_rudder, rudderAngle, LV_ANIM_OFF);

	// Determine new sign
	int newSign = (rudderAngle > 0) ? 1 : (rudderAngle < 0 ? -1 : 0);

	// Update color ONLY if sign changed
	if (newSign != lastSign) {
		lastSign = newSign;

		if (newSign >= 0) {
			lv_obj_set_style_bg_color(ui_rudder, lv_color_hex(0x00BD3C),
					LV_PART_INDICATOR);
		} else {
			lv_obj_set_style_bg_color(ui_rudder, lv_color_hex(0xBD0000),
					LV_PART_INDICATOR);
		}
	}
}

void displayLog(const String &msg) {
	Serial.println(msg);

	String line = msg + "\n";              // Proper newline
	lv_label_ins_text(ui_debugLog, LV_LABEL_POS_LAST, line.c_str()); // Convert to const char *
}


/* ========= Global color swap (LVGL v8) ========= */

lv_style_t style_color_swap;
lv_color_filter_dsc_t color_swap_filter;
//static bool g_color_swap_enabled = false;

/* Swap colors callback */
static lv_color_t color_swap_cb(const lv_color_filter_dsc_t *dsc,
                                lv_color_t color,
                                lv_opa_t opa)
{
    if(color.full == 0x0000) {
        return lv_color_white();
    }

    if(color.full == 0xFFFF) {
        return lv_color_black();
    }

    if(color.full == lv_color_hex(0xC5C5C5).full) {
        return lv_color_hex(0x393939);
    }

    if(color.full == lv_color_hex(0x393939).full) {
        return lv_color_hex(0xC5C5C5);
    }

    return color;
}

void ui_color_swap_init(void)
{
    lv_style_init(&style_color_swap);
    lv_color_filter_dsc_init(&color_swap_filter, color_swap_cb);
    lv_style_set_color_filter_dsc(&style_color_swap, &color_swap_filter);

    /* Start disabled */
    lv_style_set_color_filter_opa(&style_color_swap, LV_OPA_0);
}

void ui_light_theme(void)
{
    lv_style_set_color_filter_opa(&style_color_swap, LV_OPA_100);
    lv_obj_refresh_style(lv_scr_act(), LV_PART_ANY, LV_STYLE_PROP_ANY);

    lv_img_set_src(ui_fade, &ui_img_fadeinv_png);
    lv_img_set_src(ui_compassStrip, &ui_img_compassinv_png);
    lv_img_set_src(ui_indicator, &ui_img_indicatorinv_png);
    lv_img_set_src(ui_degSymbol, &ui_img_degreesinv_png);
    lv_img_set_src(ui_rudderSymbol, &ui_img_wheelinv_png);

    lv_obj_set_style_bg_img_src(ui_Bar2, &ui_img_rudderinv_png,
                                LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_img_src(ui_rudder, &ui_img_rudderlineinv_png,
                                LV_PART_KNOB | LV_STATE_DEFAULT);

    lv_img_set_src(ui_redgradient, &ui_img_redinv_png);
    lv_img_set_src(ui_greengradient, &ui_img_greeninv_png);
}


void ui_dark_theme(void)
{
    lv_style_set_color_filter_opa(&style_color_swap, LV_OPA_0);
    lv_obj_refresh_style(lv_scr_act(), LV_PART_ANY, LV_STYLE_PROP_ANY);

    lv_img_set_src(ui_fade, &ui_img_fade_png);
    lv_img_set_src(ui_compassStrip, &ui_img_compass_png);
    lv_img_set_src(ui_indicator, &ui_img_indicator_png);
    lv_img_set_src(ui_degSymbol, &ui_img_degrees_png);
    lv_img_set_src(ui_rudderSymbol, &ui_img_wheel_png);

    lv_obj_set_style_bg_img_src(ui_Bar2, &ui_img_rudder_png,
                                LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_img_src(ui_rudder, &ui_img_rudderline_png,
                                LV_PART_KNOB | LV_STATE_DEFAULT);

    lv_img_set_src(ui_redgradient, &ui_img_red_png);
    lv_img_set_src(ui_greengradient, &ui_img_green_png);
}

static void apply_swap_to_screen(lv_obj_t *scr)
{
    if(scr == NULL) return;

    lv_obj_add_style(scr, &style_color_swap, LV_PART_MAIN);
}

void ui_apply_swap_globally_once(void)
{
    apply_swap_to_screen(ui_Screen1);
    apply_swap_to_screen(ui_Screen2);
    apply_swap_to_screen(ui_Screen3);
    apply_swap_to_screen(ui_Screen4);
    apply_swap_to_screen(ui_Screen5);
}

