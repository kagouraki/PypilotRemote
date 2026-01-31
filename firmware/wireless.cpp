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
#include "Arduino.h"
#include <WiFi.h>
#include <Preferences.h>
#include <ESPAsyncWebServer.h>
#include "pypilot_client.h"
#include "functions.h"
#include <Update.h>

Preferences prefs;
AsyncWebServer server(80);

AsyncTCPClient tcp;

WiFiState wifiState = WIFI_IDLE;

void updateWifiUI(void);

// ============================
// Timing / Retry Config
// ============================
unsigned long wifiAttemptStart = 0;
unsigned long retryWaitStart = 0;

unsigned long WIFI_TIMEOUT = 8000;     // 8 sec
unsigned long RETRY_WAIT_TIME = 5000;  // 5 sec

int retryCount = 0;
int MAX_RETRIES = 3;  // now configurable

// ============================
// User Configuration Variables
// ============================
String ssid = "";
String pass = "";

String PypilotIP = "";
int PypilotTCPport = 0;
int PypilotUDPport = 0;
int BrightnessLcdAlways = 100;
int BrightnessLcdOn = 100;
int BrightnessKeys = 100;
String StartTheme = "Light";
String BacklightTheme = "Light";

// ============================
// Load / Save Configuration
// ============================
void loadConfig() {
	ssid = prefs.getString("ssid", "pypilot");
	pass = prefs.getString("pass", "");

	MAX_RETRIES = prefs.getInt("maxret", 4);

	PypilotIP = prefs.getString("pip", "192.168.14.1");
	PypilotTCPport = prefs.getInt("ptcp", 23322);
	PypilotUDPport = prefs.getInt("pudp", 8317);
	BrightnessLcdAlways = prefs.getInt("brtLcdAlways", 255);
	BrightnessLcdOn = prefs.getInt("brtLcdOn", 100);
	BrightnessKeys = prefs.getInt("brtKeys", 50);
	StartTheme = prefs.getString("startTheme", "Light");
	BacklightTheme = prefs.getString("backlightTheme", "Light");
}

void saveConfig(String ns, String pw, int maxret, String pip, int tcp, int udp,
		int brtLcdAlways, int brtLcdOn, int brtKeys, String startTheme,
		String backlightTheme) {
	prefs.putString("ssid", ns);
	prefs.putString("pass", pw);

	prefs.putInt("maxret", maxret);

	prefs.putString("pip", pip);
	prefs.putInt("ptcp", tcp);
	prefs.putInt("pudp", udp);
	prefs.putInt("brtLcdAlways", brtLcdAlways);
	prefs.putInt("brtLcdOn", brtLcdOn);
	prefs.putInt("brtKeys", brtKeys);
	prefs.putString("startTheme", startTheme);
	prefs.putString("backlightTheme", backlightTheme);
}

// ============================
// Start Portal
// ============================
static String wifiListHtml;

