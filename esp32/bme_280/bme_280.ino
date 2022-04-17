/*********
  Complete project details at http://randomnerdtutorials.com  
*********/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;


// WiFi
const char* ssid     = "SYBR3";
const char* password = "PortgFood";

//const char* host = "data.sparkfun.com";
const char* host = "www.catmountain.com";
const char* streamId   = "....................";
const char* privateKey = "....................";

const char* ca_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFOTCCBCGgAwIBAgISBF2rjKt0ugbwIp5IG6i4ZRwSMA0GCSqGSIb3DQEBCwUA\n" \
"MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD\n" \
"EwJSMzAeFw0yMjA0MTQwMDE5MzFaFw0yMjA3MTMwMDE5MzBaMBoxGDAWBgNVBAMT\n" \
"D2NhdG1vdW50YWluLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n" \
"ALriM5MNT16/nwSKriGvEUEezbIhA4DDGKNKnR6jV5QcNnyfCaoA64x4ruVJ090C\n" \
"qmzwnxdj/vS6IbOAEvCg5HWYq36diLrQI9MpoQgJE+bvU7A5QlDAdycBqAsGtbPJ\n" \
"lB4EvpEQH+JtqsN6ySVYDNPYHRCnD+gmrJEHXqTMSEwzU8YNL23Os16TmZJVvnKY\n" \
"yynp5AxB1Ka56zyQKpwqIJJh0o14deioCSMfQIsdAnnZCNh6sQ5sGsI3EQpnIE2c\n" \
"ofYKtbFNocJQyRJxTSx6rN+qMLJfP7atv6uakAytscrLq36S9sPfQbto6aoSaKkZ\n" \
"h7WCMc5kp++ckk9EWIyaHfUCAwEAAaOCAl8wggJbMA4GA1UdDwEB/wQEAwIFoDAd\n" \
"BgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwDAYDVR0TAQH/BAIwADAdBgNV\n" \
"HQ4EFgQUQbPMMMU0ZLt5EjfOoD7VZHe2fDMwHwYDVR0jBBgwFoAUFC6zF7dYVsuu\n" \
"UAlA5h+vnYsUwsYwVQYIKwYBBQUHAQEESTBHMCEGCCsGAQUFBzABhhVodHRwOi8v\n" \
"cjMuby5sZW5jci5vcmcwIgYIKwYBBQUHMAKGFmh0dHA6Ly9yMy5pLmxlbmNyLm9y\n" \
"Zy8wLwYDVR0RBCgwJoIPY2F0bW91bnRhaW4uY29tghN3d3cuY2F0bW91bnRhaW4u\n" \
"Y29tMEwGA1UdIARFMEMwCAYGZ4EMAQIBMDcGCysGAQQBgt8TAQEBMCgwJgYIKwYB\n" \
"BQUHAgEWGmh0dHA6Ly9jcHMubGV0c2VuY3J5cHQub3JnMIIBBAYKKwYBBAHWeQIE\n" \
"AgSB9QSB8gDwAHYAKXm+8J45OSHwVnOfY6V35b5XfZxgCvj5TV0mXCVdx4QAAAGA\n" \
"JaduzQAABAMARzBFAiBCVxf3GyT+qj13F7VBrQubehAj6lbMhNUS3u2kycje1wIh\n" \
"AOwJi1mItdic1BUPV+cfNNJiwTSls4pAurcIqnSjDDrPAHYAb1N2rDHwMRnYmQCk\n" \
"URX/dxUcEdkCwQApBo2yCJo32RMAAAGAJadvRwAABAMARzBFAiEA9V8uzF+c30Vd\n" \
"2aG8p0a0Itlzc4RrPcoFvEbRpq/YSjkCIF/HG5KNA/mHE/RyBpQY/PhML/ebMYKK\n" \
"Abse3E6YBnGeMA0GCSqGSIb3DQEBCwUAA4IBAQBmEYxQf38JXSGjhhgwN6orpq/F\n" \
"oGorzr41ILADV/kh856KlRD13MCSrroUrmNxlIz3KpiY7eUUQyiHhk/7LBSVvEIz\n" \
"2nq1cj5Pn1P8KhdLqIWzrSi9hcns/8xpFofezyy9PTww+5W0KfqxAWGPW/VY1kYm\n" \
"RCHnsSOV+0mv9AemLgkohgGe8mU/aCeeexj02AAvb3cu/9xuOy9mdygJrFxkkGPF\n" \
"A3N2HT/muZbIuRrYJmeHD1narrlkqtAQ73fp84oxKEFPybLT5pIvrnx26Smnp/Po\n" \
"Tch0QIy0DwKYwZYDFDBAxe1DcWU5wZ76HLz6jydnMOnD9U/eMq1GijvWBBIS\n" \
"-----END CERTIFICATE-----\n";

const char* test_root_ca= \
     "-----BEGIN CERTIFICATE-----\n" \
     "MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\n" \
     "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
     "DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow\n" \
     "PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\n" \
     "Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n" \
     "AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\n" \
     "rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\n" \
     "OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\n" \
     "xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\n" \
     "7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\n" \
     "aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n" \
     "HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\n" \
     "SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\n" \
     "ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\n" \
     "AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\n" \
     "R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\n" \
     "JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\n" \
     "Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\n" \
     "-----END CERTIFICATE-----\n";
     

void setup() {
  Serial.begin(9600);
  while(!Serial);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("");
    
  Serial.println(F("BME280 test"));

  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x76);  
  //status = bme.begin();  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    Serial.println("Whoops!");
    while (1);
  }

  Serial.println("-- Default Test --");
  delayTime = 60000;

  Serial.println();
}

int value = 0;
const char*  server = "www.howsmyssl.com";

void loop() { 
  printValues();

      delay(5000);
    ++value;

    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    // WiFiClient client;
    WiFiClientSecure client;
    //client.setCACert(ca_cert);
    client.setCACert(test_root_ca);
//    Serial.println(ca_cert);
    //const int httpPort = 80;
    const int httpPort = 443;
    
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
//    String url = "/input/";
//    url += streamId;
//    url += "?private_key=";
//    url += privateKey;
//    url += "&value=";
//    url += value;

      String url = "/";

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    
    //client.print(String("GET ") + url + " HTTP/1.1\r\n" +
     //            "Host: " + host + "\r\n" +
       //          "Connection: close\r\n\r\n");

 //   client.println("GET https://www.howsmyssl.com/a/check HTTP/1.0");
 //   client.println("Host: www.howsmyssl.com");
    client.println("GET https://www.catmountain.com/ HTTP/1.0");
    client.println("Host: www.catmountain.com");
    client.println("Connection: close");
    client.println();
    
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");
    
  delay(delayTime);
}

void printValues() {
  /*Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");*/
  
  // Convert temperature to Fahrenheit
  Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}
