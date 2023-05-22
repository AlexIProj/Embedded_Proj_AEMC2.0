#include "sensors.h"
#include "i2c.h"


//---------->DHT11<----------
#define DHT11_DATA          PORTDbits.RD0
#define DHT11_DATA_LATCH    TRISDbits.TRISD0
#define LM35_DATA_LATCH     TRISAbits.TRISA0
#define HIH5030_DATA_LATCH  TRISAbits.TRISA1
#define LDR1_DATA_LATCH     TRISAbits.TRISA2

int check_bit;
int adc_val, adc_val1;

char DHT11_readData(){
    char i, data = 0;
    for(i = 0; i < 8; i++){
        while(DHT11_DATA==0);
        __delay_us(30);
        if(DHT11_DATA == 0)
            data &= ~(1 << (7 - i));
        else{
            data |= (1 << (7 - i));
            while(DHT11_DATA);
        }
    }
    return data;
}

void DHT11_Start(void){
    DHT11_DATA_LATCH |= 0;
    DHT11_DATA = 0;
    __delay_ms(18);
    DHT11_DATA = 1;
    __delay_us(25);
    DHT11_DATA_LATCH &= 1;

}

void DHT11_checkResponse(void){
    check_bit = 0;
    while(DHT11_DATA == 1);
    while(DHT11_DATA == 0);
    check_bit=1;
    __delay_us(50);
}

//---------->LM35<----------
void LM35_initADC(void){
    LM35_DATA_LATCH |= 1;
    ADCON0 = 0x01;
    ADCON1 = 0x00;
}

int LM35_readData(void){
    ADCON0bits.GO_nDONE |= 1;
    while(ADCON0bits.GO_nDONE);
    adc_val = ADRESH << 2;
    adc_val1 = ADRESL >> 6;
    
    //try to save memory modifying the line code below
    adc_val += adc_val1;
    float temperature = adc_val * 5;
    temperature = temperature / 1024 * 100;
    
    return (int)temperature;
}

//---------->HIH5030<----------
void HIH5030_initADC(void){
    HIH5030_DATA_LATCH |=1;
    ADCON0 = 0x05; 
    ADCON1 = 0x00;
}

int HIH5030_readData(void){
   ADCON0bits.GO_nDONE |= 1;
   while(ADCON0bits.GO_nDONE);
   adc_val = ADRESH << 2;
   adc_val1 = ADRESL >> 6;
   
   adc_val = adc_val + adc_val1;
   float x = adc_val * 5;
   x = x / 1024;
   float RH = (x - 0.95) / (4.939 - 0.95) * 100;

   return roundFloat(RH);
}

int roundFloat(float num){
    int rounded = (int)num;
    float decimal = num - rounded;
    if(decimal >= 0.0)  rounded += (num > 0.0) ? 1 : -1;
    
    return rounded;
}

//---------->LDR1<----------
void LDR1_initADC(void){
    TRISAbits.TRISA2 |=1;
    TRISDbits.TRISD2 &=0;
    ADCON0 = 0x09;
    ADCON1 = 0x00;
}

int LDR1_readData(void){
   ADCON0bits.GO_nDONE |= 1;
   while(ADCON0bits.GO_nDONE);
   adc_val = ADRESH << 2;
   adc_val1 = ADRESL >> 6;
   
   adc_val = adc_val + adc_val1;
   adc_val = (adc_val / 3) - 1;   
   
   return adc_val;
}