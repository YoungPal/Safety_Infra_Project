#include "MPU9250.h"
#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;
MPU9250 IMU(i2c0, 0x68);

String str = "";
byte flag = 0;

int status;
int i = 0;
const uint16_t port = 5000;
const char* host = "10.10.141.69";  // ip or dns

WiFiClient client;

void setup() {
  Serial.begin(115200);

  WiFiMulti.addAP("iot0", "iot00000");

  Serial.println();
  Serial.println();
  Serial.print("Waiting for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1) {
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.connect(host, port);
  client.print("[2:PASSWD]\n");

  IMU.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  // setting the gyroscope full scale range to +/-500 deg/s
  IMU.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
}

#define RAD_TO_DEG 57.3

void loop() {
  int maxloops = 0;

  if (client.available()) {
    IMU.readSensor();
    client.printf("[1]%.3f@%.3f@%.3f@%.3f@%.3f@%.3f", IMU.getAccelX_mss(), IMU.getAccelY_mss(), IMU.getAccelZ_mss(), IMU.getGyroX_rads(), IMU.getGyroY_rads(), IMU.getGyroZ_rads());
    Serial.printf("[1]%.3f@%.3f@%.3f@%.3f@%.3f@%.3f", IMU.getAccelX_mss(), IMU.getAccelY_mss(), IMU.getAccelZ_mss(), IMU.getGyroX_rads(), IMU.getGyroY_rads(), IMU.getGyroZ_rads());
    
    delay(1000);
    if (!str.equals("")) {
      Serial.println(str);
      str = "";
    }
  }
}