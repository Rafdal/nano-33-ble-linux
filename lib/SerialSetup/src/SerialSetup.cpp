#include "SerialSetup.h"

// Cambia modo y avisa
void SerialSetup::switchMode()
{
    active = !active;
    if (active)
    {
        Serial.println(F("Setup abierto. Puede usar los siguientes comandos:"));
    }
    else
    {
        Serial.println(F("Ha salido del modo configuracion"));
    }
    first_loop = false;
    buffer="";
}

void SerialSetup::flush()
{
    delay(50);
    while (Serial.available())
        Serial.read();
}

// Lee una linea y guarda en el buffer
void SerialSetup::read()
{
    if (Serial.available())
    {
        buffer="";
        lastMs = millis();
        while (lastMs + 5000UL >= millis())
        {
            if (Serial.available())
            {
                lastMs = millis();
                char c = Serial.read();
                Serial.print(c);
                if (c == '\n' || c == '\r')
                {
                    Serial.println('>'+buffer);
                    flush();
                    return;
                }                
                buffer += c;
            }
            delay(0);
        }
        Serial.println(F("Timeout! Intenta escribir mas rapido"));
        flush();
        buffer = "";
    }
}

/* String SerialSetup::get(const char* message)
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
} */

// Escucha pasivamente, si detecta keyword cambia de modo
void SerialSetup::keywordDetector()
{
    if (buffer.length() > 1)
    {
        if (buffer.indexOf(_keyword) >= 0)
        {
            if (!active)
            {
                if(pass.length() > 0)
                {
                    if (buffer.indexOf(pass) == -1)
                    {
                        delay(2000);
                        Serial.println(F("Password incorrecta"));
                        flush();
                        return;
                    }
                }
            }
            switchMode();
            flush();
        }
        if (buffer.indexOf(F("exit")) >= 0)
        {
            if (active)
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
}

void SerialSetup::on(const char* keyword, int_callback_t c)
{
    printOption(keyword, "int");
    if (buffer.length() > 1)
    {
        if (buffer.indexOf(keyword) >= 0)
        {
            buffer.remove(0, sizeof(keyword)+1);
            DEBUG(buffer);

            int output=0;
            sscanf(buffer.c_str(), "%u", &output);
            (*c)(output);
            buffer="";
            flush();
        }
    }
}
void SerialSetup::on(const char* keyword, str_callback_t c)
{
    printOption(keyword, "text");

    if (buffer.length() > 1)
    {
        if (buffer.indexOf(keyword) >= 0)
        {
            buffer.remove(0, sizeof(keyword)+1);
            DEBUG(buffer);

            (*c)(buffer);
            buffer = "";
            flush();
        }
    }
}

void SerialSetup::on(const char* keyword, void_callback_t c)
{
    printOption(keyword, "void");

    if (buffer.length() > 1)
    {
        if (buffer.indexOf(keyword) >= 0)
        {
            if(c != NULL)
                (*c)();
            buffer = "";
            flush();
        }
    }
}

void SerialSetup::setupLoop(void_callback_t _callback)
{
    callback = _callback;
}

void SerialSetup::printOption(const char* keyword, const char* type)
{
    if (!first_loop)
    {
        Serial.print('[');
        Serial.print(keyword);
        Serial.print(F(" ("));
        Serial.print(type);
        Serial.println(F(") ]"));   
    }
}

void SerialSetup::listen()
{
    if (active)
    {
        read();
        (*callback)();
        first_loop = true;
        unsigned long currentMs = millis();
        if (currentMs - lastMs > (((unsigned long)timeout)*1000UL))
        {
            Serial.println(F("Timeout!"));
            switchMode();
        }

        keywordDetector();
            
        if (buffer.length() > 0)
        {
            Serial.print('"');
            Serial.print(buffer);
            Serial.println(F("\" comando incorrecto!"));
            buffer="";
        }
    }
    else if (Serial.available())
    {
        read();
        keywordDetector();
    }
    
}