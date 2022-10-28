#include <SPI.h>
#include <LoRa.h>
#include <HAMqttDevice.h>
#define ss 5
#define rst 14
#define dio0 2

const int powerLatch = 17;
const int MAX_ANALOG_VAL = 4095;
const float MAX_BATTERY_VOLTAGE = 4.2; 

void setup() {
  pinMode(powerLatch, OUTPUT);
  digitalWrite(powerLatch, HIGH);
  HAMqttDevice binary_Sensor("Mailbox", HAMqttDevice::BINARY_SENSOR, "homeassistant");
  binary_Sensor
    .addConfigVar("ret","true")
    .addConfigVar("pl_on","ON")
    .addConfigVar("pl_off","OFF")
    .addConfigVar("stat_t","homeassistant/binary_sensor/mailbox/state")
    .addConfigVar("dev","{\"name\":\"Mailbox Sensor\",\"ids\":[\"MailboxSensor\"]}")
    .addConfigVar("dev_cla","occupancy");
  HAMqttDevice sensor("Mailbox Battery", HAMqttDevice::SENSOR, "homeassistant");
  sensor
    .addConfigVar("dev","{\"name\":\"Mailbox Sensor\",\"ids\":[\"MailboxSensor\"]}")
    .addConfigVar("stat_t","homeassistant/sensor/mailbox_battery/state")
    .addConfigVar("dev_cla","battery")
    .addConfigVar("unit_of_meas","%");
  HAMqttDevice button("Mailbox Reset", HAMqttDevice::BUTTON, "homeassistant");
  button
    .addConfigVar("ret","true")
    .addConfigVar("cmd_t","homeassistant/binary_sensor/mailbox/state")
    .addConfigVar("dev","{\"name\":\"Mailbox Sensor\",\"ids\":[\"MailboxSensor\"]}")
    .addConfigVar("payload_press","OFF");
  delay(500);
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);
  
  while (!LoRa.begin(866E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0x34);
  Serial.println("LoRa Initializing OK!");
  LoRa.beginPacket();
  LoRa.println(binary_Sensor.getConfigTopic()+"?"+binary_Sensor.getConfigPayload());
  LoRa.endPacket();
  delay(500);
  LoRa.beginPacket();
  LoRa.println("homeassistant/binary_sensor/mailbox/state?ON");
  LoRa.endPacket();
  delay(500);
  LoRa.beginPacket();
  LoRa.println(sensor.getConfigTopic()+"?"+sensor.getConfigPayload());
  LoRa.endPacket();
  delay(500);  
  int rawValue = analogRead(13);
  float voltageLevel = (rawValue / 4095.0) * 2 * 1.1 * 3.3;
  int batteryFraction = (float)((voltageLevel / MAX_BATTERY_VOLTAGE) * 100);
  if(batteryFraction > 100){ batteryFraction = 100; }
  LoRa.beginPacket();
  LoRa.println("homeassistant/sensor/mailbox_battery/state?" + String(batteryFraction));
  LoRa.endPacket();
  delay(500);
  LoRa.beginPacket();
  LoRa.println(button.getConfigTopic()+"?"+button.getConfigPayload());
  LoRa.endPacket();
  delay(500);
  digitalWrite(powerLatch, LOW);
}

void loop() {
}
