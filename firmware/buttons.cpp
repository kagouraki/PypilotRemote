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
#include "buttons.h"
#include "pypilot_client.h"
#include "wireless.h"
#include "ui/ui.h"
#include "functions.h"
#include <WiFi.h>
//--------
extern void ui_light_theme(void);
extern void ui_dark_theme(void);
//-------

#define BACKLIGHT_PINLCD 47
#define BACKLIGHT_PINKEYS 21
#define SCROLL_STEP 60

static bool backlightOn = false;

unsigned long debounceTime = 50;

static const unsigned long AUTO_LONG_PRESS_MS = 3000;

static const unsigned long BACKLIGHT_LONG_PRESS_MS = 5000;

static const unsigned long BACKLIGHT_MODE_LONG_PRESS_MS = 5000;

static bool backlightModeComboArmed = false;
static bool backlightModeComboSent = false;
static unsigned long backlightModeComboStartTime = 0;
static bool backlightModeOverlap = false;

#define BUTTON_QUEUE_SIZE 16
static ButtonEvent eventQueue[BUTTON_QUEUE_SIZE];
static uint8_t queueHead = 0;
static uint8_t queueTail = 0;

static void enqueue(ButtonEvent e) {
	uint8_t next = (queueHead + 1) % BUTTON_QUEUE_SIZE;
	if (next != queueTail) {
		eventQueue[queueHead] = e;
		queueHead = next;
	}
}

bool eventAvailable() {
	return queueHead != queueTail;
}

ButtonEvent popButtonEvent() {
	if (!eventAvailable())
		return BTN_EVENT_NONE;

	ButtonEvent e = eventQueue[queueTail];
	queueTail = (queueTail + 1) % BUTTON_QUEUE_SIZE;
	return e;
}

ButtonState buttons[TOTAL_BUTTONS] = { { BTN_PORT1, false, 0, BTN_EVENT_PORT1,
		false, 0, false }, {
BTN_STBD1, false, 0, BTN_EVENT_STBD1, false, 0, false }, {
BTN_PORT10, false, 0, BTN_EVENT_PORT10, false, 0, false }, {
BTN_STBD10, false, 0, BTN_EVENT_STBD10, false, 0, false }, {
BTN_MODE, false, 0, BTN_EVENT_MODE, false, 0, false }, {
BTN_INFO, false, 0, BTN_EVENT_INFO, false, 0, false }, {
BTN_AUTO, false, 0, BTN_EVENT_AUTO, false, 0, false }, // long-press enabled
		{ BTN_STANDBY, false, 0, BTN_EVENT_STANDBY, false, 0, false }, {
		BTN_BACKLIGHT, false, 0, BTN_EVENT_BACKLIGHT, false, 0, false } };

// ------------------------
// Setup
// ------------------------

void setupButtons() {
	for (int i = 0; i < TOTAL_BUTTONS; i++) {
		pinMode(buttons[i].pin, INPUT_PULLUP);
		buttons[i].lastState = digitalRead(buttons[i].pin);
		buttons[i].lastChangeTime = 0;
		buttons[i].isDown = false;
		buttons[i].pressStartTime = 0;
		buttons[i].longPressSent = false;
	}
	pinMode(BACKLIGHT_PINLCD, OUTPUT);
	pinMode(BACKLIGHT_PINKEYS, OUTPUT);

	ledcSetup(1, 20000, 8);        // 20 kHz PWM
	ledcSetup(2, 20000, 8);        // 20 kHz PWM
	ledcAttachPin(BACKLIGHT_PINLCD, 1);
	ledcAttachPin(BACKLIGHT_PINKEYS, 2);

	int levelLcdAlways = getBrightnessLcdAlways();
	//Serial.printf("lcdAlways %d\n", levelLcdAlways);
	ledcWrite(1, levelLcdAlways);
	ledcWrite(2, 0);
	if (getStartTheme() == "Light") {
		ui_light_theme();
	} else {
		ui_dark_theme();
	}
}

// ------------------------
// Polling with debounce
// ------------------------

