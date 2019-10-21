/*
 * This project implements a simple FSM.
 * If the left button is pushed twice, the word "left" is displayed.
 * If the right button is pushed twice, the word "right" is displayed.
 * If one of each button is pressed, the word "none" is displayed.
 * A third button push (to either left or right) resets the FSM to its original state where it responds to two buttons as stated earlier.
 * Throughout the program, we assume the two buttons are not pushed at the same time
 */

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

/* HAL Includes */
#include "ButtonLED_HAL.h"
#include "Timer32_HAL.h"
#include "LCD_display_HAL.h"

/* Other Includes */

extern HWTimer_t timer0, timer1;

void init_all(Graphics_Context *g_sContext_p);

typedef struct {
    bool left, right, none;
} fsm_output_t;

fsm_output_t simple_FSM(bool, bool);
void respond_to_fsm(fsm_output_t fsm_output, Graphics_Context *g_sContext_p);

int main(void)
{
    Graphics_Context g_sContext;

    init_all(&g_sContext);


    // Launchpad left button
    button_t L_S1;
    initButton(&L_S1, GPIO_PORT_P1, GPIO_PIN1, &timer0);

    // Launchpad right button
    button_t L_S2;
    initButton(&L_S2, GPIO_PORT_P1, GPIO_PIN4, &timer0);

    bool L_S1_Pushed, L_S2_Pushed;
    fsm_output_t fsm_output;
    while(1)
    {
        L_S1_Pushed = ButtonPushed(&L_S1);
        L_S2_Pushed = ButtonPushed(&L_S2);

        fsm_output = simple_FSM(L_S1_Pushed, L_S2_Pushed);

        respond_to_fsm(fsm_output, &g_sContext);

   }
}


void init_leds()
{
    initialize_LaunchpadLED1();
    initialize_LaunchpadLED2_red();

    turnOff_LaunchpadLED1();
    turnOff_LaunchpadLED2_red();

}

void init_buttons()
{
    initialize_LaunchpadRightButton();
    initialize_LaunchpadLeftButton();
}

void init_timers()
{
    initHWTimer0();
    initHWTimer1();
}

void init_all(Graphics_Context *g_sContext_p)
{
    WDT_A_hold(WDT_A_BASE);

    init_leds();
    init_buttons();
    init_timers();
    init_graphics(g_sContext_p);

}





fsm_output_t simple_FSM(bool leftButtonWasPushed, bool rightButtonWasPushed) {

    typedef enum {zero, one_l, one_r, two_ll, two_lr, two_rl, two_rr} fsm_state_t;

    static fsm_state_t state = zero;

    // outputs of the FSM and their default
     bool left = false;
     bool right = false;
     bool none = false;

     switch(state)
     {
     case zero:
         if (leftButtonWasPushed)
         {
             state = one_l;
         }
         else if (rightButtonWasPushed)
         {
             state = one_r;
         }
         break;
     case one_l:
         if (leftButtonWasPushed)
         {
             state = two_ll;
             left = true;
         }
         else if (rightButtonWasPushed)
         {
             state = two_lr;
             none = true;
         }
         break;
     case one_r:
         if (leftButtonWasPushed)
         {
             state = two_rl;
             none = true;
         }
         else if (rightButtonWasPushed)
         {
             state = two_rr;
             right = true;
         }
         break;
     case two_rl:
     case two_rr:
     case two_lr:
     case two_ll:
         if (leftButtonWasPushed || rightButtonWasPushed)
         {
             state = zero;
         }
         break;
     }

     fsm_output_t output;
     output.left = left;
     output.right = right;
     output.none = none;

     return (output);
}

void respond_to_fsm(fsm_output_t fsm_output, Graphics_Context *g_sContext_p) {
    if (fsm_output.left)
    {
        Graphics_drawString(g_sContext_p,"         " , -1, 5, 5, true);
        Graphics_drawString(g_sContext_p,"left" , -1, 5, 5, true);
    }
    if (fsm_output.right)
    {
        Graphics_drawString(g_sContext_p,"         " , -1, 5, 5, true);
        Graphics_drawString(g_sContext_p,"right" , -1, 5, 5, true);
    }
    if (fsm_output.none)
    {
        Graphics_drawString(g_sContext_p,"         " , -1, 5, 5, true);
        Graphics_drawString(g_sContext_p,"none" , -1, 5, 5, true);
    }
}
