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
#ifndef WIRELESS_H
#define WIRELESS_H

#include <Arduino.h>

// Forward declare AsyncTCPClient so we can reference it
class AsyncTCPClient;

// ------------------------------
// WiFi state machine enumeration
// ------------------------------
enum WiFiState {
    WIFI_IDLE,
    WIFI_TRY_CONNECT,
    WIFI_CONNECTED,
    WIFI_RETRY_WAIT,
    WIFI_PORTAL
};

// ------------------------------
// Public API
// ------------------------------
void initWireless();
void updateWiFi();
void startPortal();
void loadConfig();

// Getter functions
String getPypilotIP();
int getPypilotTCPport();
int getPypilotUDPport();
int getBrightnessLcdAlways();
int getBrightnessLcdOn();
int getBrightnessKeys();
WiFiState getWiFiState();
String getStartTheme();
String getBacklightTheme();

// Global TCP client (defined in pypilot_client.cpp)
extern AsyncTCPClient tcp;

// Global WiFi state
extern WiFiState wifiState;

#endif
