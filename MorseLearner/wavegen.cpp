//  01/20/2014
//
//This code was copied to create portable code for use elsewhere. Not tested for accurate
// creation of a proper dit or dah wave!
//

// this code creates a proper length dit....maybe 11/23/2013..fixed using Scilab! 12/2/2013!
//
# include <math.h>
#ifndef pi
#define pi (3.145)
#endif


int wavegen (float freqc = 800, float duration_ms = 1000, 
				float begin_ramp = 100, float end_ramp = 100, 
				float sample_rate = 44100)
				{
					// this code creates a proper length dit....maybe 11/23/2013..fixed using Scilab! 12/2/2013!
//
float adjust_amp = 0;
float slice_value = 0;
const int sw_duration_length =((duration_ms / 1000)*(freqc));
float wave_slot[sw_duration_length];
int x=0;

while (x<=begin_ramp)
	{
	//loop for first amplitude taper
	adjust_amp = ((x)/begin_ramp);
	slice_value = ((sin(freqc * (2 * pi) * x / sample_rate))*adjust_amp);
	wave_slot[x] = (slice_value);
			//wave_slot.push_back(slice_value);
	x++;
	}

while (x < ((sw_duration_length) - end_ramp)) 
	{
	//loop for rest of sine wave at full amplitude
	slice_value = (sin(freqc * (2 * pi) * x / sample_rate));	
	wave_slot[x] = slice_value;
	x++;
	}

while (x<=sw_duration_length)
	{
	//loop for final taper of sound wave
	adjust_amp = (((sw_duration_length-x))/end_ramp);
	slice_value = (sin(freqc * (2 * pi) * x / sample_rate))*adjust_amp;	
	wave_slot[x] = slice_value;
	x++;
	}
return 0;			
	}
				

