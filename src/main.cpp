#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>


Adafruit_MPU6050 mpu;
unsigned int sampling_period_us;
double freq = 1000; // $ Hz
#define SAMPLING 512
const uint16_t samples = SAMPLING;

#define ARCHIVOS 256

#include <datalog.h>
Datalog datalog("mov"); // Nombre del archivo

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


datalog_type_t data;

void loop() {

	/* Get new sensor events with the readings */
	for (size_t j = 0; j < ARCHIVOS; j++)
	{
		ms_stamp = millis();
		us = micros();
		for (int i = 0; i < samples; i++)
		{
			sensors_event_t a;
			mpu.getAccelEvent(&a);

			data.x[i] = a.acceleration.x;
			data.y[i] = a.acceleration.y;
			data.z[i] = a.acceleration.z;

			while (micros() - us < sampling_period_us);

			us += sampling_period_us;
		}
		Serial.println("Time2Read");
		Serial.println(millis() - ms_stamp);

		datalog.log(data);

		/* for (uint16_t i = 0; i < samples; i++)
		{
			Serial.print(data.x[i]);
			Serial.print(",\t");
			Serial.print(data.y[i]);
			Serial.print(",\t");
			Serial.println(data.z[i]);
		} */
		// for(;;);

		Serial.print(j);
		Serial.println(F(" Lectura Finalizada"));
	}


	for(;;);
}