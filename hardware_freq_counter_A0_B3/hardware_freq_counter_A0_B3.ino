/*
 * Example of the Timer Input Capture mode to measure PWM pulse width and period.
 *  
 * This example uses:
 * - Timer2 channel 1 as capture input
 * - Timer1 to generate a PWM trigger signal for capture input
 */

#include <Streaming.h>
long freq1,freq2,timerr1,timerr2;

int flag1,flag2;
const uint8_t pwmPulse = 20; // ms
const uint8_t pwmPeriod = 50; // ms
//-----------------------------------------------------------------------------
void toggle_led()
{
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
//-----------------------------------------------------------------------------
void setup()
{

	// setup PA0 (Timer 2 channel 1) as input capture mode
	pinMode(PA0, INPUT);

	// stop the timers before configuring them
	Timer2.pause();

	Timer2.setPrescaleFactor(72); // 1 microsecond resolution

	// setup timer 2 channel 1 capture on rising edge
	Timer2.setInputCaptureMode(TIMER_CH1, TIMER_IC_INPUT_DEFAULT); // use default input TI1
	// setup timer 2 channel 2 capture on falling edge
	Timer2.setInputCaptureMode(TIMER_CH2, TIMER_IC_INPUT_SWITCH); // use switched input TI1
	Timer2.setPolarity(TIMER_CH2, 1); // trigger on falling edge

	// counter setup as slave triggered by TI1 in reset mode
	Timer2.setSlaveFlags( TIMER_SMCR_TS_TI1FP1 | TIMER_SMCR_SMS_RESET );

	Timer2.refresh();
	Timer2.getCompare(TIMER_CH1); // clear capture flag
	Timer2.getCompare(TIMER_CH2); // clear capture flag
	Timer2.resume(); // let timer 2 run


  pinMode(PA8, INPUT);
 // stop the timers before configuring them
 Timer1.pause();

  Timer1.setPrescaleFactor(72); // 1 microsecond resolution

  // setup timer 2 channel 1 capture on rising edge
  Timer1.setInputCaptureMode(TIMER_CH1, TIMER_IC_INPUT_DEFAULT); // use default input TI1
  // setup timer 2 channel 2 capture on falling edge
  Timer1.setInputCaptureMode(TIMER_CH2, TIMER_IC_INPUT_SWITCH); // use switched input TI1
  Timer1.setPolarity(TIMER_CH2, 1); // trigger on falling edge

  // counter setup as slave triggered by TI1 in reset mode
  Timer1.setSlaveFlags( TIMER_SMCR_TS_TI1FP1 | TIMER_SMCR_SMS_RESET );

  Timer1.refresh();
  Timer1.getCompare(TIMER_CH1); // clear capture flag
  Timer1.getCompare(TIMER_CH2); // clear capture flag
  Timer1.resume(); // let timer 2 run
}
//-----------------------------------------------------------------------------
void loop()
{
if(Timer2.getInputCaptureFlag(TIMER_CH1))
{timerr1=millis();flag1=0;}
if(millis()-timerr1>100)flag1=1;
freq1=1000000/Timer2.getCompare(TIMER_CH1);

if(Timer1.getInputCaptureFlag(TIMER_CH1))
{timerr2=millis();flag2=0;}
if(millis()-timerr2>100)flag2=1;
freq2=1000000/Timer1.getCompare(TIMER_CH1);



    Serial << ", period: " << freq1<< "  " << flag1 
    << ", period2: " << freq2<< "  " << flag2   << endl;
}
