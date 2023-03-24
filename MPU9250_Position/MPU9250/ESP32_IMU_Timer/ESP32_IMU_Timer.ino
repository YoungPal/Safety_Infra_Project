#include "MPU9250.h"
#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;
MPU9250 IMU(i2c0, 0x68);

String str = "";
byte flag = 0;

volatile int intCnt;
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

int status;
int i = 0;
const uint16_t port = 5000;
const char* host = "10.10.141.69";  // ip or dns

WiFiClient client;

void IRAM_ATTR onTimer(){
  portENTER_CRITICAL_ISR(&timerMux);
  intCnt++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
  Serial.begin(115200);
  Wifi_init();
  IMU_init();
  Timer_init();

#define RAD_TO_DEG 57.3
}

void loop() {  
  if (client.available()) {
    IMU.readSensor();
    if(intCnt>0)
    {
      portENTER_CRITICAL(&timerMux);
      intCnt--;
      portEXIT_CRITICAL(&timerMux);      
      client.printf("[1]%.3f@%.3f@%.3f@%.3f@%.3f@%.3f", IMU.getAccelX_mss(), IMU.getAccelY_mss(), IMU.getAccelZ_mss(), IMU.getGyroX_rads(), IMU.getGyroY_rads(), IMU.getGyroZ_rads());
      Serial.printf("[1]%.3f@%.3f@%.3f@%.3f@%.3f@%.3f\n", IMU.getAccelX_mss(), IMU.getAccelY_mss(), IMU.getAccelZ_mss(), IMU.getGyroX_rads(), IMU.getGyroY_rads(), IMU.getGyroZ_rads());
    } 
  } 
}

void Wifi_init(){
  WiFiMulti.addAP("iot0", "iot00000");
  Serial.println();
  Serial.print("Waiting for WiFi... ");
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.connect(host, port);
  client.print("[2:PASSWD]\n");
}

void IMU_init(){
    status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1) {
    }
    IMU.setAccelRange(MPU9250::ACCEL_RANGE_2G);
    IMU.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
  }
}

void Timer_init(){
  timer = timerBegin(0,80,true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 50000, true);
  timerAlarmEnable(timer);
}