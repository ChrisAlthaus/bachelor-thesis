#ifndef LIGHT_PATTERNS_H
#define LIGHT_PATTERNS_H

#define NUMBER_SAMPLE_VALUES 100
#define SUM_BEACON_INTERVALLS 32.0
#define LENGTH_BEACON_ARRAY 8

#define SUM_STAIRCASE_INTERVALLS 24.0
#define LENGTH_STAIRCASE_ARRAY 5

#define SUM_BLINK_INTERVALLS 4
#define LENGTH_BLINK_ARRAY 4.0

#include<stdio.h>
//intervall: even entries=active, uneven entries=inactive
int beaconIntervalls[]={3,2,3,8,3,2,3,8};
int beaconSampleValues[NUMBER_SAMPLE_VALUES]={};

int staircaseIntervalls[]={4,4,4,4,8};
int staircaseSampleValues[NUMBER_SAMPLE_VALUES]={};

//int blinkIntervalls[]={1,1,1,1};
//int blinkSampleValues[NUMBER_SAMPLE_VALUES]={};


void setArrayValues(int array[], int indexStart, int indexStop,int value){
	for(int i=indexStart; i<indexStop; i++){
		array[i]=value;
	}

}

void initIntervalls(){
	int indexStart=0;
	int indexStop=0;
	
	for(int i=0; i<LENGTH_BEACON_ARRAY;i++){
		indexStop=indexStart+(int)((beaconIntervalls[i]/SUM_BEACON_INTERVALLS)*NUMBER_SAMPLE_VALUES);
		
		if(i%2==0){		//led should be active
			setArrayValues(beaconSampleValues,indexStart,indexStop,1);
		}
		
		indexStart=indexStop;
	}
	
	indexStart=0;
	indexStop=0;
	
	for(int i=0; i<LENGTH_STAIRCASE_ARRAY-1;i++){
		indexStop=indexStart+(int)((staircaseIntervalls[i]/SUM_STAIRCASE_INTERVALLS)*NUMBER_SAMPLE_VALUES);
		setArrayValues(staircaseSampleValues,indexStart,indexStop,i+1);
		
		indexStart=indexStop;
	}
	
	/*indexStart=0;
	indexStop=0;
	
	for(int i=0; i<LENGTH_BLINK_ARRAY;i++){
		indexStop=indexStart+(int)((staircaseIntervalls[i]/SUM_STAIRCASE_INTERVALLS)*NUMBER_SAMPLE_VALUES);
		
		if(i%2==0){		//led should be active
			setArrayValues(blinkSampleValues,indexStart,indexStop,1);
		}
		indexStart=indexStop;
	}*/
	
}


#endif