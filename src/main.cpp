#include <Arduino.h>

#include "main.h"

#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

#include "arduino_secrets.h"

const char SSID[] = SECRET_SSID;
const char PASS[] = SECRET_PASS;
const unsigned int LOCAL_PORT = 2390;

WiFiUDP udp;

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.println("Starting Access Point…");

    // Start AP mode
    int status = WiFi.beginAP(SSID, PASS);
    if (status != WL_AP_LISTENING) {
        Serial.println("Failed to start AP");
        while (true)
            ;
    }

    digitalWrite(LED_BUILTIN, HIGH);

    Serial.println("Access Point active");
    printAPStatus();

    // Start UDP
    udp.begin(LOCAL_PORT);
    Serial.print("Listening for UDP on port ");
    Serial.println(LOCAL_PORT);
}

void loop() {
    int packetSize = udp.parsePacket();
    if (packetSize) {
        Serial.print("Received packet from ");
        Serial.print(udp.remoteIP());
        Serial.print(":");
        Serial.println(udp.remotePort());

        byte incoming[256];
        int len = udp.read(incoming, 255);

        Serial.print("Message: ");
        for (int i = 0; i < len; i++) {
            uint8_t b = incoming[i];
            for (int bit = 7; bit >= 0; bit--) {
                Serial.print((b >> bit) & 1);
            }
            Serial.print(" ");
        }
        Serial.println();
    }
}

void printAPStatus() {
    Serial.print("SSID: ");
    Serial.println(SSID);

    IPAddress ip = WiFi.localIP();
    Serial.print("AP IP address: ");
    Serial.println(ip);

    Serial.print("MAC: ");
    byte mac[6];
    WiFi.macAddress(mac);
    for (int i = 0; i < 6; i++) {
        if (mac[i] < 16)
            Serial.print("0");
        Serial.print(mac[i], HEX);
        if (i < 5)
            Serial.print(":");
    }
    Serial.println();
}