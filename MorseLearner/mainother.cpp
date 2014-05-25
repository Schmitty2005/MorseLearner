/** @file paex_sine.c
	@ingroup examples_src
	@brief Play a sine wave for several seconds.
	@author Ross Bencina <rossb@audiomulch.com>
    @author Phil Burk <philburk@softsynth.com>
*/
/*
 * $Id: paex_sine.c 1752 2011-09-08 03:21:55Z philburk $
 *
 * This program uses the PortAudio Portable Audio Library.
 * For more information see: http://www.portaudio.com/
 * Copyright (c) 1999-2000 Ross Bencina and Phil Burk
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The text above constitutes the entire PortAudio license; however, 
 * the PortAudio community also makes the following non-binding requests:
 *
 * Any person wishing to distribute modifications to the Software is
 * requested to send the modifications to the original developer so that
 * they can be incorporated into the canonical version. It is also 
 * requested that these non-binding requests be included along with the 
 * license above.
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "portaudio.h"
#include "string.h"

//Defines for Port Audio usage
/* #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (1024)
#define NUM_CHANNELS    (2)
#define NUM_SECONDS     (15)
/* #define DITHER_FLAG     (paDitherOff)  */
#define DITHER_FLAG     (0) /**/

/* @todo Underflow and overflow is disabled until we fix priming of blocking write. */
#define CHECK_OVERFLOW  (0)
#define CHECK_UNDERFLOW  (0)


/* Select sample format. */
#if 0
#define PA_SAMPLE_TYPE  paFloat32
#define SAMPLE_SIZE (4)
#define SAMPLE_SILENCE  (0.0f)
#define CLEAR(a) memset( (a), 0, FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE )
#define PRINTF_S_FORMAT "%.8f"
#elif 0
#define PA_SAMPLE_TYPE  paInt16
#define SAMPLE_SIZE (2)
#define SAMPLE_SILENCE  (0)
#define CLEAR(a) memset( (a), 0,  FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE )
#define PRINTF_S_FORMAT "%d"
#elif 1
#define PA_SAMPLE_TYPE  paInt24
#define SAMPLE_SIZE (3)
#define SAMPLE_SILENCE  (0)
#define CLEAR(a) memset( (a), 0,  FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE )
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
#define SAMPLE_SIZE (1)
#define SAMPLE_SILENCE  (0)
#define CLEAR(a) memset( (a), 0,  FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE )
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
#define SAMPLE_SIZE (1)
#define SAMPLE_SILENCE  (128)
#define CLEAR( a ) { \
    int i; \
    for( i=0; i<FRAMES_PER_BUFFER*NUM_CHANNELS; i++ ) \
        ((unsigned char *)a)[i] = (SAMPLE_SILENCE); \
}
#define PRINTF_S_FORMAT "%d"
#endif
//


#define NUM_SECONDS   (5)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER (2048) // (38400) was previous value that worked 
#define pi 3.14159265
#ifndef M_PI
#define M_PI  (3.14159265)
#endif
#define NUM_CHANNELS (2)
#define PA_SAMPLE_TYPE (paFloat32)
#define TABLE_SIZE   (38400)

double freqc=800.00;							//defined for testing purposes
// 								set variables for spacing of wave files
float dit_space=3000;  				//dit space in milliseconds
float dah_space=(2 * dit_space); 			//Dah length in milliseconds
float word_space=(7 * dit_space);			// Space length in milliseconds
float letter_space=(3 * dit_space); 		// letter space length in milliseconds
//								set variables for proper cw wave taper on begining and end of wave.
float ramp_begin_ms = 100;// time in MilliSECONDS for beginning
float ramp_end_ms = 100;
float begin_ramp = ((ramp_begin_ms/1000)*freqc);
float end_ramp = ((ramp_end_ms/1000)*freqc);

//	float begin_ramp=200 ;						// time in milliseconds for beginning
//	float end_ramp=200;						// time in milliseconds for end taper
	
