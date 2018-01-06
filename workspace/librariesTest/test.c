#include "LightPatterns.h"

#include<stdio.h>

void printArray(int array[],int length){
	for(int i=0;i<length;i++){
		printf("%d,",array[i]);
	}
	
}

void printArrayFloat(float array[],int length){
	for(int i=0;i<length;i++){
		printf("%f,",array[i]);
	}
	
}

int main(){
	for(int i=0; i<LENGTH_BEACON_ARRAY;i++){
			printf("%d\n",beaconIntervalls[i]);
		}
		
	initIntervalls();
	printArray(beaconSampleValues,NUMBER_SAMPLE_VALUES);
	printf("\n");
	printArray(staircaseSampleValues,NUMBER_SAMPLE_VALUES);
	printf("\n");
	printArray(blinkSampleValues,NUMBER_SAMPLE_VALUES);
	printf("\n");
	printArrayFloat(pulseSlowSampleValues,NUMBER_SAMPLE_VALUES);
	printf("\n");
}


