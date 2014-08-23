/********************************************************
/****** 7 Segment LED driver board Basic Code 1 *********
/****** by Matt Little **********************************
/****** Date: 5/4/13 ************************************
/****** info@re-innovation.co.uk ************************
/****** www.re-innovation.co.uk *************************
/********************************************************

This example uses the Serial Shift output to control a 7 segment LED display
Data is shifted out serially and only when all the shift registers are filled does the latch cause
the LED outputs to update.
The LED boards require 12V supply as they contain strings of 4 LEDs.
The wiring for each board is as follows (P1 and P2):

  Pin 1  ->  Serial LATCH -> A3 on Arduino (in this example)
  Pin 2  ->  Serial CLOCK -> A5 on Arduino (in this example)
  Pin 3  ->  Serial DATA  -> A4 on Arduino (in this example)  
  Pin 4  ->  GROUND       -> GND on Arduino
  Pin 5  ->  +5V          -> +5V on Arduino
  Pin 6  ->  GROUND       -> GND for LED supply
  Pin 7  ->  +12V         -> +12V for LED supply

Use a 0.1uF capacitor between Pin 1 (sLATCH) and Pin 4 (GND) to prevent flicker on the display.

see www.re-innovation.co.uk for more details
 
*/

#include "SystemFont5x8.h"
#include <avr/pgmspace.h>

// This is for the serial shifted output data
const int sLatch = 0;   //Pin connected to ST_CP of 74HC595
const int sClk =  1;    //Pin connected to SH_CP of 74HC595
const int sData = 4;    //Pin connected to DS of 74HC595
const int led = 6;  //LED of Minimus

int number = 0;  // This will be the displayed data

uint8_t dataArray[12];  // This holds the data to display 

uint8_t convertByte;  // a byte used as a holder to convert the font data


void setup()
{
  //set pins to output so you can control the shift register
  pinMode(sLatch, OUTPUT);
  pinMode(sClk, OUTPUT);
  pinMode(sData, OUTPUT);
  pinMode(led, OUTPUT);   
  
  memset(dataArray,0,sizeof(dataArray));  // Set dataArray to clear it 
 
  
}

void loop()
{ 
  
  digitalWrite(sLatch, LOW);  
  //Output the matrix
  for(int j=0;j<sizeof(dataArray);j++)
  {     
    shiftOut(sData, sClk, MSBFIRST, dataArray[j]);
  //  delay(5);
  }
  digitalWrite(sLatch, HIGH);  
  
  // output h
  for(int j=0;j<12;j++)
  {  
    if(j<5)
    {
      dataArray[j]=pgm_read_byte_near(System5x8 + (('F'-32)*5)+j) <<2;
    }
    else if(j<10)
    {
      dataArray[j]=pgm_read_byte_near(System5x8 + (('U'-32)*5)+j) <<2;
    }
    else
    {
      dataArray[j] = B11111111;
    }  
  } 
 
  


  
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)       
  delay(250);   // Delay for a second
  digitalWrite(led, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(250);

 
//    digitalWrite(sLatch, LOW);
//    shiftOut(sData, sClk, MSBFIRST, dataArray[System5x8['h']+j]);
//    digitalWrite(sLatch, HIGH); 
 
 
//  
//  
//  
////  for(int i=0;i<12;i++)
////  {
////    
//    for(int y=1;y<9;y++)
//    {
//      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
//      
//       // take the latchPin low so the LEDs don't change while you're sending in bits:
//      digitalWrite(sLatch, LOW);
//      // shift out the bits:
//      // Send data via 3 shift registers:
//      
//      //shiftOut(sData, sClk, MSBFIRST, intData(y));
//      
//      shiftOut(sData, sClk, MSBFIRST, B11111111);      
//      //shiftOut(sData, sClk, MSBFIRST, B00000000); 
//      shiftOut(sData, sClk, MSBFIRST, B11111111); 
//      
//      //take the latch pin high so the LEDs will light up:
//      digitalWrite(sLatch, HIGH);
//      //Serial.println(number);
//      delay(250);   // Delay for a second
//      digitalWrite(led, LOW);   // turn the LED on (HIGH is the voltage level)
//      delay(250);  
//    }
//    
////  }


}

// This function returns the correct binary value to display the integer
int intData (int segmentData)
{
  int displayData;
  
  switch (segmentData)
  {
    case 0:
      displayData = B00000000;  // The number 0 in binary   
    break;
    case 1:
      displayData = B00000010;  // The number 1 in binary   
    break;
    case 2:
      displayData = B00000001;  // The number 2 in binary   
    break;
    case 3:
      displayData = B10000000;  // The number 3 in binary   
    break;
    case 4:
      displayData = B01000000;  // The number 4 in binary   
    break;
    case 5:
      displayData = B00100000;  // The number 5 in binary   
    break;
    case 6:
      displayData = B00010000;  // The number 6 in binary   
    break;
    case 7:
      displayData = B00001000;  // The number 7 in binary   
    break;
    case 8:
      displayData = B00000100;  // The number 8 in binary   
    break; 
      
  }
  return displayData;
}