float slice_value = 0;
const int sw_duration_length =((SAMPLE_RATE/freqc)*(dit_space));//((dit_space / 500)*(freqc/2));	//set duration length specifically for dit at this time. Change this for dah wave later also!
double adjust_amp = 0;


typedef struct
{
    float sine[TABLE_SIZE];
    int left_phase;
    int right_phase;
    char message[20];
}
paTestData;
//INSERT NEW CODE HERE==========================================================
int main (void)
{
	 PaStream *stream = NULL;
	int i;
	PaError err;
	char * sampleBlock ;
	
	/* -- initialize PortAudio -- */
	err = Pa_Initialize();
	 PaStreamParameters inputParameters, outputParameters;
	if( err != paNoError ) goto error;
	
	/* -- setup input and output -- */
	inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
	inputParameters.channelCount = NUM_CHANNELS;
	inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultHighInputLatency ;
	inputParameters.hostApiSpecificStreamInfo = NULL;
	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	outputParameters.channelCount = NUM_CHANNELS;
	outputParameters.sampleFormat = PA_SAMPLE_TYPE;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultHighOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	
	
	/* -- setup stream -- */
	err = Pa_OpenStream(
	&stream,
	&inputParameters,
	&outputParameters,
	SAMPLE_RATE,
	FRAMES_PER_BUFFER,
	paClipOff, /* we won't output out of range samples so don't bother clipping them */
	NULL, /* no callback, use blocking API */
	NULL ); /* no callback, so no callback userData */
	if( err != paNoError ) goto error;
	/* -- start stream -- */
	err = Pa_StartStream( stream );
	if( err != paNoError ) goto error;
	printf("Wire on. Will run one minute.\n"); fflush(stdout);
	/* -- Here's the loop where we pass data from input to output -- */
	for( i=0; i<(60*SAMPLE_RATE)/FRAMES_PER_BUFFER; ++i )
	{
	err = Pa_WriteStream( stream, sampleBlock, FRAMES_PER_BUFFER );
	if( err ) goto xrun;
	err = Pa_ReadStream( stream, sampleBlock, FRAMES_PER_BUFFER );
	if( err ) goto xrun;
	}
	/* -- Now we stop the stream -- */
	err = Pa_StopStream( stream );
	if( err != paNoError ) goto error;
	/* -- don't forget to cleanup! -- */
	err = Pa_CloseStream( stream );
	if( err != paNoError ) goto error;
	Pa_Terminate();
	return 0;
xrun:
  if( stream ) {
  Pa_AbortStream( stream );
  Pa_CloseStream( stream );
  }
 free( sampleBlock );
  Pa_Terminate();
  if( err & paInputOverflow )
  fprintf( stderr, "Input Overflow.\n" );
  if( err & paOutputUnderflow )
  fprintf( stderr, "Output Underflow.\n" );
  return -2;
error:
  if( stream ) {
  Pa_AbortStream( stream );
  Pa_CloseStream( stream );
  }
  free( sampleBlock );
  Pa_Terminate();
  fprintf( stderr, "An error occured while using the portaudio stream\n" );
  fprintf( stderr, "Error number: %d\n", err );
  fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
  return -1;
 }










//END NEW CODE HERE ============================================================


