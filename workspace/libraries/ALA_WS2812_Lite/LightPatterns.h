#ifndef LIGHT_PATTERNS_H
#define LIGHT_PATTERNS_H

//#include <stdio.h>
//#include <math.h>

#define NUMBER_SAMPLE_VALUES 70
#define SUM_BEACON_INTERVALLS 32.0
#define LENGTH_BEACON_ARRAY 8

#define SUM_STAIRCASE_INTERVALLS 16.0
#define LENGTH_STAIRCASE_ARRAY 4.0

#define SUM_BLINK_INTERVALLS 4.0
#define LENGTH_BLINK_ARRAY 4.0

#define SUM_TRANSMISSONFB_INTERVALLS 26.0
#define LENGTH_TRANSMISSONFB_ARRAY 13

//#define PI 3.147

//intervall: even entries=active, uneven entries=inactive
int beaconIntervalls[]={3,2,3,8,3,2,3,8};
int beaconSampleValues[NUMBER_SAMPLE_VALUES]={};

int blinkIntervalls[]={1,1,1,1};
int blinkSampleValues[NUMBER_SAMPLE_VALUES]={};

//intervall: ascending brightness, last entry= inactive
int staircaseIntervalls[]={4,4,4,4};	
int staircaseSampleValues[NUMBER_SAMPLE_VALUES]={};

//intervall: even entries=active, uneven entries=inactive
int transmissionFixedBrightnessIntervalls[]={2,3,2,3,1,1,2,2,2,3,1,2,2};	
int transmissionFBSampleValues[NUMBER_SAMPLE_VALUES]={};

//intervall: sinus function sample values
float pulseSlowSampleValues[NUMBER_SAMPLE_VALUES]={};

float mapRange(int x, int in_min, int in_max, int out_min, int out_max);
void setArrayValues(int array[], int indexStart, int indexStop,int value){
	for(int i=indexStart; i<indexStop; i++){
		array[i]=value;
	}

}

//initialize sample intervalls with weights
void initIntervalls(){
	int indexStart=0;
	int indexStop=0;
	
	//beacon intervall
	for(int i=0; i<LENGTH_BEACON_ARRAY;i++){		
		indexStop=indexStart+(int)((beaconIntervalls[i]/SUM_BEACON_INTERVALLS)*NUMBER_SAMPLE_VALUES);
		
		if(i%2==0){		//led should be active, current intervall [indexStart,indexStop]
			setArrayValues(beaconSampleValues,indexStart,indexStop,1);
		}
		
		indexStart=indexStop;
	}
	
	indexStart=0;
	indexStop=0;
	
	//staircase intervall
	for(int i=0; i<LENGTH_STAIRCASE_ARRAY;i++){	//values from 0-4( 4=max brightness, 0 = off)
		indexStop=indexStart+(int)((staircaseIntervalls[i]/SUM_STAIRCASE_INTERVALLS)*NUMBER_SAMPLE_VALUES);
		setArrayValues(staircaseSampleValues,indexStart,indexStop,i+1);	
		
		indexStart=indexStop;
	}
	//remaining entries in staircaseSampleValues = 0 /leds off
	
	indexStart=0;
	indexStop=0;
	
	//blink intervall
	for(int i=0; i<LENGTH_BLINK_ARRAY;i++){		
		indexStop=indexStart+(int)((blinkIntervalls[i]/SUM_STAIRCASE_INTERVALLS)*NUMBER_SAMPLE_VALUES);
		
		if(i%2==0){		//led should be active
			setArrayValues(blinkSampleValues,indexStart,indexStop,1);
		}
		indexStart=indexStop;
	}
	
	indexStart=0;
	indexStop=0;
	
	//transmission fixed brightness intervall
	for(int i=0; i<LENGTH_TRANSMISSONFB_ARRAY;i++){		
		indexStop=indexStart+(int)((transmissionFixedBrightnessIntervalls[i]/SUM_TRANSMISSONFB_INTERVALLS)*NUMBER_SAMPLE_VALUES);
		
		if(i%2==0){		//led should be active
			setArrayValues(transmissionFBSampleValues,indexStart,indexStop,1);
		}
		indexStart=indexStop;
	}
	
	
	indexStart=0;
	indexStop=0;
	
	//pulse slow intervall
	for(int i=0; i<NUMBER_SAMPLE_VALUES;i++){		
		float degree= mapRange(i,0,NUMBER_SAMPLE_VALUES-1,0,360);
		pulseSlowSampleValues[i]=sin(degree*PI/180);
	}
	
}

void printArray(int array[],int length){
  for(int i=0;i<length;i++){
    int number = array[i];
    //printf("%d",number);
    //printf(",");
  }
}

void printArrayFloat(float array[],int length){
  for(int i=0;i<length;i++){
    float number = array[i];
    //printf("%f",number);
    //printf(",");
  }
}

float mapRange(int x, int in_min, int in_max, int out_min, int out_max){
  return (x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}


#endif
