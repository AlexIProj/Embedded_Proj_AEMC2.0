#include <xc.h>
#include "rtc.h"

#define RTC_WRITE_ADDRESS   0xD0
#define RTC_READ_ADDRESS    0xD1

unsigned char rtc_convUpperNibble(unsigned char digit){
    return ((digit >> 4) + 48);
}

unsigned char rtc_convLowerNibble(unsigned char digit){
    return ((digit & 0x0F) + 48);
}

void rtc_readTime(unsigned char clock_address){
    i2c_masterStart();
    i2c_masterWrite(RTC_WRITE_ADDRESS);
    i2c_masterWrite(clock_address);
    i2c_masterRepeatedStart();
    i2c_masterWrite(RTC_READ_ADDRESS);
    
    switch (clock_address){
        case 0x00:
            sec = i2c_readByte(0x00);
            break;
        
        case 0x01:
            min = i2c_readByte(0x01);
            break;
            
        case 0x02:
            hour = i2c_readByte(0x02);
            break;
            
        default:
            break;
    }    
    i2c_masterStop();
}

void rtc_readDate(char calendar_address){
    i2c_masterStart();
    i2c_masterWrite(RTC_WRITE_ADDRESS);
    i2c_masterWrite(calendar_address);
    i2c_masterRepeatedStart();
    i2c_masterWrite(RTC_READ_ADDRESS);
    
    switch (calendar_address){
        case 0x03:
            day = i2c_readByte(0x03);
            break;
            
        case 0x04:
            date = i2c_readByte(0x04);
            break;
            
        case 0x05:
            month = i2c_readByte(0x05);
            break;
            
        case 0x06:
            year = i2c_readByte(0x06);
            break;
            
        default:
            break;
    }      
    i2c_masterStop();
}











