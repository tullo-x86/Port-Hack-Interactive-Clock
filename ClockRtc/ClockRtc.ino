/*
 * If your system clock is in UTC, set the RTC's clock with the command:
 * THETIME=$((`date +%s` + 36000)) && echo T$THETIME > /dev/ttyACM0
 */

#include <Time.h>  
#include <Wire.h>  
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
#include "WS2811.h"

#define BIT(B)           (0x01 << (uint8_t)(B))
#define SET_BIT_HI(V, B) (V) |= (uint8_t)BIT(B)
#define SET_BIT_LO(V, B) (V) &= (uint8_t)~BIT(B)

DEFINE_WS2811_FN(WS2811RGB, PORTB, 0)

RGB_t rgb[60];
const int Elements = ARRAYLEN(rgb);

void setup()  {
  SET_BIT_HI(DDRB, 0);
  SET_BIT_LO(PORTB, 0);
  memset(rgb, 0, Elements * 3);

  Serial.begin(9600);
  setSyncProvider(RTC.get);   // the function to get the time from the RTC

  // Resync at a little less than once an hour, so that any glitching
  // resulting from the resync happens at a different time each hour.
  setSyncInterval(4000);
  
  if(timeStatus()!= timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");      
}

time_t lastTime;
void loop()
{  
  if (Serial.available()) {
    time_t t = processSyncMessage();
    if (t != 0) {
      RTC.set(t);   // set the RTC and the system time to the received value
      setTime(t);          
    }
  }

  time_t theTime = now();
  if (lastTime != theTime)
  {
    lastTime = theTime;
    clockDialDisplay(theTime);
    serialClockDisplay(theTime);
  }
}

void clearRgb(bool bright) {
  memset(rgb, bright ? 0x10 : 0x08, 60 * 3);
}

void clockDialDisplay(time_t theTime){
  clearRgb(theTime % 2 == 0);
  int theSecond = second(theTime);
  int secondPosition = theSecond;
  rgb[secondPosition].g = 255;
  
  int theMinute = minute(theTime);
  int minutePosition = theMinute;
  rgb[minutePosition].b = 255;
  
  int theHour = hour(theTime);
  int hourPosition = (
      (theHour * 5) + (theMinute / 12)
    ) % 60;
  rgb[hourPosition].r = 255;
  
  WS2811RGB(rgb, Elements);
}

void serialClockDisplay(time_t theTime){
  Serial.print(year(theTime)); 
  Serial.print("-");
  Serial.print(month(theTime));
  Serial.print("-");
  Serial.print(day(theTime));
  Serial.print("T");
  Serial.print(hour(theTime));
  printDigits(minute(theTime));
  printDigits(second(theTime));
  Serial.println(); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

/*  code to process time sync messages from the serial port   */
#define TIME_HEADER  "T"   // Header tag for serial time sync message

unsigned long processSyncMessage() {
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     return pctime;
     if( pctime < DEFAULT_TIME) { // check the value is a valid time (greater than Jan 1 2013)
       pctime = 0L; // return 0 to indicate that the time is not valid
     }
  }
  return pctime;
}