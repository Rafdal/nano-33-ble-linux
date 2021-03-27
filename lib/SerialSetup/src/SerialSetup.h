#ifndef _SERIAL_SETUP_H_
#define _SERIAL_SETUP_H_

#include <Arduino.h>

typedef void (*void_callback_t)(void);
typedef void (*int_callback_t)(int);
typedef void (*uint8_callback_t)(uint8_t);
typedef void (*float_callback_t)(float);
typedef void (*str_callback_t)(String);

#ifdef DEBUG
#define DEBUG(str) Serial.print(F("debug: \""));Serial.println(str+'\"')
#else
#define DEBUG(str)
#endif

class SerialSetup
{
private:
    const String pass;
    const String _keyword;
    const uint8_t timeout;
    

    String buffer;
    bool active=false, first_loop=false;
    unsigned long lastMs;

    void_callback_t callback;

    void switchMode();
    void flush();


    void read();

    void keywordDetector();

    void printOption(const char* keyword, const char* type);

public:
    SerialSetup(String _pass = "", String wakeup_word = "setup", uint8_t _timeout = 60) : 
        pass(_pass),
        _keyword(wakeup_word),
        timeout(_timeout)
    {
        buffer.reserve(256);
    }
    ~SerialSetup() {}

    void on(const char* keyword, int_callback_t c);
    void on(const char* keyword, str_callback_t c);
    void on(const char* keyword, void_callback_t c);

    // String get(const char* message);

    void setupLoop(void_callback_t _cback);
    void listen();
};




#endif