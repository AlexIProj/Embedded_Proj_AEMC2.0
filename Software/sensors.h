#include <xc.h>
#define _XTAL_FREQ 16000000

//---------->DHT11<----------
int check_bit;

char DHT11_readData(void);
void DHT11_Start(void);
void DHT11_checkResponse(void);

//---------->LM35<----------
int LM35_readData(void);
void LM35_initADC(void);

//---------->HIH5030<----------
void HIH5030_initADC(void);
int HIH5030_readData(void);
int roundFloat(float);

//---------->LDR1<----------
void LDR1_initADC(void);
int LDR1_readData(void);