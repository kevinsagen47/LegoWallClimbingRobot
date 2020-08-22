 /*
 * Example of the Timer Input Capture mode to measure PWM pulse width and period.
 *  
 * This example uses:
 * - Timer2 channel 1 as capture input
 * - Timer3 to generate a PWM trigger signal for capture input
 */

#include <Streaming.h>

long freq,freq2, flag,timer;

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void setup()
{
	// setup PA0 (Timer 2 channel 1) as input capture mode
	pinMode(PA0, INPUT);
  pinMode(PA1, INPUT);
	// stop the timers before configuring them
	Timer2.pause();
	//Timer3.pause();

	Timer2.setPrescaleFactor(72); // 1 microsecond resolution

	// setup timer 2 channel 1 capture on rising edge
	Timer2.setInputCaptureMode(TIMER_CH1, TIMER_IC_INPUT_DEFAULT); // use default input TI1
	// setup timer 2 channel 2 capture on falling edge
	Timer2.setInputCaptureMode(TIMER_CH3, TIMER_IC_INPUT_SWITCH); // use switched input TI1
	Timer2.setPolarity(TIMER_CH3, 1); // trigger on falling edge

    Timer2.setInputCaptureMode(TIMER_CH2, TIMER_IC_INPUT_DEFAULT); // use default input TI1
  // setup timer 2 channel 2 capture on falling edge
 Timer2.setInputCaptureMode(TIMER_CH4, TIMER_IC_INPUT_SWITCH); // use switched input TI1
 Timer2.setPolarity(TIMER_CH4, 1); // trigger on falling edge


	// counter setup as slave triggered by TI1 in reset mode
	Timer2.setSlaveFlags( TIMER_SMCR_TS_TI1FP1 | TIMER_SMCR_SMS_RESET );
  //Timer2.setSlaveFlags( TIMER_SMCR_TS_TI2FP2 | TIMER_SMCR_SMS_RESET );

	Timer2.refresh();
	Timer2.getCompare(TIMER_CH1); // clear capture flag
	Timer2.getCompare(TIMER_CH3); // clear capture flag
 Timer2.getCompare(TIMER_CH2);
 Timer2.getCompare(TIMER_CH4);
	Timer2.resume(); // let timer 2 run


}
//-----------------------------------------------------------------------------
void loop()
{

if(Timer2.getInputCaptureFlag(TIMER_CH1))
{timer=millis();flag=0;}
if(millis()-timer>100)flag=1;
freq=1000000/Timer2.getCompare(TIMER_CH1);
		Serial << ", period: " << freq<< "  " << flag 
		<< ", period2: " << freq2<< "  " << flag   << endl;
freq2=1000000/Timer2.getCompare(TIMER_CH2);

		//delay(500); // read the values only 2 times per second
	
}
