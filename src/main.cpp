#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include <SD.h>

Adafruit_MPU6050 mpu;
unsigned int sampling_period_us;
double freq = 400; // $ Hz
#define SAMPLING 512
const uint16_t samples = SAMPLING;	


void setup(void) {
	Serial.begin(921600);
	while (!Serial)
		delay(10); // will pause Zero, Leonardo, etc until serial console opens

	Serial.println("Adafruit MPU6050 test!");

	// Try to initialize!
	if (!mpu.begin()) {
		Serial.println("Failed to find MPU6050 chip");
		while (1) {
		delay(10);
		}
	}
	Serial.println("MPU6050 Found!");

	Wire.setClock(400000L);

	mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

	mpu.setAccelerometerRange(MPU6050_RANGE_2_G);

	delay(100);

	sampling_period_us = round(1000000 * (1.0 / freq));


	pinMode(4, OUTPUT);
    digitalWrite(4, LOW);   // Activar SD
    if (!SD.begin(4)) {
        Serial.println(F("SD card failed, or not present"));
        for(;;);
    }
    Serial.println(F("SD card initialized.\n"));
}

unsigned long us, ms_stamp;

float x[SAMPLING], y[SAMPLING], z[SAMPLING];

void loop() {

	/* Get new sensor events with the readings */

	us = micros();
	ms_stamp = millis();
	for (int i = 0; i < samples; i++)
	{
		sensors_event_t a, g, temp;
		mpu.getEvent(&a, &g, &temp);

		x[i] = a.acceleration.x;
		y[i] = a.acceleration.y;
		z[i] = a.acceleration.z;

		while (micros() - us < sampling_period_us);

		us += sampling_period_us;
	}
	Serial.println("Time2Read");
	Serial.println(millis() - ms_stamp);

	for (uint16_t i = 0; i < samples; i++)
	{
		Serial.print(x[i]);
		Serial.print(",\t");
		Serial.print(y[i]);
		Serial.print(",\t");
		Serial.println(z[i]);
	}
	

	for(;;);
}