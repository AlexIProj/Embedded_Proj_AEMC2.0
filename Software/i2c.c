#include <xc.h>
#include "i2c.h"

unsigned char RS, i2c_add, backlight_state = LCD_BACKLIGHT;  //putem scoate variabila asta

//---------->I2C<----------
void i2c_masterInit(void){
    SSPCONbits.SSPM |= 0b1000;
    SSPCONbits.SSPEN |= 1;
    SSPCON2 &= 0x00;
    
    SSPSTAT &= 0x00;
    SSPADD = ((_XTAL_FREQ/4)/i2c_baudRate)-1;
}   

void i2c_masterWait(void){
    while((SSPSTAT & 0b00000100) || (SSPCON2 & 0b00011111));
    //wait while S = 1 (Start bit)
    //OR
    //SEN=1 || RSEN=1 || PEN=1 || RCEN=1 || ACKEN=1
}

void i2c_masterStart(void){
    i2c_masterWait();
    SSPCON2bits.SEN |= 1;    //Initiate Start condition on SDA and SCL pins. Automatically cleared by hardware
}

unsigned char i2c_masterRead(void){
    i2c_masterWait();
    SSPCON2bits.RCEN |= 1;
    return SSPBUF;
}

void i2c_masterRepeatedStart(void){
    i2c_masterWait();
    SSPCON2bits.RSEN |= 1;   //Initiate Repeated Start condition on SDA and SCL pins. Automatically cleared by hardware. 
}

void i2c_masterStop(void){
    i2c_masterWait();
    SSPCON2bits.PEN |= 1;    //Initiate Stop condition on SDA and SCL pins. Automatically cleared by hardware
}

unsigned char i2c_masterWrite(unsigned char data){
    i2c_masterWait();
    SSPBUF = data;
    return ACKSTAT;
}

unsigned char i2c_readByte(uint8_t ack){
    uint8_t incoming_data;
    i2c_masterWait();
    SSPCON2bits.RCEN |= 1;       //Enables Receive mode for I2C
    
    i2c_masterWait();
    incoming_data = SSPBUF;
    
    i2c_masterWait();
    SSPCON2bits.ACKDT |= (ack)?0:1;
    SSPCON2bits.ACKEN |= 1;
    return incoming_data;
}

//---------->LCD<----------
void lcd_init(unsigned char i2c_Add){
    i2c_add = i2c_Add;
    //io_expanderWrite(0x00);
    __delay_ms(30);
    lcd_cmd(0x03);
    __delay_ms(5);
    lcd_cmd(0x03);
    __delay_ms(5);
    lcd_cmd(0x03);
    __delay_ms(5);
    lcd_cmd(LCD_RETURN_HOME);
    __delay_ms(5);
    lcd_cmd(0x20 | (LCD_TYPE << 2));
    __delay_ms(50);
    lcd_cmd(LCD_TURN_ON);
    __delay_ms(50);
    lcd_cmd(LCD_CLEAR);
    __delay_ms(50);
    lcd_cmd(LCD_ENTRY_MODE_SET | LCD_RETURN_HOME);
    __delay_ms(50);
}

void io_expanderWrite(unsigned char data){
    i2c_masterStart();
    i2c_masterWrite(i2c_add);
    i2c_masterWrite(data | backlight_state);
    i2c_masterStop();
}

void lcd_write4Bit(unsigned char nibble){
    nibble |= RS;
    io_expanderWrite(nibble | 0x04);
    io_expanderWrite(nibble & 0xFB);
    __delay_us(50);
}

void lcd_cmd(unsigned char cmd){
    RS = 0;
    lcd_write4Bit(cmd & 0xF0);
    lcd_write4Bit((cmd << 4) & 0xF0);
}

void lcd_writeChar(char data){
    RS = 1;
    lcd_write4Bit(data & 0xF0);
    lcd_write4Bit((data << 4) & 0xF0);
}

void lcd_writeString(char* str){
    for(int i = 0;str[i] != '\0';i++)
        lcd_writeChar(str[i]);
}

void lcd_setCursor(unsigned char row, unsigned char col){
    if(row == 1)
        lcd_cmd(0x80 + col - 1);
    else if(row == 2)
        lcd_cmd(0xC0 + col - 1);
}

void backlight(){
    backlight_state = LCD_BACKLIGHT;
    io_expanderWrite(0);
}

void noBacklight(){
    backlight_state = LCD_NOBACKLIGHT;
    io_expanderWrite(0);
}

void lcd_shiftLeft(){
    lcd_cmd(0x18);
    __delay_us(40);
}

void lcd_shiftRight(){
    lcd_cmd(0x1C);
    __delay_us(40);
}

void lcd_clear(){
    lcd_cmd(0x01);
    __delay_us(40);
}

void printInteger(int num) {
    int cifra;
    int divisor = 1;
    while (num / divisor >= 10) { 
        divisor *= 10;
    }
    while (divisor > 0) { 
        cifra = (num / divisor) % 10; 
       lcd_writeChar(cifra+'0'); 
        divisor /= 10; 
    }
}

