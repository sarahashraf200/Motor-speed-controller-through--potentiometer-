#include "motor.h"

/* External INT1 Interrupt Service Routine */
ISR(INT1_vect)
{
	PORTB ^= 0x03;
}

/* External INT1 enable and configuration function */
void INT1_Init(void)
{
	/* Enable external interrupt pin INT1 */
	GICR  |= (1<<INT1);

	/* Trigger INT1 with the raising edge */
	MCUCR |= (1<<ISC11) | (1<<ISC10);
}

void Timer0_PWM_Init(unsigned char set_duty_cycle)
{

	TCNT0 = 0; /* Timer initial value */

	OCR0  = set_duty_cycle; /* compare value */

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}


int main (void){

	uint16 res_value;

		/* Enable interrupts by setting I-bit */
		SREG  |= (1<<7);

		/* Configure INT1/PD3 as input pin */
		DDRD  &= (~(1<<PD3));

		/* Configure PB3/ OC0 as output pin --> pin where the PWM signal is generated from MC */
		DDRB  = DDRB | (1<<PB3);

		/* Configure first two pins in PORTB as output pins (Motor Pins) */
		DDRB  |= (1<<PB0) | (1<<PB1);

		/* motor is rotating clock wise at the beginning */
		PORTB |= (1<<PB0);
		PORTB &= ~(1<<PB1);
		LCD_init(); /* initialize LCD driver */
		ADC_init(); /* initialize ADC driver */
		INT1_Init(); /* Activate INT1 */
		LCD_clearScreen(); /* clear LCD at the beginning */
		/* display this string "ADC Value = " only once at LCD */


		LCD_displayString("ADC Value = ");
	    while(1)
	    {
			LCD_goToRowColumn(0,12); /* display the number every time at this position */
			res_value = ADC_readChannel(0); /* read channel zero where the potentiometer is connect */
			LCD_intgerToString(res_value); /* display the ADC value on LCD screen */
			Timer0_PWM_Init(res_value/4);
	    }
}
