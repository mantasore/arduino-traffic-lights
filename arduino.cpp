/*=============Temporary code=============*/

// This code block is for intelisense to shut the fuck up

#include <cstdint>

enum LEVEL
{
    LOW,
    HIGH,
    INPUT,
    OUTPUT,
    RISING,
    INPUT_PULLUP
};

void digitalWrite(int a, enum LEVEL);
void pinMode(int a, enum LEVEL);
void delay(int a);
void attachInterrupt(int a, void(*func), enum LEVEL);
int digitalPinToInterrupt(int a);
int millis();
enum LEVEL digitalRead(int a);
/*========================================*/

#define YELLOW_LIGHT_DELAY 1000
#define GREEN_LIGHT_DELAY 7000
#define PEDESTRIAN_LIGHT_DELAY 10000

const uint8_t ledPins[] = {13, 12, 11, 10, 9, 8, 7, 6}; // Pins to initialize as output pins

// Order of the traffic lights
const uint8_t ledOrder[][3] = { {8, 9, 10},
                                {13, 12, 11},
                                {11, 12, 13},
                                {10, 9, 8}
                                };
const unsigned int ledOrderSize = sizeof(ledOrder) / sizeof(ledOrder[0]);

unsigned int currentPins = 0;

volatile unsigned long lastInterruptTime = 0;
volatile bool buttonPressed = false;

void buttonInterrupt()
{
    //Button Debouncer
    unsigned long interruptTime = millis();

    if (interruptTime - lastInterruptTime > 100)
    {
        buttonPressed = true;
        lastInterruptTime = interruptTime;
    }
}

void switchTrafficLights(uint8_t firstPin, uint8_t yellowLightpin, uint8_t finalPin)
{
    digitalWrite(firstPin, LOW);
    digitalWrite(yellowLightpin, HIGH);
    delay(YELLOW_LIGHT_DELAY);
    digitalWrite(yellowLightpin, LOW);
    digitalWrite(finalPin, HIGH);
}

void switchPedestrianLights()
{
    digitalWrite(7, LOW);
    digitalWrite(6, HIGH);
    delay(PEDESTRIAN_LIGHT_DELAY);
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
}

void setup()
{
    for (int pin : ledPins)
    {
        pinMode(pin, OUTPUT);
    }

    pinMode(3, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(3), buttonInterrupt, RISING);

    digitalWrite(7, HIGH);
}


void loop()
{
    if (buttonPressed)
    {
        buttonPressed = false;

        if (ledOrder[currentPins-1][0] == 8 || ledOrder[currentPins-1][2] == 11)
        {
            currentPins--;

            switchTrafficLights( ledOrder[currentPins][2], 
                                 ledOrder[currentPins][1], 
                                 ledOrder[currentPins][0] );

            switchPedestrianLights();
        }
        else
        {
            switchPedestrianLights();
        }
    }

    switchTrafficLights( ledOrder[currentPins][0], 
                         ledOrder[currentPins][1], 
                         ledOrder[currentPins][2] );
    
    if (currentPins % 2 != 0)
    {
        delay(GREEN_LIGHT_DELAY);
    }
    else
    {
        delay(1000);
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