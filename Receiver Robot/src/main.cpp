#include <Arduino.h>
#include <RH_NRF24.h>
#include <L298N.h>

byte RIGHT_MOTOR_SPEED_PIN = 3;
byte RIGHT_MOTOR_SPEED_DIRECTION_1_PIN = 2;
byte RIGHT_MOTOR_SPEED_DIRECTION_2_PIN = 4;
byte RIGHT_MOTOR_SPEED_CONTROL = 0;

byte LEFT_MOTOR_SPEED_PIN = 5;
byte LEFT_MOTOR_SPEED_DIRECTION_1_PIN = 6;
byte LEFT_MOTOR_SPEED_DIRECTION_2_PIN = 7;
byte LEFT_MOTOR_SPEED_CONTROL = 0;
byte TRIG_PIN = A0;
byte ECHO_PIN = 9;
byte MIN_DISTANCE = 15;
byte LCD_RES = 27;
byte LCD_DC = 27;

RH_NRF24 nrf24(8, 10);

void setup(){
  pinMode(RIGHT_MOTOR_SPEED_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_SPEED_DIRECTION_1_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_SPEED_DIRECTION_2_PIN, OUTPUT);

  pinMode(LEFT_MOTOR_SPEED_PIN, OUTPUT);
  pinMode(LEFT_MOTOR_SPEED_DIRECTION_1_PIN, OUTPUT);
  pinMode(LEFT_MOTOR_SPEED_DIRECTION_2_PIN, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600);
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
}

void loop(){
 if (nrf24.available()){
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (nrf24.recv(buf, &len)){
      Serial.println("Data from receiver : ");
      RIGHT_MOTOR_SPEED_CONTROL = buf[0];
      LEFT_MOTOR_SPEED_CONTROL = buf[1];
      Serial.print("Left motor speed : ");
      Serial.println(RIGHT_MOTOR_SPEED_CONTROL);
      Serial.print("Right motor speed : ");
      Serial.println(LEFT_MOTOR_SPEED_CONTROL);
    }
 }

  //  Cek Kondisi
  int duration, distance;
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW); 
  duration = pulseIn(ECHO_PIN, HIGH);
  distance =(duration/2)/29.1;
  // Serial.print("Jarak : ");
  // Serial.println(distance);

  // KONTROL MOTOR KANAN
  if(distance > MIN_DISTANCE){
    if ((RIGHT_MOTOR_SPEED_CONTROL <= 115)){
      digitalWrite(RIGHT_MOTOR_SPEED_DIRECTION_1_PIN, HIGH);
      digitalWrite(RIGHT_MOTOR_SPEED_DIRECTION_2_PIN, LOW);
      digitalWrite(RIGHT_MOTOR_SPEED_PIN, 50);
    }

    if (RIGHT_MOTOR_SPEED_CONTROL > 115 && RIGHT_MOTOR_SPEED_CONTROL <= 125){
      digitalWrite(RIGHT_MOTOR_SPEED_DIRECTION_1_PIN, LOW);
      digitalWrite(RIGHT_MOTOR_SPEED_DIRECTION_2_PIN, LOW);
      digitalWrite(RIGHT_MOTOR_SPEED_PIN, 0);
    }

    if (RIGHT_MOTOR_SPEED_CONTROL > 125){
      digitalWrite(RIGHT_MOTOR_SPEED_DIRECTION_1_PIN, LOW);
      digitalWrite(RIGHT_MOTOR_SPEED_DIRECTION_2_PIN, HIGH);
      digitalWrite(RIGHT_MOTOR_SPEED_PIN, 50);
    }

    // KONTROL MOTOR KIRI
    if (LEFT_MOTOR_SPEED_CONTROL <= 115){
      digitalWrite(LEFT_MOTOR_SPEED_DIRECTION_1_PIN, HIGH);
      digitalWrite(LEFT_MOTOR_SPEED_DIRECTION_2_PIN, LOW);
      digitalWrite(LEFT_MOTOR_SPEED_PIN, 50);
    }

    if (LEFT_MOTOR_SPEED_CONTROL > 115 && LEFT_MOTOR_SPEED_CONTROL <= 125){
      digitalWrite(LEFT_MOTOR_SPEED_DIRECTION_1_PIN, LOW);
      digitalWrite(LEFT_MOTOR_SPEED_DIRECTION_2_PIN, LOW);
      digitalWrite(LEFT_MOTOR_SPEED_PIN, 0);
    }

    if (LEFT_MOTOR_SPEED_CONTROL > 125){
      digitalWrite(LEFT_MOTOR_SPEED_DIRECTION_1_PIN, LOW);
      digitalWrite(LEFT_MOTOR_SPEED_DIRECTION_2_PIN, HIGH);
      digitalWrite(LEFT_MOTOR_SPEED_PIN, 50);
    }
  }else{
      digitalWrite(RIGHT_MOTOR_SPEED_DIRECTION_1_PIN, LOW);
      digitalWrite(RIGHT_MOTOR_SPEED_DIRECTION_2_PIN, LOW);
      digitalWrite(RIGHT_MOTOR_SPEED_PIN, 0);
      digitalWrite(LEFT_MOTOR_SPEED_DIRECTION_1_PIN, LOW);
      digitalWrite(LEFT_MOTOR_SPEED_DIRECTION_2_PIN, LOW);
      digitalWrite(LEFT_MOTOR_SPEED_PIN, 0);
  }
}
