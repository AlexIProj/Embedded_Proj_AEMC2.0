#include<xc.h>
#include "config.h"
#include "i2c.h"
#include "rtc.h"
#include "sensors.h"


#define SCL                 TRISCbits.TRISC3 
#define SDA                 TRISCbits.TRISC4 
#define LCD_ADDRESS         0x4E

#define LEFT_BUTTON         PORTBbits.RB0
#define MIDDLE_BUTTON       PORTBbits.RB1
#define RIGHT_BUTTON        PORTBbits.RB2
#define UPPER_BUTTON        PORTBbits.RB3
#define LOWER_BUTTON        PORTBbits.RB4
#define SWITCH_MODE_BUTTON  PORTBbits.RB5
#define BUZZER              PORTDbits.RD2

#define LDR_STATUS_LED      PORTDbits.RD1


int sec, min, hour;
int day, date, month, year;

extern int check_bit;

char alarm_status = 0;  //OFF

void main(void){
    char pos = 8;
    unsigned char device_mode = 0;
    int week_days[7] = {'S', 'M', 'T', 'W', 't', 'F', 's'};
    char sec_0, sec_1, min_0, min_1, hour_0, hour_1;
    char day_1, day_0, month_1, month_0, year_1, year_0;
    char alarm_digit[4] = {'0','0','0','0'};
    int ok, time_aux = 1;
    
    OSCCON |= 0x77; //internal oscillator, set at 8 MHz
    SCL |= 1;
    SDA |= 1;
    i2c_masterInit();
    lcd_init(LCD_ADDRESS);
    
    TRISD &= 0x00;
    TRISB &= 0xFF;
    ANSELH &= 0x00;
    ANSELbits.ANS0 |= 1;
    ANSELbits.ANS1 |= 1;
    BUZZER &= 0;
    
    OPTION_REG &= 0x00;
    WPUB &= 0xFF;
    
    lcd_setCursor(1,2);
    lcd_writeString("INITIALIZING");
    __delay_ms(1000);
    
    lcd_clear();
    lcd_setCursor(1,1);

    while(1){
        ok=1;
        
        rtc_readTime(0x01); //minutes
        rtc_readTime(0x02); //hours
        
        hour_1 = rtc_convUpperNibble(hour);
        hour_0 = rtc_convLowerNibble(hour);
        min_1 = rtc_convUpperNibble(min);
        min_0 = rtc_convLowerNibble(min);
        
        if(SWITCH_MODE_BUTTON == 0){
            while(!SWITCH_MODE_BUTTON);
            lcd_clear();
            if (device_mode == 4) device_mode = 0;
            else device_mode++;               
        }
        
//--------->CLOCK MODE<---------
        if (device_mode == 0){
            rtc_readTime(0x00); //seconds
            rtc_readDate(0x03); //day
            rtc_readDate(0x04); //date
            rtc_readDate(0x05); //month
            rtc_readDate(0x06); //year
          
            sec_1 = rtc_convUpperNibble(sec);
            sec_0 = rtc_convLowerNibble(sec);
            
            lcd_setCursor(1,1);
            lcd_writeString("Time: ");
            lcd_writeChar(hour_1);
            lcd_writeChar(hour_0);
            lcd_writeChar(':');
            lcd_writeChar(min_1);
            lcd_writeChar(min_0);
            lcd_writeChar(':');
            lcd_writeChar(sec_1);
            lcd_writeChar(sec_0);
            
            day_1 = rtc_convUpperNibble(date);
            day_0 = rtc_convLowerNibble(date);
            month_1 = rtc_convUpperNibble(month);
            month_0 = rtc_convLowerNibble(month);
            year_1 = rtc_convUpperNibble(year);
            year_0 = rtc_convLowerNibble(year);
            
            lcd_setCursor(2,1);
            lcd_writeString("Date:");
            lcd_writeChar(day_1);
            lcd_writeChar(day_0);
            lcd_writeChar('/');
            lcd_writeChar(month_1);
            lcd_writeChar(month_0);
            lcd_writeChar('/');
            lcd_writeChar(year_1);
            lcd_writeChar(year_0);      
            
            switch(week_days[--day]){
                case 'S':
                    lcd_writeString("Sun");
                    break;
                case 'M':
                    lcd_writeString("Mon");
                    break;
                case 'T':
                    lcd_writeString("Tue");
                    break;
                case 'W':
                    lcd_writeString("Wed");
                    break;
                case 't':
                    lcd_writeString("Thu");
                    break;
                case 'F':
                    lcd_writeString("Fri");
                    break;
                case 's':
                    lcd_writeString("Sat");
                    break;
                default:
                    lcd_writeString("ERR");
                    break;
            }
        }
        
//--------->SET_ALARM_MODE<---------
        else if (device_mode == 1){
            char trigger;
            
            lcd_setCursor(1,1);
            lcd_writeString("ALARM MODE");
            
            lcd_setCursor(2,1);
            lcd_writeString("Alarm: ");
            lcd_writeChar(alarm_digit[0]);
            lcd_writeChar(alarm_digit[1]);
            lcd_writeChar(':');
            lcd_writeChar(alarm_digit[2]);
            lcd_writeChar(alarm_digit[3]);
            lcd_setCursor(2,8);
            
            if (MIDDLE_BUTTON == 0){
                if(alarm_status == 0){
                    while(!MIDDLE_BUTTON);
                    alarm_status = 1;
                    time_aux = 1;
                    lcd_setCursor(1,1);
                    lcd_writeString("ALARM MODE");
                    lcd_writeString("  ON");
                }
                else{
                    while(!MIDDLE_BUTTON);
                    alarm_status = 0;
                    lcd_setCursor(1,1);
                    lcd_writeString("ALARM MODE");
                    lcd_writeString(" OFF");
                }
            }
            if(LEFT_BUTTON == 0 && ok==1 && alarm_status==0){
                pos--;
                if(pos < 8)
                    pos = 11;
                while(ok == 1){
                    if(LEFT_BUTTON == 1)
                        ok = 0;
                }
            }
            if(RIGHT_BUTTON == 0 && ok==1 && alarm_status==0){
                pos++;
                if(pos > 11)
                    pos = 8;
                while(ok == 1){
                    if(RIGHT_BUTTON == 1)
                        ok = 0;
                }
            }
            if(UPPER_BUTTON ==0 && ok == 1 && alarm_status == 0){
                alarm_digit[(pos - 8)]++;
                if(alarm_digit[(pos - 8)] > '9')    alarm_digit[(pos - 8)] = '0';
                if(alarm_digit[0] > '2')    alarm_digit[0] = '0';
                if(alarm_digit[2] > '5')    alarm_digit[2] = '0';
                if(alarm_digit[0] == '2')   
                    if(alarm_digit[1] > '4')    alarm_digit[1] = '0';
                while(ok == 1)
                    if(UPPER_BUTTON == 1)   ok = 0;
            }
            if(LOWER_BUTTON == 0 && ok==1 && alarm_status == 0){
                
                alarm_digit[(pos-8)]--;
                if(alarm_digit[(pos - 8)] < '0')    alarm_digit[(pos - 8)] = '9';
                if(alarm_digit[2] < '0')  alarm_digit[2] = '5';
                if(alarm_digit[0] < '0')  alarm_digit[0] = '2';
                if(alarm_digit[0] == '2')
                    if(alarm_digit[1] < '0')
                        alarm_digit[1] = '4';
                while(ok == 1){
                    if(LOWER_BUTTON == 1)
                        ok = 0;
                }
            }
            if(alarm_status == 1 && alarm_digit[0] == hour_1 && alarm_digit[1] == hour_0 && alarm_digit[2] == min_1 && alarm_digit[3] == min_0 && time_aux ==1){
                TRISDbits.TRISD2 = 0;
                for(uint8_t j =0 ; j< 4 ; j++){
                    for(uint8_t i =0 ; i < 200 ; i++){
                        BUZZER = 1;
                        __delay_us(500);
                        BUZZER = 0;
                        __delay_us(500);
                    }
                    __delay_ms(200);
                }
                time_aux = 0;
            }
        }
        
//--------->DIGITAL_TEMP_HUM_MODE<---------
        else if (device_mode == 2){
            int aux;
            
            lcd_setCursor(1,6);
            lcd_writeString("DIGITAL");
            DHT11_Start();
            DHT11_checkResponse();
            
            if (check_bit == 1){
                int rh_octet_1 = DHT11_readData();
                int rh_octet_2 = DHT11_readData();
                int temp_octet_1 = DHT11_readData();
                int temp_octet_2 = DHT11_readData();

                lcd_setCursor(2,1);
                lcd_writeString("Temp=");
                if(temp_octet_2==0){
                    aux=2*temp_octet_1;
                    printInteger(aux);
                }
                else{
                    aux=2*temp_octet_1+1;
                    printInteger(aux);
                }
                
                lcd_setCursor(2,11);
                lcd_writeString("Hum=");
                if(rh_octet_2==0){
                    aux=2*rh_octet_1;
                    printInteger(aux);
                }
                else{
                    aux=2*rh_octet_1+1;
                    printInteger(aux);
                }
            }
            else{
                lcd_setCursor(1,1);
                lcd_writeString("CHECK BIT ERROR");
            }
        }

//--------->ANALOGIC_TEMP_HUM_MODE<---------      
        else if (device_mode == 3){
            lcd_setCursor(1,5);
            lcd_writeString("ANALOGIC");
            
            lcd_setCursor(2,1);
            lcd_writeString("Temp=");
            LM35_initADC();
            printInteger(LM35_readData());
            
            lcd_setCursor(2,11);
            lcd_writeString("Hum=");
            HIH5030_initADC();
            printInteger(HIH5030_readData());
        }
//--------->LDR_1_MODE<--------- 
        else if (device_mode == 4){
            lcd_setCursor(1,5);
            lcd_writeString("LDR MODE");
            
            LDR1_initADC();
            if(LDR1_readData() > 150){
                lcd_setCursor(2,1);
                lcd_writeString("LED OFF");
                LDR_STATUS_LED &= 0;
            }
            else{
                lcd_setCursor(2,1);
                lcd_writeString("LED  ON");
                LDR_STATUS_LED |= 1;                
            }
        }
    }        
}