void pollButtons() {
	unsigned long now = millis();

	int modeIndex = -1;
	int backlightIndex = -1;
	for (int i = 0; i < TOTAL_BUTTONS; i++) {
		if (buttons[i].eventID == BTN_EVENT_MODE) {
			modeIndex = i;
		} else if (buttons[i].eventID == BTN_EVENT_BACKLIGHT) {
			backlightIndex = i;
		}
	}

	for (int i = 0; i < TOTAL_BUTTONS; i++) {

		ButtonState &b = buttons[i];

		bool current = digitalRead(b.pin);
		bool previous = b.lastState;

		// Debounce: only act if state is stable for debounceTime
		if (current != previous) {

			if (now - b.lastChangeTime > debounceTime) {

				b.lastState = current;
				b.lastChangeTime = now;

				// --------------------------
				// Stable press (active LOW)
				// --------------------------
				if (current == LOW) {

					// Track press for AUTO (long-press)
					if (b.eventID == BTN_EVENT_AUTO) {
						b.isDown = true;
						b.pressStartTime = now;
						b.longPressSent = false;
					}
					// --------------------------
					// Track press for BACKLIGHT (short/long like AUTO)
					// --------------------------
					else if (b.eventID == BTN_EVENT_BACKLIGHT) {
						b.isDown = true;
						b.pressStartTime = now;
						b.longPressSent = false;

					}
					// --------------------------
					// Track press for MODE (delay until release so MODE+BACKLIGHT can be decided)
					// --------------------------
					else if (b.eventID == BTN_EVENT_MODE) {
						b.isDown = true;
						b.pressStartTime = now;
					} else {
						enqueue(b.eventID);
					}

					// --------------------------
					// Stable release (HIGH)
					// --------------------------
				} else {

					// For AUTO: if no long-press was sent, treat as short press
					if (b.eventID == BTN_EVENT_AUTO && b.isDown) {
						b.isDown = false;

						if (!b.longPressSent) {
							// Short press
							enqueue(BTN_EVENT_AUTO);
						}
					}

					if (b.eventID == BTN_EVENT_BACKLIGHT && b.isDown) {
						b.isDown = false;
						if (!backlightModeOverlap && !b.longPressSent) {
							enqueue(BTN_EVENT_BACKLIGHT);
						}
					}

					if (b.eventID == BTN_EVENT_MODE && b.isDown) {
						b.isDown = false;
						if (!backlightModeOverlap) {
							enqueue(BTN_EVENT_MODE);
						}
					}

				}
			}
		}

		// --------------------------
		// Long-press check for AUTO
		// --------------------------
		if (b.eventID == BTN_EVENT_AUTO && b.isDown && !b.longPressSent) {
			if (now - b.pressStartTime >= AUTO_LONG_PRESS_MS) {
				b.longPressSent = true;
				enqueue(BTN_EVENT_AUTO_LONG);
			}
		}

		// --------------------------
		// Long-press check for BACKLIGHT
		// Only when it's not part of a MODE+BACKLIGHT overlap.
		// --------------------------
		if (b.eventID == BTN_EVENT_BACKLIGHT && b.isDown && !b.longPressSent) {
			if (!backlightModeOverlap
					&& (now - b.pressStartTime >= BACKLIGHT_LONG_PRESS_MS)) {
				b.longPressSent = true;
				enqueue(BTN_EVENT_BACKLIGHT_LONG);
			}
		}
	}

	// --------------------------
	// Long-press check for BACKLIGHT + MODE together
	// --------------------------
	if (modeIndex >= 0 && backlightIndex >= 0) {
		ButtonState &m = buttons[modeIndex];
		ButtonState &bl = buttons[backlightIndex];

		// Detect overlap (both down at the same time). Once it happens, suppress individual
		// MODE/BACKLIGHT events until BOTH are released.
		if (m.isDown && bl.isDown) {
			backlightModeOverlap = true;

			if (!backlightModeComboArmed) {
				backlightModeComboArmed = true;
				backlightModeComboSent = false;
				// Start timing when BOTH are actually down:
				backlightModeComboStartTime =
						(m.pressStartTime > bl.pressStartTime) ?
								m.pressStartTime : bl.pressStartTime;
			}

			if (!backlightModeComboSent
					&& (now - backlightModeComboStartTime
							>= BACKLIGHT_MODE_LONG_PRESS_MS)) {
				backlightModeComboSent = true;
				enqueue(BTN_EVENT_BACKLIGHT_MODE_LONG);
			}
		} else {
			// If either button is up, stop timing the combo.
			backlightModeComboArmed = false;
			backlightModeComboSent = false;
			backlightModeComboStartTime = 0;

			// Clear overlap suppression only once BOTH are released.
			if (!m.isDown && !bl.isDown) {
				backlightModeOverlap = false;
			}
		}
	}
}

