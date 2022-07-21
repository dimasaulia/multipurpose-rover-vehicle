#include <Arduino.h>
#include <RH_NRF24.h>

byte LEFT_MOTOR = A0;
byte RIGHT_MOTOR = A1;

RH_NRF24 nrf24(8, 10);

void setup()
{
  Serial.begin(9600); while (!Serial); // wait for serial port to connect. Needed for Leonardo only
  pinMode(LEFT_MOTOR, INPUT);
  pinMode(RIGHT_MOTOR, INPUT);
  if (!nrf24.init()) {
    Serial.println("init failed");
  }

  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(3)) {
    Serial.println("setChannel failed");
  }

  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)) {
    Serial.println("setRF failed");
  }

  Serial.println("Transmitter started");
  delay(2000);
}

void loop(){
  // ANALOG READ
  byte LEFT_MOTOR_SPEED = map(analogRead(LEFT_MOTOR), 0, 1023, 0, 255);
  byte RIGHT_MOTOR_SPEED = map(analogRead(RIGHT_MOTOR), 0, 1023, 0, 255);

  Serial.print("LEFT_MOTOR AXIS : ");
  Serial.println(LEFT_MOTOR_SPEED);
  Serial.print("RIGHT_MOTOR AXIS : ");
  Serial.println(RIGHT_MOTOR_SPEED);
  
  uint8_t data[2];
  data[0] = LEFT_MOTOR_SPEED; // left motor speed
  data[1] = RIGHT_MOTOR_SPEED; // right motor speed
  nrf24.send(data, sizeof(data));
  nrf24.waitPacketSent();
  Serial.println("Sending data...");
}