//==============================================================================
//=================================================Use this later===============
//==============================================================================
/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
/*static int patestCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned long i;

    (void) timeInfo; // Prevent unused variable warnings. 
    (void) statusFlags;
    (void) inputBuffer;
    
    for( i=0; i<=FRAMES_PER_BUFFER; i++ )
    {
        *out++ = data->sine[i];  
        //*out++ = data->sine[data->right_phase];  
		//data->left_phase += 1;
			if( i >= FRAMES_PER_BUFFER/2 ) {
				*out++=0;
				return paContinue;
			}//data->left_phase -= FRAMES_PER_BUFFER;
		//data->right_phase += 1; 
			//if( data->right_phase >= FRAMES_PER_BUFFER ) data->right_phase -= FRAMES_PER_BUFFER;
    }
	*out++ = 0;
    //Pa_Sleep( 5500 );
    return paContinue;
}


 // This routine is called by portaudio when playback is done.
 
static void StreamFinished( void* userData )
{
   paTestData *data = (paTestData *) userData;
   printf( "Stream Completed: %s\n", data->message );
}

////////////////////////////////////////////////////////////////
int main(void);
int main(void)
{
    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
    paTestData data;
	memset ( data.sine, 0, TABLE_SIZE );

    printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);
    
    * initialise sinusoidal wavetable *
    
	 * 
	 * 
	 * //INSERT CODE HERE FOR TABLE!//////////////////////////////////////////////////////
	int x=0;
while (x<=begin_ramp)							//loop for first amplitude taper
	{
	adjust_amp = ((x)/begin_ramp);  
	data.sine[x] = ((sin(freqc * ( pi) * x / SAMPLE_RATE))*adjust_amp);
	x++;
	}

while (x < ((sw_duration_length) - end_ramp)) //loop for sine wave at full amplitude
	{
	data.sine[x] = (sin(freqc * ( pi) * x / SAMPLE_RATE));	
	x++;
	}

while (x<=sw_duration_length) 					//loop for final taper of sound wave
	{
	adjust_amp = (((sw_duration_length-x))/end_ramp);
	data.sine[x] = (sin(freqc * ( pi) * x / SAMPLE_RATE))*adjust_amp;	
	x++;
	}
	data.left_phase = data.right_phase = 0;
	
    //END CODE HERE FOR TABLE!///////////////////////////////////////////////
    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device 
    if (outputParameters.device == paNoDevice) {
      fprintf(stderr,"Error: No default output device.\n");
      goto error;
    }
    outputParameters.channelCount = 2;       /* stereo output 
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output 
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              NULL, /* no input 
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them 
              patestCallback,
              &data );
    if( err != paNoError ) goto error;

    sprintf( data.message, "No Message" );
    err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    //printf("Play for %d seconds.\n", NUM_SECONDS );
    Pa_Sleep( 10000);

    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    Pa_Terminate();
    printf("Test finished.\n");
    
    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}






/*
 *
 * 
//////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////

#include "portaudio.h"

#include <vector>					//used vector for wave data....maybe delete in future....
#include <math.h>					//sine function was in here
#include <iostream>
#include <stdio.h>
#include <algorithm>  			//used for display vector
#include <iterator>				//used for display vector


#define pi 3.1415								//define pi for calculations
	//defined just for testing code.  Will become adjustable variables later.
#define FRAMES_PER_BUFFER 256
#define SAMPLE_RATE 44100				//defined for testing purposes  May remain in final code.
#define freqc 600.00							//defined for testing purposes
// 								set variables for spacing of wave files
#define dit_space 400.0000  				//dit space in milliseconds
#define dah_space (2 * dit_space) 			//Dah length in milliseconds
#define word_space (7 * dit_space)			// Space length in milliseconds
#define letter_space (3 * dit_space) 		// letter space length in milliseconds
//								set variables for proper cw wave taper on begining and end of wave.
#define begin_ramp 100.00 						// time in milliseconds for beginning
#define end_ramp 100.00						// time in milliseconds for end taper
#define NUM_CHANNELS 2

#define 	PA_SAMPLE_TYPE   paFloat32

PaStreamParameters inputParameters;
PaStreamParameters outputParameters;

//create struct for passing to port audio.

//using namespace std;
//void display_vector(const vector<double> &v)
{
    std::copy(v.begin(), v.end(),
        std::ostream_iterator<double>(std::cout, "    "));
}*/
/*
PaStream* stream;
PaError err;

int main(int argc, char **argv)
{

	using namespace std;
PaStream *stream;
//vector <double> wave_slot; wave_slot.clear();				//create vector for wave data

double slice_count = SAMPLE_RATE / freqc;					//refered to as table in pa demo . number of slices per 360 wave



//int x = 0;											//set to long for weird math reasons
float slice_value = 0;
const int sw_duration_length =((dit_space / 1000)*(freqc));	//set duration length specifically for dit at this time. Change this for dah wave later also!
double wave_slot[sw_duration_length];
double adjust_amp = 0;									//used for ramp amplitude
int  i;

double *sampleBlock;

;//, inputParameters, PA_SAMPLE_TYPE, NUM_CHANNELS;
//1. this creates a proper sine wave stored as a vector! YAY!  Completed on 										11/20/2013 @ 9:31 PM!
//2. and now it makes a proper sine wave with adjustable beinging and ending envolopes 
//	  for proper CW!  		11/24/2013 @ 7:34 PM
//code beginning for ramping taper of wave.
//
// this code creates a proper length dit....maybe 11/23/2013..fixed using Scilab! 12/2/2013!
//
int x=0;
while (x<=begin_ramp)
	{
	//loop for first amplitude taper
	adjust_amp = ((x)/begin_ramp);
	slice_value = ((sin(freqc * (2 * pi) * x / SAMPLE_RATE))*adjust_amp);
	wave_slot[x] = (slice_value);
			//wave_slot.push_back(slice_value);
	x++;
	}

while (x < ((sw_duration_length) - end_ramp)) //rename 240 to sw_duration_lentght after troubleshooting.
	{
	//loop for rest of sine wave at full amplitude
	slice_value = (sin(freqc * (2 * pi) * x / SAMPLE_RATE));	
	wave_slot[x] = slice_value;
	//wave_slot.push_back(slice_value);
			//cout << x << ", "<< slice_value << ", "<<endl; USED for DEBUGGING
	x++;
	}

while (x<=sw_duration_length)
	{
	//loop for final taper of sound wave
	adjust_amp = (((sw_duration_length-x))/end_ramp);
	slice_value = (sin(freqc * (2 * pi) * x / SAMPLE_RATE))*adjust_amp;	
	wave_slot[x] = slice_value;
	//wave_slot.push_back(slice_value);
	x++;
	}

sampleBlock = &wave_slot[0];//
// this code is used for debugging
//
std::cout<<endl<<"Sample Rate : "<<SAMPLE_RATE<<endl<<"Frequency    : "<<freqc<<endl;		
std::cout<<slice_count<<" slices and slices are"<<endl;
//std::cout<<"Vector Size : "<<wave_slot.size()<<endl;
std::cout << "Wave Duration : "<<sw_duration_length<<"Formuala: "<<((dit_space /1000)*(freqc))<<endl;
std::cout << "Dit Spaceing : " << dit_space <<"  Attempt: "<<((dit_space /1000)*(freqc))<<endl;
std::cout<<"Frequency:   "<<freqc<<endl;
std::cout<<"Begin Ramp : "<<begin_ramp<<endl<<"End Ramp  : " << end_ramp<< endl;
//display_vector(wave_slot);
//
 * This is where the snippet ends
 * 
 * 
 * 
 * 
 * 
//
//
//http://stackoverflow.com/questions/9751932/displaying-contents-of-a-vector-container-in-c
//The display_vector code was found at this ^ web address
//
//IMPORTANT : vector<int> means that it stores an integer, vector <double> means that it will store a double! DUH!
// so....vector <data_type> was the lesson learned in this friggin mess!
// Also learned how to pass by reference! with const vector<double> &v! YAY
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////
//     SAMPLE CODE FOR Port Audio Usage.....														//
//			Just for reference only....																//	
//																										//
/////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_SECONDS   (3)			//used for duration of tone I guess?
#define SAMPLE_RATE   (44100)		//sample rate
#define FRAMES_PER_BUFFER  (256)	// frames per buffer....leave as is for now

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (73)	//table size = freq/hz
typedef struct
{
    float sine[TABLE_SIZE];
    int left_phase;
    int right_phase;
    char message[20];
}
paTestData;
//

//


/// This routine will be called by the PortAudio engine when audio is needed.
// It may called at interrupt level on some machines so don't do anything
// that could mess up the system like calling malloc() or free().
//
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned long i;

    (void) timeInfo; // Prevent unused variable warnings. 
    (void) statusFlags;
    (void) inputBuffer;
    
    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ = data->sine[data->left_phase]; /// // left 
        *out++ = data->sine[data->right_phase];  //right 
        data->left_phase += 1;
        if( data->left_phase >= TABLE_SIZE ) data->left_phase -= TABLE_SIZE;
        data->right_phase += 2; // higher pitch so we can distinguish left and right. 
        if( data->right_phase >= TABLE_SIZE ) data->right_phase -= TABLE_SIZE;
    }
    
    return paContinue;
}

//
//  This routine is called by portaudio when playback is done.
 //
static void StreamFinished( void* userData )
{
   paTestData *data = (paTestData *) userData;
   printf( "Stream Completed: %s\n", data->message );
}

//=====================================================
//int main(void);
int main(int argc, char **argv)
{
    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
    paTestData data;
    int i;

    
    printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);
    //
	
	//
	
    // initialise sinusoidal wavetable 
    for( i=0; i<TABLE_SIZE; i++ )
    {
        data.sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
    }
    data.left_phase = data.right_phase = 0;
    
    err = Pa_Initialize();
    if( err != paNoError ) goto error;
	int numDevices;
numDevices = Pa_GetDeviceCount();
const PaDeviceInfo *deviceInfo;
for( i=0; i<numDevices; i++ )
{
deviceInfo = Pa_GetDeviceInfo( i );

}

if( numDevices < 0 )
{
printf( "ERROR: Pa_CountDevices returned 0x%x\n", numDevices );
std::cout<<"Number of Devices :"<<numDevices<<std::endl<<"Press a key"<<std::endl;
char poop;
std::cin>>poop;


err = numDevices;
goto error;

}
//
    outputParameters.device = Pa_GetDefaultOutputDevice(); // default output device 
    
	
	if (outputParameters.device == paNoDevice) {
      fprintf(stderr,"Error: No default output device.\n");
      goto error;
    }
//

    outputParameters.channelCount = 2;      // stereo output 
    outputParameters.sampleFormat = paFloat32; // 32 bit floating point output 
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              NULL, // no input 
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      // we won't output out of range samples so don't bother clipping them 
              patestCallback,
              &data );
    if( err != paNoError ) goto error;
	//int numDevices;
numDevices = Pa_GetDeviceCount();
if( numDevices < 0 )
{
printf( "ERROR: Pa_CountDevices returned 0x%x\n", numDevices );
std::cout<<"Number of Devices :"<<numDevices<<std::endl<<"Press a key"<<std::endl;
char poop;
std::cin>>poop;


err = numDevices;
goto error;

}
//
    sprintf( data.message, "No Message" );
    err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    printf("Play for %d seconds.\n", NUM_SECONDS );
    Pa_Sleep( NUM_SECONDS * 1000 );

    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    Pa_Terminate();
    printf("Test finished.\n");
    
    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}*/

/////////////////////////////////////////////////////////////////////////////////////////////




//
//end of debug code
//
////}  <<<----maybe needed later......
// old code down below....for comedy relief.....
//


/*void SineGen (double freq, double sample_rate, vector<int> &wave_slot) 	//a * is needed because we are returning a pointer!  
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
}

//
//
//

int main(int argc, char **argv)
{
	vector <int> sample_pointer;
	double slice;
	vector <int> wave_slot;
	
	
	printf("hello world\n");
	SineGen (100,44100, wave_slot(440)); //440 to be samples/freq later.....
	
	cout<<sample_pointer<<" <--- Sample Pointer"<<endl;
	int c; c=0;
	while (c<=441) {
	slice = sample_pointer.size;
	cout<<slice<<" <--- Slice 'value' of "<<c<<endl;
	c++;
	
	return 0;*/