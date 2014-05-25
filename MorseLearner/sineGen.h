//	This will be the code to generate a simple sine wave.  
//
//	It will be used later in making a complete morse dit or dah.
//
//		November 18th, 2013
//
//		Brian Schmitt
//
//
#include <math.h>		//needed for the sin fucntion!
#include <vector>		//going to be used for passing values to main routine
#define pi 3.1415		//define pi for calculations
//
//
void SineGen (double freq, double sample_rate, std::vector<int> &wave_slot) 	//a * is needed because we are returning a pointer!  
															// I will have to see if this will acutally work
															//this way! (the compiler suggested it!)
															
{
	
	// possibly delete ... std::vector<int> wave_slot (441);		//Initialize vector with 441 zeros!
	double slice_count; slice_count=0;		//variable to be used for number of 'slices'/samples in sample wave
	double slice_sample;						//variable to be used for currently calculated sample slice period
	int x; x=0;								//variable to be used for counter
	//double* wave_slot; no more pointers!	//variable to be used for memory pointer to sine wave data sample
	slice_count = sample_rate/freq;			//set sample length
	//double wave_slot[int(slice_count)];	//<---- This will be a VECTOR! temp set to 441! For ease of debugging
	//
	//		create sine wave with loop and calculation
	//
	while (x<=slice_count) {
		wave_slot[x] = sin(freq * (2 * pi) * x / sample_rate);
		x++;
		};
	
return ; 	
// Sine is being calcuated correctly, but memory is being destroyed after routine ends!
// Move to int main ????  Easy !
//

//******************************************Notes!***********************************
//
//
//
// 					a sine wave reference : http://www.topherlee.com/software/pcm-tut-sine.html
//
//	Excerpt from :
//  http://www.daniweb.com/software-development/c/threads/354944/creating-a-sine-wave-at-a-specific-frequency-and-sample-rate
	
	//    for(i = 0; i < 10000; i++)
    //{
    // i is the sample index
    // Straight sine function means one cycle every 2*pi samples:
    // buffer[i] = sin(i);
    // Multiply by 2*pi--now it's one cycle per sample:
    // buffer[i] = sin((2 * pi) * i);
    // Multiply by 1,000 samples per second--now it's 1,000 cycles per second:
    // buffer[i] = sin(1000 * (2 * pi) * i);
    // Divide by 44,100 samples per second--now it's 1,000 cycles per 44,100
    // samples, which is just what we needed:
    //
	//
	//					buffer[i] = sin(1000 * (2 * pi) * i / 44100); <==== Use this!
    //}
	//
	
	//
	// How to return an array....
	//http://www.cplusplus.com/forum/beginner/6644/
/*int *f(size_t s){
	int *ret=new int[s];
	for (size_t a=0;a<s;a++)
		ret[a]=a;
	return ret;
}*/
