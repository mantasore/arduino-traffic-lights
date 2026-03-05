/*-------------Temporary code-------------*/

// This block code is for intelisense to shut the fuck up

#include <cstdint>

enum LEVEL
{
    LOW,
    HIGH,
    INPUT,
    OUTPUT,
    RISING
};

void digitalWrite(int a, enum LEVEL);
void pinMode(int a, enum LEVEL);
void delay(int a);
void attachInterrupt(int a, void(*func), enum LEVEL);
int digitalPinToInterrupt(int a);
int millis();
enum LEVEL digitalRead(int a);
/*----------------------------------------*/

#define YELLOW_LIGHT_DELAY 1000
#define RED_GREEN_LIGHT_DELAY 7000
#define PEDESTRIAN_LIGHT_DELAY 10000

const uint8_t ledPins[] = {13, 12, 11, 10, 9, 8, 7, 6}; // Pins to initialize as output pins

// Order of the traffic lights
const uint8_t ledOrder[][4] = { {8, 13}, // {1st Pin, 2nd Pin}
                                {8, 13},
                                {10, 0},
                                {12, 0},
                                {11, 0}};
const unsigned int ledOrderSize = sizeof(ledOrder) / sizeof(ledOrder[0]);

uint8_t previousBuffer[2] = {};

unsigned int currentPins = 0;

volatile unsigned long lastInterruptTime = 0;
volatile bool buttonPressed = false;

void buttonInterrupt()
{
    //Button Deboucer
    unsigned long interruptTime = millis();

    if (interruptTime - lastInterruptTime > 100)
    {
        buttonPressed = true;
        lastInterruptTime = interruptTime;
    }
}

void setup()
{
    for (int pin : ledPins)
    {
        pinMode(pin, OUTPUT);
    }

    pinMode(3, INPUT);

    attachInterrupt(digitalPinToInterrupt(3), buttonInterrupt, RISING);

    digitalWrite(7, HIGH);
}

int lastButtonState = 0;

void loop()
{
    // This is just a parser 

    // Check if we really need to turn of the previous leds
    if(ledOrder[currentPins][0] != previousBuffer[0])
    {
        digitalWrite(ledOrder[currentPins][0], LOW);
    }
    else if(ledOrder[currentPins][1] != previousBuffer[1])
    {
        digitalWrite(ledOrder[currentPins][1], LOW);
    }

    previousBuffer[0] = ledOrder[currentPins][0];
    previousBuffer[1] = ledOrder[currentPins][1];

    // Check if pin corresponds to a yellow light pin
    if (ledOrder[currentPins][0] == 12 || ledOrder[currentPins][0] == 9)
    {
        digitalWrite(ledOrder[currentPins][0], HIGH);
        delay(YELLOW_LIGHT_DELAY);
    }
    else
    {
        digitalWrite(ledOrder[currentPins][0], HIGH);
        digitalWrite(ledOrder[currentPins][1], HIGH);
        delay(RED_GREEN_LIGHT_DELAY);

        // Do this if the button was pressed
        if (buttonPressed)
        {
            buttonPressed = false;

            digitalWrite(12, HIGH);
            digitalWrite(9, HIGH);
            delay(YELLOW_LIGHT_DELAY);
            digitalWrite(12, LOW);
            digitalWrite(9, LOW);

            digitalWrite(13, HIGH);
            digitalWrite(10, HIGH);
            digitalWrite(7, LOW);
            digitalWrite(6, HIGH);
            delay(PEDESTRIAN_LIGHT_DELAY);
            digitalWrite(13, LOW);
            digitalWrite(10, LOW);
            digitalWrite(7, HIGH);
            digitalWrite(6, LOW);
        }
    }

    // Check if the cycle has ended
    if (currentPins >= ledOrderSize - 1)
    {
        currentPins = 0;
    }
    else
    {
        currentPins++;
    }
}