void startPortal() {
	displayLog("Starting WiFi Portal…");

	wifiState = WIFI_PORTAL;
	updateWifiUI();

	WiFi.mode(WIFI_AP);
	WiFi.softAP("PypilotRemote", "12345678");

	int n = WiFi.scanNetworks();
	wifiListHtml = "<ul>";
	for (int i = 0; i < n; i++) {
		wifiListHtml += "<li>" + WiFi.SSID(i) + " (" + String(WiFi.RSSI(i))
				+ " dBm)</li>";
	}
	wifiListHtml += "</ul>";
	WiFi.scanDelete();

	char buf[128];
	snprintf(buf, sizeof(buf), "Portal at: http://%s\n",
			WiFi.softAPIP().toString().c_str());
	displayLog(buf);

	server.reset();

	/* ================= MAIN PORTAL PAGE ================= */
	server.on("/", HTTP_GET,
			[](AsyncWebServerRequest *req) {

				String page =
						"<html><body>"
								"<h2>ESP WiFi Setup</h2>"
								"<form action='/save'>"

								"WiFi SSID:<br>"
								"<input name='ssid' value='" + ssid + "'><br>"

								"Password:<br>"
								"<input type='password' name='pass' value='"
								+ pass
								+ "'><br><br>"

										"Max WiFi Retries:<br>"
										"<input type='number' name='maxret' value='"
								+ String(MAX_RETRIES) + "'><br><br>"

										"<h3>Pypilot Configuration</h3>"
										"Pypilot IP:<br>"
										"<input name='pip' value='" + PypilotIP
								+ "'><br>"

										"TCP Port (23322):<br>"
										"<input type='number' name='ptcp' value='"
								+ String(PypilotTCPport)
								+ "'><br>"

										"UDP Port(8317):<br>"
										"<input type='number' name='pudp' value='"
								+ String(PypilotUDPport)
								+ "'><br>"
										"<h3>Device Configuration</h3>"
										"BrightnessBacklightOff(0-255):<br>"
										"<input type='number' name='brtLcdAlways' value='"
								+ String(BrightnessLcdAlways)
								+ "'><br>"

										"BrightnessBacklightOn(0-255):<br>"
										"<input type='number' name='brtLcdOn' value='"
								+ String(BrightnessLcdOn)
								+ "'><br>"

										"BrightnessKeys(0-255):<br>"
										"<input type='number' name='brtKeys' value='"
								+ String(BrightnessKeys)
								+ "'><br><br>"

								// ADDED: Start Theme dropdown
										"Start Theme:<br>"
										"<select name='startTheme'>"
										"<option value='Light'"
								+ (StartTheme == "Light" ? " selected" : "")
								+ ">Light</option>"
										"<option value='Dark'"
								+ (StartTheme == "Dark" ? " selected" : "")
								+ ">Dark</option>"
										"</select><br><br>"

										// ADDED: Backlight Theme dropdown
										"Backlight Theme:<br>"
										"<select name='backlightTheme'>"
										"<option value='Light'"
								+ (BacklightTheme == "Light" ? " selected" : "")
								+ ">Light</option>"
										"<option value='Dark'"
								+ (BacklightTheme == "Dark" ? " selected" : "")
								+ ">Dark</option>"
										"</select><br><br>"

										"<input type='submit' value='Save & Reboot'>"
										"</form>"

										"<form action='/rescan' method='POST'>"
										"<input type='submit' value='Rescan WiFi'>"
										"</form>"

										"<h3>OTA Firmware Update</h3>"
										"<form method='POST' action='/update' enctype='multipart/form-data'>"
										"<input type='file' name='firmware'>"
										"<input type='submit' value='Upload Firmware'>"
										"</form>"

										"<h3>Available Networks</h3>"
								+ wifiListHtml +

								"</body></html>";

				req->send(200, "text/html", page);
			});

	/* ================= WIFI RESCAN ================= */
	server.on("/rescan", HTTP_POST, [](AsyncWebServerRequest *req) {

		xTaskCreate([](void*) {

			int n = WiFi.scanNetworks();
			wifiListHtml = "<ul>";
			for (int i = 0; i < n; i++) {
				wifiListHtml += "<li>" + WiFi.SSID(i) + " (" + String(WiFi.RSSI(i)) + " dBm)</li>";
}
			wifiListHtml += "</ul>";
			WiFi.scanDelete();

			vTaskDelete(nullptr);

		}, "wifi_rescan", 4096, nullptr, 1, nullptr);

		req->redirect("/");
	});

	/* ================= SAVE CONFIG ================= */
	server.on("/save", HTTP_GET, [](AsyncWebServerRequest *req) {

		String ns = req->arg("ssid");
		String pw = req->arg("pass");
		int maxret = req->arg("maxret").toInt();

		String pip = req->arg("pip");
		int ptcp = req->arg("ptcp").toInt();
		int pudp = req->arg("pudp").toInt();
		int brtLcdAlways = req->arg("brtLcdAlways").toInt();
		int brtLcdOn = req->arg("brtLcdOn").toInt();
		int brtKeys = req->arg("brtKeys").toInt();

		// ADDED: Get theme values
		String startTheme = req->arg("startTheme");
		String backlightTheme = req->arg("backlightTheme");

		saveConfig(ns, pw, maxret, pip, ptcp, pudp, brtLcdAlways, brtLcdOn, brtKeys,
	startTheme, backlightTheme);  // ADDED: Pass theme values

		req->send(200, "text/html", "<h3>Saved! Rebooting…</h3>");

		req->onDisconnect([]() {
			ESP.restart();
		});
	});

	/* ================= OTA UPDATE ================= */
	server.on("/update", HTTP_POST, [](AsyncWebServerRequest *req) {
		bool ok = !Update.hasError();
		req->send(200, "text/plain", ok ? "Update OK. Rebooting…" :"Update Failed!" );
		if (ok) {
			delay(500);
			ESP.restart();
		}
	},
	[](AsyncWebServerRequest *req,
			String filename,
			size_t index,
			uint8_t *data,
			size_t len,
			bool final) {

		if (!index) {
			displayLog("OTA update started");
			if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
				Update.printError(Serial);
			}
		}

		if (Update.write(data, len) != len) {
			Update.printError(Serial);
		}

		if (final) {
			if (!Update.end(true)) {
				Update.printError(Serial);
			} else {
				displayLog("OTA update complete");
			}
		}
	}
	);

	server.begin();
}

