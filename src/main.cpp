

#define SAMPLING 512

#include <PerceptorLogger.h>

PerceptorLogger logger;

#include <SerialSetup.h>
SerialSetup ser;

void setup(void) {
	Serial.begin(115200);

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
		ser.get("Ingresar SSID");
		ser.get("Ingresar contrasenia");
		delay(2000);

		Serial.println(F("Muchas gracias"));
	});

	// logger.initialize();
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

	// logger.readAndLog(25, "SET");
}