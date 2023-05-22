#define _XTAL_FREQ 8000000

#define i2c_baudRate 10000

#define LCD_BACKLIGHT           0x08    //display OFF, cursor OFF
#define LCD_NOBACKLIGHT         0x00
#define LCD_FIRST_ROW           0x80
#define LCD_SECOND_ROW          0xC0
#define LCD_CLEAR               0x01
#define LCD_RETURN_HOME         0x02
#define LCD_ENTRY_MODE_SET      0x04    //decrement cursor(shift cursor to left)
#define LCD_CURSOR_OFF          0x0C    //display ON, cursor OFF
#define LCD_UNDERLINE_ON        0x0E   
#define LCD_BLINK_CURSOR_ON     0x0F
#define LCD_MOVE_CURSOR_LEFT    0x10
#define LCD_MOVE_CURSOR_RIGHT   0x14
#define LCD_TURN_ON             0x0C
#define LCD_TURN_OFF            0x08
#define LCD_SHIFT_LEFT          0x18
#define LCD_SHIFT_RIGHT         0x1E
#define LCD_TYPE                2

//---------->MODUL I2C<----------
void i2c_masterInit(void);
void i2c_masterWait(void);
void i2c_masterStart(void);
void i2c_masterRepeatedStart(void);
unsigned char i2c_masterRead(void);
void i2c_masterStop(void);
unsigned char i2c_masterWrite(unsigned char data);
unsigned char i2c_readByte(uint8_t ack);

//---------->MODUL LCD<----------
void lcd_init(unsigned char i2c_Add);
void io_expanderWrite(unsigned char data);
void lcd_write4Bit(unsigned char nibble);
void lcd_cmd(unsigned char cmd);
void lcd_setCursor(unsigned char row,unsigned char col);
void lcd_writeChar(char);
void lcd_writeString(char*);
void backlight();
void noBacklight();
void lcd_shiftRight();
void lcd_shiftLeft();
void lcd_clear();
void printInteger(int);

