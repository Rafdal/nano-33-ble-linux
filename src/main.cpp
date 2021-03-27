

#define SAMPLING 512

#include <PerceptorLogger.h>

PerceptorLogger logger;

#include <SerialSetup.h>
SerialSetup ser;

void setup(void) {
	Serial.begin(230400);

	delay(800);

	Serial.println(F("Init"));

	pinMode(LED_BUILTIN, OUTPUT);	

	for (uint8_t i = 0; i < 5; i++)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(500);
		digitalWrite(LED_BUILTIN, LOW);
		delay(500);
	}

	ser.setupLoop([](){
		ser.on("read", [](){
			delay(500);
			logger.logFor(3, 512);
		});
	});

	logger.initialize(150);
}


bool ledState;
unsigned long lastMs=0;
unsigned long currentMs=0;

void loop() {

	ser.listen();

	currentMs = millis();
	if (currentMs - lastMs > 1000)
	{
		digitalWrite(LED_BUILTIN, ledState);
		ledState = !ledState;
		lastMs = currentMs;
	}
}