// ------------------------
// Non-blocking event processor
// ------------------------

void processButtonEvents() {
	while (eventAvailable()) {
		ButtonEvent e = popButtonEvent();

		switch (e) {
		case BTN_EVENT_PORT1: {
			//Serial.println("Port1");
			lv_obj_t *cur = lv_scr_act();
			if (cur != ui_Screen1) {
				return;
			}
			bool enabled = readBool("ap.enabled");
			if (enabled) {
				float headingCommand = readNumber("ap.heading_command");
				float headingCommandNew = headingCommand - 1;
				String msg = "ap.heading_command="
						+ String(headingCommandNew, 2) + "\n";
				//Serial.println(msg);
				tcp.send(msg);
			}
			break;
		}
		case BTN_EVENT_STBD1: {
			//Serial.println("Stbd1");
			lv_obj_t *cur = lv_scr_act();
			if (cur != ui_Screen1) {
				if (cur == ui_Screen5) {
					int32_t y = lv_obj_get_scroll_y(ui_scroll);
					int32_t newY = y - SCROLL_STEP;
					lv_obj_scroll_to_y(ui_scroll, newY, LV_ANIM_ON);
					return;
				}
				return;
			}
			bool enabled = readBool("ap.enabled");
			if (enabled) {
				float headingCommand = readNumber("ap.heading_command");
				float headingCommandNew = headingCommand + 1;
				String msg = "ap.heading_command="
						+ String(headingCommandNew, 2) + "\n";
				//Serial.println(msg);
				tcp.send(msg);
			}
			break;
		}

		case BTN_EVENT_PORT10: {
			//Serial.println("Port10");
			lv_obj_t *cur = lv_scr_act();
			if (cur != ui_Screen1) {
				return;
			}
			bool enabled = readBool("ap.enabled");
			if (enabled) {
				float headingCommand = readNumber("ap.heading_command");
				float headingCommandNew = headingCommand - 10;
				String msg = "ap.heading_command="
						+ String(headingCommandNew, 2) + "\n";
				//Serial.println(msg);
				tcp.send(msg);
			}
			break;
		}

		case BTN_EVENT_STBD10: {
			//Serial.println("Stbd10");
			lv_obj_t *cur = lv_scr_act();
			if (cur != ui_Screen1) {
				if (cur == ui_Screen5) {
					int32_t y = lv_obj_get_scroll_y(ui_scroll);
					int32_t newY = y + SCROLL_STEP;
					lv_obj_scroll_to_y(ui_scroll, newY, LV_ANIM_ON);
					return;
				}
				return;
			}

			bool enabled = readBool("ap.enabled");
			if (enabled) {
				float headingCommand = readNumber("ap.heading_command");
				float headingCommandNew = headingCommand + 10;
				String msg = "ap.heading_command="
						+ String(headingCommandNew, 2) + "\n";
				//Serial.println(msg);
				tcp.send(msg);
			}
			break;
		}

		case BTN_EVENT_MODE: {
			//Serial.println("Mode");
			lv_obj_t *cur = lv_scr_act();
			if (cur != ui_Screen1) {
				return;
			}

			static int modeIndex = 0;

			// Get available modes from parsed JSON
			auto modes = readStringList("ap.modes");
			if (modes.empty()) {
				displayLog("No modes available!");
				break;
			}

			// Wrap around
			modeIndex = (modeIndex + 1) % modes.size();

			String nextMode = modes[modeIndex];

			// Send to pypilot
			String msg = "ap.mode=\"" + nextMode + "\"" + "\n";
			//Serial.println(msg);
			tcp.send(msg);
			break;
		}

		case BTN_EVENT_INFO: {
			//Serial.println("Info");

			lv_obj_t *cur = lv_scr_act();

			if (cur == ui_Screen1) {
				enterInfoScreen();
				lv_scr_load(ui_Screen3);
			} else if (cur == ui_Screen3) {
				lv_scr_load(ui_Screen4);
			} else if (cur == ui_Screen4) {
				lv_scr_load(ui_Screen5);
			} else {
				exitInfoScreen();
				lv_scr_load(ui_Screen1);
			}

			break;
		}

		case BTN_EVENT_AUTO: {
			bool enabled = readBool("ap.enabled");
			if (enabled) {
				break;
			}
			//Serial.println("Auto (short)");
			float headingCommand = readNumber("ap.heading");
			String msg = "ap.heading_command=" + String(headingCommand, 2)
					+ "\nap.enabled=true\n";
			//Serial.println(msg);
			tcp.send(msg);
			break;
		}

		case BTN_EVENT_AUTO_LONG: {
			bool enabled = readBool("ap.enabled");
			if (enabled) {
				break;
			}
			//Serial.println("Auto (LONG 3s)");
			//Serial.println("ap.enabled=true\n");
			tcp.send("ap.enabled=true\n");
			break;
		}

		case BTN_EVENT_STANDBY: {
			bool enabled = readBool("ap.enabled");
			if (!enabled) {
				break;
			}
			//Serial.println("Standby");
			//Serial.println("ap.enabled=false\n");
			tcp.send("ap.enabled=false\n");
			break;
		}

		case BTN_EVENT_BACKLIGHT: {
			//Serial.println("Backlight");

			backlightOn = !backlightOn;

			if (backlightOn) {
				// Restore brightness from config
				int levelLcdOn = getBrightnessLcdOn();   // 0–255
				int levelKeys = getBrightnessKeys();   // 0–255
				//Serial.printf("lcdON %d\n", levelLcdOn);
				//Serial.printf("lcdKeys %d\n", levelKeys);
				ledcWrite(1, levelLcdOn);
				ledcWrite(2, levelKeys);
				if (getBacklightTheme() == "Light") {
					ui_light_theme();
				} else {
					ui_dark_theme();
				}

			} else {
				// Turn OFF
				int levelLcdAlways = getBrightnessLcdAlways();
				ledcWrite(1, levelLcdAlways);
				ledcWrite(2, 0);
				if (getStartTheme() == "Light") {
					ui_light_theme();
				} else {
					ui_dark_theme();
				}

			}

			break;
		}

		case BTN_EVENT_BACKLIGHT_LONG: {
			//Serial.println("Backlight (LONG)");
			startPortal();
			break;
		}

		case BTN_EVENT_BACKLIGHT_MODE_LONG: {
			//Serial.println("Backlight + Mode (LONG)");
			ESP.restart();
			break;
		}

		default:
			break;
		}
	}
}

