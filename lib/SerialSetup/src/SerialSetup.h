#ifndef _SERIAL_SETUP_H_
#define _SERIAL_SETUP_H_

typedef void (*callback_t)(void);

#include <Arduino.h>

class SerialSetup
{
private:
    const String keyword;
    const uint8_t timeout;
    const String pass;
    

    String buffer;
    bool active;
    unsigned long lastMs;

    callback_t callback;

    void switchMode();
    void flush();


    void read();

    void keywordDetector();

public:
    SerialSetup(String wakeup_word = "setup", uint8_t _timeout = 60, String _pass = "") : 
        keyword(wakeup_word),
        timeout(_timeout),
        pass(_pass)
    {
        buffer.reserve(64);
    }
    ~SerialSetup() {}

    String get(const char* message);

    void setupLoop(callback_t _cback);
    void listen();
};




#endif