// ============================
// WiFi Connect Attempt
// ============================
void startWiFiConnect() {
	char buf[128];
	snprintf(buf, sizeof(buf), "Trying WiFi: %s\n", ssid.c_str());

	displayLog(buf);
	WiFi.mode(WIFI_STA);
	WiFi.setMinSecurity(WIFI_AUTH_WPA_PSK); //added because couldnt connect to pi zero 2 AP
	WiFi.begin(ssid.c_str(), pass.c_str());
	wifiAttemptStart = millis();
	wifiState = WIFI_TRY_CONNECT;
}

// ============================
// WiFi FSM
// ============================
void updateWiFi() {

	switch (wifiState) {

	case WIFI_IDLE:
		if (ssid.length() == 0)
			startPortal();
		else {
			retryCount = 0;
			startWiFiConnect();
		}
		break;

	case WIFI_TRY_CONNECT:
		if (WiFi.status() == WL_CONNECTED) {
			updateWifiUI();
			wifiState = WIFI_CONNECTED;
			displayLog("Connected!");
			tcp.begin(PypilotIP.c_str(), PypilotTCPport);
		} else if (millis() - wifiAttemptStart > WIFI_TIMEOUT) {

			retryCount++;

			char buf[128];
			snprintf(buf, sizeof(buf), "WiFi timeout %d/%d\n", retryCount,
					MAX_RETRIES);

			displayLog(buf);

			if (retryCount >= MAX_RETRIES)
				startPortal();
			else {
				retryWaitStart = millis();
				wifiState = WIFI_RETRY_WAIT;
				updateWifiUI();
			}
		}
		break;

	case WIFI_CONNECTED:
		if (WiFi.status() != WL_CONNECTED) {
			displayLog("WiFi lost → retry…");
			retryCount = 0;
			retryWaitStart = millis();
			wifiState = WIFI_RETRY_WAIT;
			updateWifiUI();
		}
		break;

	case WIFI_RETRY_WAIT:
		if (millis() - retryWaitStart > RETRY_WAIT_TIME)
			startWiFiConnect();
		break;

	case WIFI_PORTAL:
		updateWifiUI();
		break;
	}
}

void initWireless() {
	prefs.begin("wifi", false);
	loadConfig();

	wifiState = WIFI_IDLE;
}

// ============================
// Getters (safe accessors)
// ============================

String getPypilotIP() {
	return PypilotIP;
}

int getPypilotTCPport() {
	return PypilotTCPport;
}

int getPypilotUDPport() {
	return PypilotUDPport;
}

int getBrightnessLcdAlways() {
	return BrightnessLcdAlways;
}

int getBrightnessLcdOn() {
	return BrightnessLcdOn;
}

int getBrightnessKeys() {
	return BrightnessKeys;
}
String getStartTheme() {
	return StartTheme;
}
String getBacklightTheme() {
	return BacklightTheme;
}

WiFiState getWiFiState() {
	return wifiState;
}

void updateWifiUI(void) {
	wifi_mode_t mode = WiFi.getMode();
	wl_status_t staStatus = WiFi.status();

	bool staEnabled = (mode == WIFI_MODE_STA || mode == WIFI_MODE_APSTA);
	bool apEnabled = (mode == WIFI_MODE_AP || mode == WIFI_MODE_APSTA);
	bool staConnected = (staStatus == WL_CONNECTED);

	/* -------- STATE 2: Connected as station -------- */
	if (staEnabled && staConnected) {
		lv_label_set_text(ui_wifiMsg, "Station Mode");

		IPAddress ip = WiFi.localIP();
		lv_label_set_text_fmt(ui_IPadress, "%d.%d.%d.%d", ip[0], ip[1], ip[2],
				ip[3]);
		String ssid;
		ssid = WiFi.SSID();
		lv_label_set_text(ui_ssid, ssid.c_str());
	}

	/* -------- STATE 3: Portal / AP mode -------- */
	else if (apEnabled) {
		lv_label_set_text(ui_wifiMsg, "Access Point Mode");

		IPAddress ip = WiFi.softAPIP();
		lv_label_set_text_fmt(ui_IPadress, "%d.%d.%d.%d", ip[0], ip[1], ip[2],
				ip[3]);
		String ssid;
		ssid = WiFi.softAPSSID();
		lv_label_set_text(ui_ssid, ssid.c_str());
	}

	/* -------- STATE 1: Trying / disconnected -------- */
	else {
		lv_label_set_text(ui_wifiMsg, "WiFi not connected");
		lv_label_set_text(ui_IPadress, "0.0.0.0");
		lv_label_set_text(ui_ssid, "");
	}
}