void enterInfoScreen() {
	if (tcp.isTcpConnected) {
		String watchCmd = "watch={"
				"\"profile\":2.0,"
				"\"imu.accel.calibration.locked\":2.0,"
				"\"imu.compass.calibration.locked\":2.0,"
				"\"imu.warning\":2.0,"
				"\"servo.max_current\":2.0,"
				"\"servo.amp_hours\":2.0,"
				"\"servo.voltage\":2.0,"
				"\"servo.current\":2.0,"
				"\"servo.motor_temp\":2.0,"
				"\"servo.watts\":2.0,"
				"\"servo.flags\":2.0,"
				"\"ap.version\":2.0,"
				"\"gps.source\":2.0,"
				"\"wind.source\":2.0,"
				"\"truewind.source\":2.0,"
				"\"rudder.source\":2.0,"
				"\"apb.source\":2.0,"
				"\"water.source\":2.0,"
				"\"servo.controller_temp\":2.0"
				"}\n";

		//displayLog("[TCP] Sending watch Info: " + watchCmd);
		tcp.send(watchCmd.c_str());
	}
}
void exitInfoScreen() {
	if (tcp.isTcpConnected) {
		String watchCmd = "watch={"
				"\"profile\":false,"
				"\"imu.accel.calibration.locked\":false,"
				"\"imu.compass.calibration.locked\":false,"
				"\"imu.warning\":false,"
				"\"servo.max_current\":false,"
				"\"servo.amp_hours\":false,"
				"\"servo.voltage\":false,"
				"\"servo.current\":false,"
				"\"servo.motor_temp\":false,"
				"\"servo.watts\":false,"
				"\"servo.flags\":false,"
				"\"ap.version\":false,"
				"\"gps.source\":false,"
				"\"wind.source\":false,"
				"\"truewind.source\":false,"
				"\"rudder.source\":false,"
				"\"apb.source\":false,"
				"\"water.source\":false,"
				"\"servo.controller_temp\":false"
				"}\n";

		//displayLog("[TCP] Sending watch Info: " + watchCmd);
		tcp.send(watchCmd.c_str());
	}

}

