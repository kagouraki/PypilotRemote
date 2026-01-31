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
#include "pypilot_client.h"
#include <ctype.h>
#include "functions.h"

// GLOBAL MAP (name -> Value)
std::map<String, Value> dataStore;

static String rxBuffer = "";
//static unsigned long tcpReconnectAt = 0;

// ---------------------------------------------------------
// READ NUMBER
// ---------------------------------------------------------
float readNumber(const String &key, float defaultValue) {
	auto it = dataStore.find(key);
	if (it == dataStore.end())
		return defaultValue;
	if (it->second.type != Value::NUMBER)
		return defaultValue;
	return it->second.numberValue;
}

// ---------------------------------------------------------
// READ BOOL
// ---------------------------------------------------------
bool readBool(const String &key, bool defaultValue) {
	auto it = dataStore.find(key);
	if (it == dataStore.end())
		return defaultValue;
	if (it->second.type != Value::BOOL)
		return defaultValue;
	return it->second.boolValue;
}


// ---------------------------------------------------------
// READ STRING
// ---------------------------------------------------------
String readString(const String &key, const String &defaultValue) {
	auto it = dataStore.find(key);
	if (it == dataStore.end())
		return defaultValue;
	if (it->second.type != Value::STRING)
		return defaultValue;
	return it->second.stringValue;
}

// ---------------------------------------------------------
// READ STRING LIST
// ---------------------------------------------------------
std::vector<String> readStringList(const String &key) {
	auto it = dataStore.find(key);
	if (it == dataStore.end())
		return {};
	if (it->second.type != Value::STRING_LIST)
		return {};
	return it->second.stringList;
}

// Parse a single "name=value" line from TCP
void parseIncomingNameValue(const char *data, size_t len) {
	String s = String(data).substring(0, len);
	s.trim();

	int eq = s.indexOf('=');
	if (eq < 0)
		return;

	// FIXED: cannot chain substring().trim()
	String name = s.substring(0, eq);
	name.trim();

	String raw = s.substring(eq + 1);
	raw.trim();

	Value v;

	// 1) STRING LIST: ["a","b","c"]
	if (raw.startsWith("[") && raw.endsWith("]")) {
		v.type = Value::STRING_LIST;
		String inner = raw.substring(1, raw.length() - 1);
		inner.replace("\"", "");

		v.stringList.clear();
		int start = 0;

		while (true) {
			int comma = inner.indexOf(',', start);

			if (comma == -1) {
				String item = inner.substring(start);
				item.trim();                        // FIXED
				if (item.length() > 0)
					v.stringList.push_back(item);
				break;
			}

			String item = inner.substring(start, comma);
			item.trim();                            // FIXED

			if (item.length() > 0)
				v.stringList.push_back(item);

			start = comma + 1;
		}

		dataStore[name] = v;
		return;
	}

	// 2) Boolean
	if (raw == "true" || raw == "false") {
		v.type = Value::BOOL;
		v.boolValue = (raw == "true");
		dataStore[name] = v;
		return;
	}

	// 3) Number
	if (raw.length() > 0
			&& (isdigit(raw.charAt(0)) || raw.charAt(0) == '-'
					|| raw.charAt(0) == '+')) {

		v.type = Value::NUMBER;
		v.numberValue = raw.toFloat();
		dataStore[name] = v;
		return;
	}

	// 4) String (strip surrounding quotes if present)
	v.type = Value::STRING;

	// Remove wrapping quotes: "wind" → wind
	if (raw.startsWith("\"") && raw.endsWith("\"") && raw.length() >= 2) {
		v.stringValue = raw.substring(1, raw.length() - 1);
	} else {
		v.stringValue = raw;
	}

	v.stringValue.trim();   // optional but helpful
	dataStore[name] = v;
}

// -------------------------
// AsyncTCPClient methods
// -------------------------

AsyncTCPClient::AsyncTCPClient() {
	client = new AsyncClient();

	isTcpConnected = false;

	lastIncoming = 0;
	lastPing = 0;
	tcpReconnectAt = 0;

	serverHost = "";
	serverPort = 0;
}

void AsyncTCPClient::begin(const char *host, uint16_t port) {
	serverHost = host;
	serverPort = port;

	setupCallbacks();
	connect();
}

