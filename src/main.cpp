#include <Arduino.h>

#include "main.h"

#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

#include "arduino_secrets.h"

const char SSID[] = SECRET_SSID;
const char PASS[] = SECRET_PASS;
const unsigned int LOCAL_PORT = 2390;

WiFiUDP Udp;

void setup() {
    Serial.begin(115200);
    while (!Serial){
        ; // wait for serial port to connect. Needed for native USB port only
    }

    Serial.println("Starting Access Point…");

    // Start AP mode
    int status = WiFi.beginAP(SSID, PASS);
    if (status != WL_AP_LISTENING) {
        Serial.println("Failed to start AP");
        while (true)
            ;
    }

    Serial.println("Access Point active");
    printAPStatus();

    // Start UDP
    Udp.begin(LOCAL_PORT);
    Serial.print("Listening for UDP on port ");
    Serial.println(LOCAL_PORT);
}

void loop()
{
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        Serial.print("Received packet from ");
        Serial.print(Udp.remoteIP());
        Serial.print(":");
        Serial.println(Udp.remotePort());

        char incoming[256];
        int len = Udp.read(incoming, 255);
        if (len > 0)
            incoming[len] = '\0';

        Serial.print("Message: ");
        Serial.println(incoming);
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
    for (int i = 0; i < 6; i++)
    {
        if (mac[i] < 16)
            Serial.print("0");
        Serial.print(mac[i], HEX);
        if (i < 5)
            Serial.print(":");
    }
    Serial.println();
}