#include <Arduino.h>
#include <HCSR04.h>

UltraSonicDistanceSensor distanceSensor(33, 32);

void setup() {
    Serial.begin(9600);
    pinMode(25, OUTPUT);
    pinMode(26, OUTPUT);
}

void loop() {
    int distance = distanceSensor.measureDistanceCm();

    Serial.print(F("Distance: "));
    Serial.print(distance);
    Serial.println(F("cm"));

    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH);
    delay(500);
    digitalWrite(25, LOW);
    digitalWrite(26, LOW);
    delay(500);

}