void updateInfoScreen() {

	static String lastProfile = "";
	static String lastVersion = "";
	static String lastsourceGps = "";
	static String lastsourceWind = "";
	static String lastsourceTruewind = "";
	static String lastsourceRudder = "";
	static String lastsourceApb = "";
	static String lastsourceWater = "";
	static String lastImuW = "";
	static String lastServoF = "";
	static float lastMoCurrent = 9999;
	static float lastMoVoltage = 9999;
	static float lastMoWatts = 9999;
	static float lastMoAh = 9999;
	static float lastMoMaxC = 9999;
	static float lastMoTemp = 9999;
	static float lastDriveTemp = 9999;
	static bool lastCompLock = false;
	static bool lastAccLock = false;

	lv_obj_t *cur = lv_scr_act();
	if (cur == ui_Screen1) {
		return;
	}

	// -------------------------
	// PROFILE
	// -------------------------
	String profile = readString("profile");
	if (lastProfile != profile) {
		lastProfile = profile;
		lv_label_set_text(ui_profile, profile.c_str());
	}

	// -------------------------
	// VERSION
	// -------------------------
	String version = readString("ap.version");
	if (lastVersion != version) {
		lastVersion = version;
		lv_label_set_text(ui_pypilotVersion, version.c_str());
	}

	// -------------------------
	// IMU WARNING
	// -------------------------
	String imuW = readString("imu.warning");
	if (lastImuW != imuW) {
		lastImuW = imuW;
		lv_label_set_text(ui_imuWarning, imuW.c_str());
	}

	// -------------------------
	// SERVO FLAGS
	// -------------------------
	String servoF = readString("servo.flags");
	if (lastServoF != servoF) {
		lastServoF = servoF;
		lv_label_set_text(ui_servoFlags, servoF.c_str());
	}

	// -------------------------
	// Gps source
	// -------------------------
	String sourceGps = readString("gps.source");
	if (lastsourceGps != sourceGps) {
		lastsourceGps = sourceGps;
		lv_label_set_text(ui_sourceGps, sourceGps.c_str());
	}

	// -------------------------
	// Wind source
	// -------------------------
	String sourceWind = readString("wind.source");
	if (lastsourceWind != sourceWind) {
		lastsourceWind = sourceWind;
		lv_label_set_text(ui_sourceWind, sourceWind.c_str());
	}

	// -------------------------
	// True wind source
	// -------------------------
	String sourceTruewind = readString("truewind.source");
	if (lastsourceTruewind != sourceTruewind) {
		lastsourceTruewind = sourceTruewind;
		lv_label_set_text(ui_sourceTruewind, sourceTruewind.c_str());
	}

	// -------------------------
	// Rudder source
	// -------------------------
	String sourceRudder = readString("rudder.source");
	if (lastsourceRudder != sourceRudder) {
		lastsourceRudder = sourceRudder;
		lv_label_set_text(ui_sourceRudder, sourceRudder.c_str());
	}

	// -------------------------
	// APB source
	// -------------------------
	String sourceApb = readString("apb.source");
	if (lastsourceApb != sourceApb) {
		lastsourceApb = sourceApb;
		lv_label_set_text(ui_sourceApb, sourceApb.c_str());
	}

	// -------------------------
	// Water source
	// -------------------------
	String sourceWater = readString("water.source");
	if (lastsourceWater != sourceWater) {
		lastsourceWater = sourceWater;
		lv_label_set_text(ui_sourceWater, sourceWater.c_str());
	}

	// -------------------------
	// MOTOR NUMBERS
	// -------------------------
	float tempFloat;

	tempFloat = readNumber("servo.current");
	if (lastMoCurrent != tempFloat) {
		lastMoCurrent = tempFloat;
		lv_label_set_text(ui_motorCurrent, String(tempFloat, 2).c_str());
	}

	tempFloat = readNumber("servo.voltage");
	if (lastMoVoltage != tempFloat) {
		lastMoVoltage = tempFloat;
		lv_label_set_text(ui_motorVoltage, String(tempFloat, 2).c_str());
	}

	tempFloat = readNumber("servo.watts");
	if (lastMoWatts != tempFloat) {
		lastMoWatts = tempFloat;
		lv_label_set_text(ui_motorWatts, String(tempFloat, 2).c_str());
	}

	tempFloat = readNumber("servo.amp_hours");
	if (lastMoAh != tempFloat) {
		lastMoAh = tempFloat;
		lv_label_set_text(ui_motorAh, String(tempFloat, 2).c_str());
	}

	tempFloat = readNumber("servo.max_current");
	if (lastMoMaxC != tempFloat) {
		lastMoMaxC = tempFloat;
		lv_label_set_text(ui_motorMaxC, String(tempFloat, 2).c_str());
	}

	tempFloat = readNumber("servo.motor_temp");
	if (lastMoTemp != tempFloat) {
		lastMoTemp = tempFloat;
		lv_label_set_text(ui_motorTemp, String(tempFloat, 2).c_str());
	}

	tempFloat = readNumber("servo.controller_temp");
	if (lastDriveTemp != tempFloat) {
		lastDriveTemp = tempFloat;
		lv_label_set_text(ui_driverTemp, String(tempFloat, 2).c_str());
	}

	// -------------------------
	// COMPASS LOCK
	// -------------------------
	bool compLock = readBool("imu.compass.calibration.locked");
	if (lastCompLock != compLock) {
		lastCompLock = compLock;
		if (compLock) {
			lv_obj_set_style_text_color(ui_compCal, lv_color_hex(0x00BD3C),
					LV_PART_MAIN);
			lv_label_set_text(ui_compCal, "LOCKED");
		} else {
			lv_obj_set_style_text_color(ui_compCal, lv_color_hex(0xBD0000),
					LV_PART_MAIN);
			lv_label_set_text(ui_compCal, "UNLOCKED");
		}
	}

	// -------------------------
	// ACCEL LOCK
	// -------------------------
	bool accLock = readBool("imu.accel.calibration.locked");
	if (lastAccLock != accLock) {
		lastAccLock = accLock;
		if (accLock) {
			lv_obj_set_style_text_color(ui_accCal, lv_color_hex(0x00BD3C),
					LV_PART_MAIN);
			lv_label_set_text(ui_accCal, "LOCKED");
		} else {
			lv_obj_set_style_text_color(ui_accCal, lv_color_hex(0xBD0000),
					LV_PART_MAIN);
			lv_label_set_text(ui_accCal, "UNLOCKED");
		}
	}
}
