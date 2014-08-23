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
const int swInputA = 7;  // An input switch

int number = 0;  // This will be the displayed data

uint8_t dataArray[12];  // This holds the data to display 

uint8_t convertByte;  // a byte used as a holder to convert the font data

int LEDcounter = 0;  // This times when the LEDs will flash
int SWcounter = 0;  // This is a debounce counter for the switch

int mode = 0;  // This holds the mode we are in

boolean lastPress= LOW;  // This is to latch the button press

int alphabet = 0;
int alphabetCounter = 0;

uint8_t displayBuffer[200];  // Buffer to hold the rendered text

const char text[] = "Nottingham Hackspace";

void setup()
{
  //set pins to output so you can control the shift register
  pinMode(sLatch, OUTPUT);
  pinMode(sClk, OUTPUT);
  pinMode(sData, OUTPUT);
  pinMode(led, OUTPUT);   
  pinMode(swInputA, INPUT_PULLUP); 
  
  memset(dataArray,0,sizeof(dataArray));  // Set dataArray to clear it 
  
  // Render the text to the displayBuffer
  textToBuffer(text);
  
}

void loop()
{ 
  // Write the dataArray to the LED matrix:
  digitalWrite(sLatch, LOW);  
  for(int j=0;j<sizeof(dataArray);j++)
  {     
    //shiftOut(sData, sClk, MSBFIRST, dataArray[j]);
    shiftOut(sData, sClk, LSBFIRST, dataArray[j]);  // Rotated text
  }
  digitalWrite(sLatch, HIGH); 

  // Test the switch - should we change mode?
  if(digitalRead(swInputA)==LOW&&lastPress==LOW)
  {
    // Button pressed so count up
    SWcounter++;
    if(SWcounter>=50)
    {
      mode++;
      if(mode>2)
      {
        mode=0;
      }
      lastPress=HIGH;
    }
  }
  else if (digitalRead(swInputA)==HIGH&&lastPress==HIGH)
  {
    // Button NOT pressed - reset everything
    SWcounter=0;
    lastPress=LOW;
  }
     
  // Choose what to do depending upon the mode:
  // Mode are:
  // 0 = flash random colours
  // 1 = Write "Nottingham Hackspace"
  // 2 = Write "ERROR"
  switch(mode)
  {
    case 0:
      // Mode = 0   
      // output the data from the text bufer
      for(int j=0;j<12;j++)
      { 
        dataArray[j]=displayBuffer[j];
      } 
    break;
    
    case 1:
      for(int j=0;j<12;j++)
      {  
        dataArray[j] = B11111111;
      }
    break;
 
    case 2:
      if(alphabetCounter>=100)
      {
        alphabet++;
        alphabetCounter=0;  // Reset the counter
      }
      
      alphabetCounter++;
       
      for(int j=0;j<12;j++)
      { 
        
        if(j<5)
        {       
          // This takes the byte from the program memory
          // The shift register is in the bit format 70123456, so we must move the last bit and push it into the first bit.
          convertByte = pgm_read_byte_near(System5x8 + (('A'+alphabet-32)*5)+j);
//          dataArray[4-j]=convertByte<<1;
//          dataArray[4-j]|=(convertByte & 0x80)?1:0;     
          dataArray[j]=convertByte>>1;  // Upside down text
          //dataArray[j]|=(convertByte & 0x01)?1:0;  

        }
        else
        {
          //dataArray[j] = B11111111;
          dataArray[j] = B00000000;
        }  
      } 
    break;   
    
    default:
    // Do this when not in a mode
       
    break;    
  }

  // Here we flash the LED to show its working:     
  if(LEDcounter<=25)
  {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)       
  }
  else if (LEDcounter<=50)
  {
    digitalWrite(led, LOW);   // turn the LED on (HIGH is the voltage level)
  }
  else 
  {
    LEDcounter=0;
  }
  LEDcounter++;
  
  // This is the main delay and slows everything down a bit.
  delay(10);
  
}


// Takes in a String, converts that data into display data and puts into displayBuffer
void textToBuffer(const char* buf)
{
  int col = 0;  // Column index
  uint8_t data; 
  
  for(int i=0;i<strlen(buf);i++)
  {
    char c = buf[i];
    // Check for if the char is in valid ascii range
    // Range for this font is 32 - 127 (0x20 - 0x7F)
    
    for(int j=0;j<5;j++)
    {
      // The shift register is in the bit format 70123456, so we must move the last bit and push it into the first bit.
      data = pgm_read_byte_near(System5x8 + ((c-32)*5)+j);
      //displayBuffer[col]=data<<1;
      //displayBuffer[col]|=(data & 0x80)?1:0; 
      displayBuffer[col]=data>>1;
      //displayBuffer[col]|=(data & 0x01)?1:0;      
      
      col++;         
    } 
    // Add inter-char gap
    displayBuffer[col++]=0;
  }
}
