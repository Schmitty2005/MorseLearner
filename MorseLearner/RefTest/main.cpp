#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <vector>


using namespace std;

void printarray (char (&array)[32])
{
	int y;
	y=sizeof(array);
	cout <<y<<endl; 
	for (int z = 0; z<32; z++)
		{
			std::cout << array[z];
			//std::cout << z;
		}
	return;
	
}
void printvector (vector <string> (&passed))
{
	cout << "Passed Vector size : ";// << passed.size << endl;
	cout << "This is the vector that was passed : " <<passed[0]<<endl;
	return;
}
int main(int argc, char **argv)
{
	char testarray[32]= "This shld an array Char 32";
	char *testpointer = testarray;
	double othertest [54];
	double *dbltest = othertest;
	for (int q = 0; q <= 54;  q++){othertest [q] = q;}
	cout << sizeof(othertest)<<endl;
	//for (int q=0; q<=sizeof(othertest); q++){cout << q << " : "<< othertest[q] << endl;};
	//this is the pointer to the array
	cout<<"Address of Array :"<<testpointer<<endl;
	cout<<"Address of Array :"<<dbltest<<endl;
	printf("This is to test the passing of Arrays in functions!\n");
	printarray(testarray);
	vector <string> testvector;
	
	
	
	getch();
	return 0;
}


