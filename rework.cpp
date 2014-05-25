//this is a quick reworking...
//
//
#include <vector>
#include <math>
#include <iostream.h>
#include <stdio.h>
#define pi 3.1415		//define pi for calculations
#define sample_rate 44100
#define freq 440

using namespace std;
vector <int> wave_slot;
double length = wave_slot.size;
double slice_count = sample_rate / freq;
int x;


while (x<=slice_count) {
		wave_slot[x] = sin(freq * (2 * pi) * x / sample_rate);
		x++;
		};


