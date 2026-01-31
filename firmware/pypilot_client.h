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
#pragma once
#include <Arduino.h>
#include <AsyncTCP.h>
#include "wireless.h"

#include <map>
#include <vector>

// ---------------------------
// VALUE VARIANT TYPE
// ---------------------------
struct Value {
	enum Type {
		STRING, NUMBER, BOOL, STRING_LIST
	} type;

	Value() :
			type(STRING), numberValue(0), boolValue(false) {
	}

	String stringValue;
	float numberValue;
	bool boolValue;
	std::vector<String> stringList;
};

// GLOBAL datastore accessible anywhere
extern std::map<String, Value> dataStore;

// ---------------------------
// Simple ACCESSORS for user
// ---------------------------
float readNumber(const String &key, float defaultValue = 0.0);
bool readBool(const String &key, bool defaultValue = false);
String readString(const String &key, const String &defaultValue = "");
std::vector<String> readStringList(const String &key);

// Parsing function
void parseIncomingNameValue(const char *data, size_t len);

class AsyncTCPClient {
public:
	AsyncTCPClient();

	void begin(const char *host, uint16_t port);
	void send(const String &msg);
	void loop();

	bool isTcpConnected;      // global connection status

private:
	AsyncClient *client;
	String serverHost;
	uint16_t serverPort;

	unsigned long lastIncoming;   // timestamp of last received packet
	unsigned long lastPing;       // timestamp of last ping

	unsigned long tcpReconnectAt; // reconnect timer

	void connect();
	void setupCallbacks();
};

