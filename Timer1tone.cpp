#include "Timer1Tone.h"

volatile bool toggleState = false;
uint8_t outputPin;

ISR(TIMER1_COMPA_vect) {
    digitalWrite(outputPin, toggleState);
    toggleState = !toggleState;
}

Timer1Tone::Timer1Tone(uint8_t pin) {
    _pin = pin;
    pinMode(_pin, OUTPUT);
    outputPin = _pin;
}

void Timer1Tone::begin(int frequency) {
    setupTimer1(frequency);
}

void Timer1Tone::stop() {
    TCCR1B = 0; // Stop Timer1
    digitalWrite(_pin, LOW);
}

void Timer1Tone::setupTimer1(int frequency) {
    noInterrupts(); // Disable interrupts

    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;

    int prescaler = 8;
    int ocr1a = (16000000 / (2 * prescaler * frequency)) - 1;

    OCR1A = ocr1a;
    TCCR1B |= (1 << WGM12); // CTC mode
    TCCR1B |= (1 << CS11);  // Prescaler = 8
    TIMSK1 |= (1 << OCIE1A); // Enable Timer1 Compare Match Interrupt

    interrupts(); // Enable interrupts
}