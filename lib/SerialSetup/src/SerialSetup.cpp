#include "SerialSetup.h"

// Cambia modo y avisa
void SerialSetup::switchMode()
{
    active = !active;
    Serial.print(F("SerialSetup mode "));
    Serial.println(active);
}

void SerialSetup::flush()
{
    delay(25);
    Serial.readString();
}

void SerialSetup::read()
{
    if (Serial.available())
    {
        Serial.println(F("Presione Enter al finalizar"));
        buffer="";
        lastMs = millis();
        while (lastMs + 6000UL >= millis())
        {
            if (Serial.available())
            {
                lastMs = millis();
                char c = Serial.read();
                Serial.print(c);
                if (c == '\n' || c == '\r')
                {
                    return;
                }
                buffer += c;
            }
            delay(0);
        }
    }
}

String SerialSetup::get(const char* message)
{
    flush();

    Serial.println(message);
    unsigned long ms_begin = millis();
    while (ms_begin+20000UL >= millis())
    {
        unsigned long currentMs = millis();
        if (currentMs - lastMs > 5000)
        {
            Serial.println(message);
            lastMs = currentMs;
        }

        if (Serial.available())
        {
            read();

            Serial.printf("Recibido(s) %u caracteres: \"%s\"\n", buffer.length(), buffer.c_str());
            flush();
            return buffer;
        }
        delay(0);
    }
    return "";
}

void SerialSetup::keywordDetector()
{
    if (buffer.indexOf(keyword) >= 0)
    {
        if (!active)
        {
            if(pass.length() > 0)
            {
                if (buffer.indexOf(pass) == -1)
                {
                    Serial.println(F("Password incorrecta"));
                    flush();
                    return;
                }
            }
            switchMode();
            flush();
        }
    }
}

void SerialSetup::setupLoop(callback_t _callback)
{
    callback = _callback;
}

void SerialSetup::listen()
{
    if (Serial.available())
    {
        read();
        keywordDetector();
    }

    if (active)
    {
        (*callback)();

        switchMode();
    }
}