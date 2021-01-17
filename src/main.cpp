

#define SAMPLING 512

#include <PerceptorLogger.h>

PerceptorLogger logger;

void setup(void) {
	Serial.begin(921600);

	pinMode(LED_BUILTIN, OUTPUT);	

	for (uint8_t i = 0; i < 5; i++)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(500);
		digitalWrite(LED_BUILTIN, LOW);
		delay(500);
	}
	

	logger.initialize();
	Serial.readString();
}


bool ledState;
unsigned long lastMs=0;
unsigned long currentMs=0;

void loop() {

	currentMs = millis();
	if (currentMs - lastMs > 1000)
	{
		digitalWrite(LED_BUILTIN, ledState);
		ledState = !ledState;
		lastMs = currentMs;
	}

	if (Serial.available())
	{
		logger.logFiles(25, "SET");
		Serial.readString();
	}

}