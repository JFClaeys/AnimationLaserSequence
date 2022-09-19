/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/
#include "alphas.h"
#include "sequence.h"

/*following are pin defitnition for the 74HC595 drive
 * running ShiftRegister 74HC595 
 * each pin has a 47k resistor connected to the base of a 2N2222 transistor
 * 2N2222 collector is connected to the negative leg of a laser diod
 * 2N2222 emitter is connected to ground
 * Following pinout would eventually change shen the CD card reader (on SPI) is mastered, since the current pinout will be used for SPI
   SR_LATCH_PIN : ST_CP - pin 12  store register clock pin  -  PB3 = STM32-pin 19  /  uno-pin 3  / nano d4
   SR_CLOCK_PIN : SH_CP - pin 11  shift register clock pin  -  PB4 = STM32-pin 20  /  uno-pin 4  / nano d5
   SR_DATA_PIN  : DS    - pin 14  serial data input         -  PB5 = STM32-pin 21  /  uno-pin 5  / nano d3
                  OE    - pin 13  Output enabled  is connected to ground 
*/
#define SR_LATCH_PIN PD4 //PB3
#define SR_CLOCK_PIN PD5 //PB4
#define SR_DATA_PIN  PD3 //PB5 

byte SequenceSize = 0;
byte looping = 0;
int  millisWait = 0;  //how many millisecnds before next step
unsigned long CurrentTime = 0;
unsigned long LoopStartTime = 0;


void WriteShiftRegisterValue( byte value ) {
  digitalWrite(SR_LATCH_PIN, LOW);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, MSBFIRST, value);
  digitalWrite(SR_LATCH_PIN, HIGH);
}

/* the following functions are helpers to allow using Serial functions in an easy manner
   BUT also to allow usage of my stm32 via ST-link which do not provide serial communications.
   Given the USB stm32 seems problematic with my units, ST-LINK is MY prefered way*/

/* define prototype for using defaiult parameter LineFeed*/   
void PrintSerialLog( int aText, bool LineFeed = false );
void PrintSerialLog( String aText, bool LineFeed = false);
  
void PrintSerialLog( String aText, bool LineFeed ) {
  #ifndef  STM32_CORE_VERSION  
  if (LineFeed) {
    Serial.println(aText);
  } else {
    Serial.print(aText);
  }
  #endif
}

void PrintSerialLog( int aText, bool LineFeed ) {
  #ifndef  STM32_CORE_VERSION  
  if (LineFeed) {
    Serial.println(aText);
  } else {
    Serial.print(aText);
  }
  #endif
}

void setup() {

  /* initialization of the 74HC595 */
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_DATA_PIN, OUTPUT);
  
 // Serial.begin(9600);
 // while (!Serial) {
 //   ; // wait for serial port to connect. Needed for native USB port only
 // }
  /*
     00 01 05 00 02 04 08 10 03 E8 04
     -- -- -- -------------- ----- --
      |  | |   |              |     +- repetition
      |  | |   |              +------- delai (1000 ms)
      |  | |   +---------------------- sequence 
      |  | +-------------------------- nombre de bytes de sequence
      |  +---------------------------- version mineure
      +------------------------------- version majeure
  */

  /*for (int i = 1; i <= SequenceRepeat; i++) {
    PrintSerialLog("Sequence ");
    PrintSerialLog(i);
    PrintSerialLog( " = "); 
    
    for (int j = 0; j < SequenceSize; j++) {
      PrintSerialLog(Sequence[j]);
      PrintSerialLog(" ");
    }
    PrintSerialLog("", true);
  }

  PrintSerialLog("-----", true); */
  SequenceSize = NUM_OF(sequences);
}
  
void loop() {

  CurrentTime = millis();
  if ((CurrentTime - LoopStartTime) <=  millisWait) {  // looks like the delay has expired.  moving in to the next step
  
    byte SequenceType = pgm_read_byte( &sequences[looping].seqtype );
  
    switch (SequenceType) {
      case sqDATA :    

        WriteShiftRegisterValue( pgm_read_byte( &sequences[looping].Data ) );  // just shift one laser after the other
        millisWait = pgm_read_byte( &sequences[looping].Delay) * 10;
        delay(millisWait);   //10 millisecons can realy be the minimum we can track...
        LoopStartTime = millis();  
        break;
      case sqCHAR :
        byte Index = pgm_read_byte( &sequences[looping].Data );    
        for (byte i = 0; i < NUM_LINES_CHAR; i++) {
          byte k = pgm_read_byte( &alphas[Index].drawing[i]);
          WriteShiftRegisterValue( k );
          millisWait = pgm_read_byte( &sequences[looping].Delay) * 10;
          delay(millisWait);       
          LoopStartTime = millis();  
        }
        break;
    }

    looping ++;
    if (looping >= SequenceSize) {
      looping = 0;
    }
  }  
  else
  {
    CurrentTime = 0;

  }
      
}
