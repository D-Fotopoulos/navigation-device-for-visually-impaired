#ifndef TIMER1TONE_H
#define TIMER1TONE_H

#include <Arduino.h>

class Timer1Tone {
public:
    Timer1Tone(uint8_t pin);
    void begin(int frequency);
    void stop();

private:
    uint8_t _pin;
    static void setupTimer1(int frequency);
};

#endif