void AsyncTCPClient::connect() {
	if (client->connected())
		return;

	char buf[128];
	snprintf(buf, sizeof(buf), "[TCP] Connecting to %s:%u...",
			serverHost.c_str(), serverPort);

	displayLog(buf);


	client->connect(serverHost.c_str(), serverPort);
}

void AsyncTCPClient::setupCallbacks() {

	// CONNECTED
	client->onConnect([this](void *arg, AsyncClient *c) {
		displayLog("[TCP] Connected!");
		isTcpConnected = true;

		// ---- RESUBSCRIBE ON EVERY CONNECT ----
		String watchCmd = "watch={"
				"\"ap.heading\":0.25,"
				//"\"ap.preferred_mode\":1.0,"
				"\"ap.mode\":1.0,"
				"\"rudder.range\":5.0,"
				"\"rudder.angle\":0.1,"
				"\"ap.modes\":1.0,"
				"\"ap.enabled\":0.25,"
				"\"ap.heading_command\":0.25,"
				"\"imu.heading\":0.25"
				"}\n";

		//displayLog("[TCP] Sending watch: " + watchCmd);

		c->write(watchCmd.c_str());
	});

	// RECEIVED DATA
	client->onData([this](void *arg, AsyncClient *c, void *data, size_t len) {

		lastIncoming = millis();

		const char *text = (const char*) data;

		// Append incoming data to buffer
		for (size_t i = 0; i < len; i++) {
			char ch = text[i];
			rxBuffer += ch;

			// When a newline arrives => full message ready
			if (ch == '\n') {
				String line = rxBuffer;
				line.trim();

				if (line.length() > 0) {
					//Serial.print("[TCP] LINE: ");
					//Serial.println(line);

					parseIncomingNameValue(line.c_str(), line.length());
				}

				rxBuffer = "";  // reset buffer
			}
		}
	});

	// DISCONNECTED
	client->onDisconnect([this](void *arg, AsyncClient *c) {
		displayLog("[TCP] Disconnected.");
		isTcpConnected = false;

		if (wifiState == WIFI_CONNECTED)
			tcpReconnectAt = millis() + 300;
		else
			tcpReconnectAt = 0;   // disable reconnect while WiFi is down
	});

	// ERROR
	client->onError([this](void *arg, AsyncClient *c, int8_t error) {

		char buf[128];
			snprintf(buf, sizeof(buf), "[TCP] Error: %d\n", error);

			displayLog(buf);

		isTcpConnected = false;

		if (wifiState == WIFI_CONNECTED)
			tcpReconnectAt = millis() + 300;
		else
			tcpReconnectAt = 0;
	});
}

void AsyncTCPClient::send(const String &msg) {
	if (client->connected()) {
		client->write(msg.c_str(), msg.length());
	}
}

void AsyncTCPClient::loop() {
	unsigned long now = millis();

	// -------------------------------------------------
	// IF WIFI IS NOT CONNECTED → FORCE TCP OFF
	// -------------------------------------------------
	if (wifiState != WIFI_CONNECTED) {

		if (client && client->connected()) {
			displayLog("[TCP] WiFi lost → closing TCP");
			client->close();
		}

		isTcpConnected = false;
		tcpReconnectAt = 0;     // disable reconnect timer
		lastIncoming = 0;
		lastPing = 0;

		return; // do nothing until WiFi connects again
	}

	// -------------------------------------------------
	// (1) Handle pending reconnect WHEN wifi is OK
	// -------------------------------------------------
	if (!isTcpConnected && tcpReconnectAt && now > tcpReconnectAt) {
		tcpReconnectAt = 0;
		displayLog("[TCP] Reconnecting (WiFi OK)...");
		connect();
		return;
	}

	// -------------------------------------------------
	// If not connected, nothing else to do
	// -------------------------------------------------
	if (!isTcpConnected || !client)
		return;

	// -------------------------------------------------
	// (2) Manual keep-alive ping
	// -------------------------------------------------
	if (now - lastIncoming > 3000 && now - lastPing > 3000) {
		if (client->canSend()) {
			client->write("\n");  // tiny ping to keep connection alive
		}
		lastPing = now;
	}

	// -------------------------------------------------
	// (3) Connection timeout watchdog
	// -------------------------------------------------
	if (now - lastIncoming > 8000) {
		displayLog("[TCP] Timeout — forcing disconnect");
		client->close();          // triggers onDisconnect()
		isTcpConnected = false;

		// schedule reconnect only if WiFi is still connected
		tcpReconnectAt = millis() + 300;
	}
}
