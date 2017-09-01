#include <ButtonV2.h>

//globals
// pins
const int ledPin = 0;
const int brightDialPin = 2; //analogRead 2 is physical pin 4, which is also used for USB -> remove trinket from circuit before programming
const int buttonPin = 1;  //this is physical pin 1
const int PeriodDialPin = 1; //analogRead 1 is physical pin2

//global variables
long mBrightness = 0;
long mPeriod = 0;
long mLastElapsedTime = 0;
int mLEDState = LOW;

//button class for making things easy
ButtonV2 button;



//function prototypes
//  modes
void changeState();
void brightnessMode();
void blinkMode();
void timerMode();

//setter/getter functions
int getBrightDialReading();
void setBrightness(long brightness);
long getBrightness();

int getPeriodDialReading();
void setPeriod(long period);
long getPeriod();

enum mode
{
  BRIGHT, BLINK, TIMER
}
lampMode;

void setup()
{
  //set LED output pin to OUTPUT
  pinMode(ledPin, OUTPUT);

  //set up button input pin
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, LOW);

  //setup button class
  // button pressed when pin is HIGH, 750ms is window for multiple presses
  button.SetStateAndTime(HIGH, 750);

  //default state for LEDs
  lampMode = BRIGHT;
}

void loop()
{
    //read and set brightness - rescale dialReading from 0-1023 -> 0-255
    setBrightness((getBrightDialReading()) / 4);

    //read and set blink period scaling and offset
    // - scaling to spread out the rage of possible periods, 2 seems good
    // - offset to set a min blink period > 0, 50 seems decent
    setPeriod((4 * (getPeriodDialReading())) + 50);


  //run mode based on lamp state
  if (lampMode == BRIGHT)
    brightnessMode();
  if (lampMode == BLINK)
    blinkMode();
  if (lampMode == TIMER)
    timerMode();

  //determine changes in lamp state
  int type = button.CheckButton(buttonPin); // current time and length of time to press the button as many times as you can ie. 1.5 seconds
  switch (type)
  {
    //don't do anything if button hasn't been pressed
    case WAITING:
      break;
      
    //one press is bright mode
    case PRESSED:
      changeState();
      break;
      
    //two presses is nothing -> not reliable!
    case DOUBLE_PRESSED:
      break;
      
    //more than two presses is nothing -> not reliable!
    case MULTI_PRESSED:
      break;
      
    //hold button for >750ms for timer mode
    case HELD:
      lampMode = TIMER;
      break;
  }

}

//toggle between BLINK and BRIGHT
void changeState()
{
    if (lampMode == BRIGHT)
        lampMode = BLINK;
    else
        lampMode = BRIGHT;
}

void brightnessMode()
{
  //analogWrite is PWM - range from 0(off) to 255(on)
  analogWrite(ledPin, getBrightness());
}

void blinkMode()
{
  //get time from last state change and if greater than the half-period, change state
  int currentElapsedTime = millis() - mLastElapsedTime;
  
  //if elapsed time is greater than a half the period, change states
  if (currentElapsedTime > getPeriod()/2)
  {
    //reset the lastElapsedTime to current time
    mLastElapsedTime = millis();

    //flip the state of the LED
    if (mLEDState == LOW)
    {
      mLEDState = HIGH;
      //use getBrightness to limit the Brightness level as set in BRIGHT mode
      analogWrite(ledPin, getBrightness());
    }
    else
    {
      mLEDState = LOW;
      analogWrite(ledPin, LOW);
    }

  }


}

void timerMode()
{
  analogWrite(ledPin, 0);
  //need to implement time-out/wait
}


//setter/getter functions
int getBrightDialReading()
{
  return analogRead(brightDialPin);
}

void setBrightness(long brightness)
{
  mBrightness = brightness;
}

long getBrightness()
{
  return mBrightness;
}

int getPeriodDialReading()
{
    return analogRead(PeriodDialPin);
}

void setPeriod(long period)
{
    mPeriod = period;
}

long getPeriod()
{
    return mPeriod;
}


