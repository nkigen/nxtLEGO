#ifndef __OPT_DECODER_H__
#define __OPT_DECODER_H__
/*
 * Options to be decoded include:
 */
/*******Setting Motor Powers*******/
/*
 * Here we are interested in fetching samples of the revolutions from the 
 * LEGO motor by changing the MOTOR powers
 * -p PORT_NO -m MIN_POWER:MAX_POWER:STEP -n NUM_SAMPLES_PER_POWER_VALUE
 * PORT_NO : The LEGO motor to be used (MOTOR_A / MOTOR_B / MOTOR_C)
 * MIN_POWER: The initial power of the motor ( >= -100 && < 100 )
 * MAX_POWER: Maximum power to be used ( >-100 && <= 100)
 * STEP: Difference between consecutive powers set to the motor (Less than 5 sounds nice!!)
 * NUM_SAMPLES_PER_VALUE : Sample size for each value (obviously unsigned :-) )
 */

int decode_options(char *opts);


#endif
