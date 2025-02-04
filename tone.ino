const int pwmPin = 9;  // Use a valid PWM-capable pin
const int pwmChannel = 0;  // Choose a PWM channel (0-15)
const int pwmFreq = 31000;  // High PWM frequency (31kHz)
const int pwmResolution = 8;  // 8-bit resolution (0-255)


// Sine wave lookup table (0-255 range for PWM)
const uint8_t sineTable[81] = {
 127, 135, 143, 151, 159, 167, 175, 183, 190, 198, 205, 212, 219, 225, 232, 238, 243, 248, 253, 255,
 255, 255, 253, 248, 243, 238, 232, 225, 219, 212, 205, 198, 190, 183, 175, 167, 159, 151, 143, 135,
 127, 119, 111, 103, 95, 87, 79, 71, 64, 56, 49, 42, 35, 29, 22, 16, 11, 6, 1, 0,
 0, 0, 1, 6, 11, 16, 22, 29, 35, 42, 49, 56, 64, 71, 79, 87, 95, 103, 111, 119, 127};


int waveIndex = 0;  // Renamed from 'index' to avoid conflicts


void setup() {
 ledcSetup(pwmChannel, pwmFreq, pwmResolution);  // Set up PWM
 ledcAttachPin(pwmPin, pwmChannel);  // Attach pin to PWM channel
}


void loop() {
 ledcWrite(pwmChannel, sineTable[waveIndex]);  // Update PWM with sine value
 waveIndex = (waveIndex + 1) % 81;  // Loop through sine table


 delayMicroseconds(5);  // Adjust to control sine wave frequency
}
