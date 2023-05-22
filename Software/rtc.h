#include "i2c.h"
extern int sec, min, hour;
extern int day, date, month, year;

void rtc_readTime(unsigned char clock_address);
void rtc_readDate(char calendar_adress);
unsigned char rtc_convUpperNibble(unsigned char digit);
unsigned char rtc_convLowerNibble(unsigned char digit);


