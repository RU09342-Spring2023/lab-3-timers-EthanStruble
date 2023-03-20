/*
 * Part2.c
 *
 *  Created on: Feb 11, 2023
 *      Author: Russell Trafford
 *
 *      This code will need to change the speed of an LED between 3 different speeds by pressing a button.
 */

#include <msp430.h>

void gpioInit();
void timerInit();
int Speed = 0;      //incrementing the speed of LED
void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    gpioInit();
    timerInit();
    PM5CTL0 &= ~LOCKLPM5;
    __bis_SR_register(LPM3_bits | GIE);
}


void gpioInit(){
    //1.0 Config(Red LED)
    P1OUT &= ~BIT0;
    P1DIR |= BIT0;
    //2.3 config(button)
    P2OUT |= BIT3;                          //Configure P2.3 as pulled-up
    P2REN |= BIT3;                           //P2.3 pull-up register enable
    P2IES &= ~BIT3;                         //P2.3 Low --> High edge
    P2IE |= BIT3;                           //P2.3 interrupt enabled

}
void timerInit(){

    TB1CTL = TBSSEL_1 | MC_2;           //Using ACLK in continuous mode
    TB1CCR0 = 50000;                    //Setting TimerB1 max to 50000
    TB1CCTL0 = CCIE;                    //Making the timer an interrupt
}

/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;         //Clearing button for any interrupts
    Speed +=1;              //Increasing speed by one
}

// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    P1OUT ^= BIT0;  //Toggle Red LED
    if(Speed==1)
    {
      TB1CCR0 += 40000;         //CCR0 = Very slow blinking speed
    }
    else if(Speed==2)
    {
      TB1CCR0 += 4000;        //CCR0 = Mid blinking speed
    }
    else if(Speed==3)
    {
        TB1CCR0 += 400;      //CCR0 = Fast blinking speed
        Speed=0;             //Reset Condition
    }
}
