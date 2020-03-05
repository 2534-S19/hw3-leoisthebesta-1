#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework3.h"
#include "myGPIO.h"
#include "myTimer.h"


typedef enum {UP,DOWN} pushbutton_state;
int main(void)
{
    // Count variables to control the LEDs.
    unsigned int count0 = 0;
    unsigned int count1 = 0;


    // TODO: Declare the variables that main uses to interact with your state machine.
    uint8_t history = 0xFF;// in order to set the button history button
    bool press = false;

    // Stops the Watchdog timer.
    initBoard();
    // Initialize the GPIO.
    // YOU MUST WRITE THIS FUNCTION IN myGPIO.c
    initGPIO();
    // Initialize Timer0 to provide a one second count interval for updating LED2.
    // YOU MUST WRITE THIS FUNCTION IN myTimer.c
    initTimer(TIMER32_0_BASE, TIMER0_PRESCALER, TIMER0_COUNT);
    // Initialize Timer1 to provide a one millisecond count interval for updating the button history.
    // YOU MUST WRITE THIS FUNCTION IN myTimer.c
    initTimer(TIMER32_1_BASE, TIMER1_PRESCALER, TIMER1_COUNT);

    while(1)
    {
        // Update the color of LED2 using count0 as the index.
        // YOU MUST WRITE THIS FUNCTION BELOW.
        changeLaunchpadLED2(count0);
        // Update the color of the Boosterpack LED using count1 as the index.
        // YOU MUST WRITE THIS FUNCTION BELOW.
        changeBoosterpackLED(count1);

        // TODO: If Timer0 has expired, increment count0.
        // YOU MUST WRITE timer0expired IN myTimer.c

        if(timer0Expired())
        {
            if(count0 == 7)//stop at 7 and loop to 0
           {
                 count0 = 0;
           }
            else
                 count0 += 1;
        }

        // TODO: If Timer1 has expired, update the button history from the pushbutton value.
        // YOU MUST WRITE timer1expired IN myTimer.c
        if(timer1Expired())
        {
           if(checkStatus_BoosterpackS1() == 0)
           {
               history = history*2 ;//  if buttom pressed, history will become 0
           }
           else
           {
               history = history*2+1;//not pressed, history will become 1
           }
        }


        // TODO: Call the button state machine function to check for a completed, debounced button press.
        // YOU MUST WRITE THIS FUNCTION BELOW.

        press = fsmBoosterpackButtonS1(history);

        // TODO: If a completed, debounced button press has occurred, increment count1.


        if(press == true)
        {
            if(count1 == 7)
            {
                count1 = 0;
            }
            else
                count1 += 1;
        }

    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: Map the value of a count variable to a color for LED2.
// Since count is an unsigned integer, you can mask the value in some way.
void changeLaunchpadLED2(unsigned int count)
{
   switch (count){
       case 0:
           turnOff_LaunchpadLED2Red();
           turnOff_LaunchpadLED2Blue();
           turnOff_LaunchpadLED2Green();
           break;
       case 1:
                turnOn_LaunchpadLED2Red();//red
                turnOff_LaunchpadLED2Blue();
                turnOff_LaunchpadLED2Green();
                break;
       case 2:
                turnOff_LaunchpadLED2Red();//green
                turnOff_LaunchpadLED2Blue();
                turnOn_LaunchpadLED2Green();
                break;
       case 3:
                turnOff_LaunchpadLED2Red();  //Blue and green
                turnOn_LaunchpadLED2Blue();
                turnOn_LaunchpadLED2Green();
                break;
       case 4:
                turnOff_LaunchpadLED2Red(); //blue
                turnOn_LaunchpadLED2Blue();
                turnOff_LaunchpadLED2Green();
                break;
       case 5:
                turnOn_LaunchpadLED2Red();//blue and red
                turnOn_LaunchpadLED2Blue();
                turnOff_LaunchpadLED2Green();
                break;
       case 6:
                turnOn_LaunchpadLED2Red(); //red and green
                turnOff_LaunchpadLED2Blue();
                turnOn_LaunchpadLED2Green();
                break;
       case 7:
                turnOn_LaunchpadLED2Red(); //red and blue
                turnOn_LaunchpadLED2Blue();
                turnOff_LaunchpadLED2Green();
                break;
     // just in case since there are clocks
       default:
                turnOff_LaunchpadLED2Red();
                turnOff_LaunchpadLED2Blue();
                turnOff_LaunchpadLED2Green();
                break;


   }

}

// TODO: Maybe the value of a count variable to a color for the Boosterpack LED
// This is essentially a copy of the previous function, using a different LED
void changeBoosterpackLED(unsigned int count)
{
    switch (count){
        case 0:
            turnOff_BoosterpackLEDRed();
            turnOff_BoosterpackLEDBlue();
            turnOff_BoosterpackLEDGreen();
            break;
        case 1:
                 turnOn_BoosterpackLEDRed();//red
                 turnOff_BoosterpackLEDBlue();
                 turnOff_BoosterpackLEDGreen();
                 break;
        case 2:
                 turnOff_BoosterpackLEDRed();//green
                 turnOff_BoosterpackLEDBlue();
                 turnOn_BoosterpackLEDGreen();
                 break;
        case 3:
                 turnOff_BoosterpackLEDRed();  //Blue and green
                 turnOn_BoosterpackLEDBlue();
                 turnOn_BoosterpackLEDGreen();
                 break;
        case 4:
                 turnOff_BoosterpackLEDRed(); //blue
                 turnOn_BoosterpackLEDBlue();
                 turnOff_BoosterpackLEDGreen();
                 break;
        case 5:
                 turnOn_BoosterpackLEDRed();//blue and red
                 turnOn_BoosterpackLEDBlue();
                 turnOff_BoosterpackLEDGreen();
                 break;
        case 6:
                 turnOn_BoosterpackLEDRed(); //red and green
                 turnOff_BoosterpackLEDBlue();
                 turnOn_BoosterpackLEDGreen();
                 break;
        case 7:
                 turnOn_BoosterpackLEDRed(); //red and blue
                 turnOn_BoosterpackLEDBlue();
                 turnOff_BoosterpackLEDGreen();
                 break;
        default:
                 turnOff_BoosterpackLEDRed();
                 turnOff_BoosterpackLEDBlue();
                 turnOff_BoosterpackLEDGreen();
                 break;
      }
}

// TODO: Create a button state machine.
// The button state machine should return true or false to indicate a completed, debounced button press.
bool fsmBoosterpackButtonS1(unsigned int buttonhistory)
{
    bool pressed = false;
    pushbutton_state state = UP;
    switch(state){
        case UP:
            if(buttonhistory == 1) // if unpressed , means that a user has finished a push.
            {
                state = DOWN;
                pressed = true;
            }
            break;
        case DOWN:
            if(buttonhistory == 0)// still pushing
            {
                state = UP;
                pressed = false;
            }
            break;

                 }



    return pressed;
}
