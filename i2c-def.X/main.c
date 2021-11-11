#include "mcc_generated_files/mcc.h"
#include <math.h>

#define controlReg 0xF4 // this is register

// device address
#define i2caddrW  0xEE

short AC1 = 0;
short AC2 = 0;
short AC3 = 0;
unsigned short AC4 = 0;
unsigned short AC5 = 0;
unsigned short AC6 = 0;
short B1 = 0;
short B2 = 0;
short MB = 0;
short MC = 0;
short MD = 0;

long X1 = 0;
long X2 = 0;
long X3 = 0;
long B3 = 0;
long B5 = 0;
unsigned long B4 = 0;
long B6 = 0;
unsigned long B7 = 0;
long UT = 0;
long UP = 0;
short oss = 0;

long press = 0;
long temp = 0;

void bmp180Start(void);
void readCalibrationDatas(void);
uint16_t getUTemp(void); // u mean uncompensated
double realTemp(void);
uint32_t getUpress(uint8_t oss);
double realPress(int oss);

double Temperature = 0.0;
double Pressure = 0.0;

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    I2C_Initialize();
    bmp180Start(); //calibrations
    
    while (1)
    {
        Temperature = realTemp();
        Pressure    = realPress(0);
    }
}

void bmp180Start(void){
    readCalibrationDatas();
}

void readCalibrationDatas(void){
    uint8_t CalibDatas[22] = {0};
    uint8_t startPointOfCalibDatas = 0xAA;
    i2c_readDataBlock(i2caddrW, startPointOfCalibDatas, CalibDatas, 22);
    
    AC1 = ((CalibDatas[0] << 8) | CalibDatas[1]);
    AC2 = ((CalibDatas[2] << 8) | CalibDatas[3]);
    AC3 = ((CalibDatas[4] << 8) | CalibDatas[5]);
    AC4 = (unsigned short) ((CalibDatas[6] << 8) | CalibDatas[7]);
    AC5 = (unsigned short) ((CalibDatas[8] << 8) | CalibDatas[9]);
    AC6 = (unsigned short) ((CalibDatas[10] << 8) | CalibDatas[11]);
    B1  = ((CalibDatas[12] << 8) | CalibDatas[13]);
    B2  = ((CalibDatas[14] << 8) | CalibDatas[15]);
    MB  = ((CalibDatas[16] << 8) | CalibDatas[17]);
    MC  = ((CalibDatas[18] << 8) | CalibDatas[19]);
    MD  = ((CalibDatas[20] << 8) | CalibDatas[21]);
    
}

uint16_t getUTemp(void){
    uint8_t write = 0x2E;
    uint8_t tempRaw[2] = {0};
    i2c_write1ByteRegister(i2caddrW, 0x74, write); __delay_ms(5);
    i2c_readDataBlock(i2caddrW, 0x76, tempRaw, 2);
    return (uint16_t) ((tempRaw[0] << 8) + tempRaw[1]);
}

double realTemp(void){
    UT   = getUTemp();
    X1   = ((UT-AC6)*(AC5/(pow(2, 15))));
    X2   = ((MC*pow(2, 11))/(X1 + MD));
    B5   = (X1 + X2);
    temp = (B5+8)/(pow(2, 4));
    return temp/10.0; // to convert to degree
}

uint32_t getUpress(uint8_t oss){
    uint8_t write = (uint8_t) 0x34+(oss<<6);
    uint8_t pressRAW[3] = {0};
    I2C_Write1ByteRegister(i2caddrW, 0x74, write);
    switch(oss){
        case 0: __delay_ms(5);  break;
        case 1: __delay_ms(8);  break;
        case 2: __delay_ms(14); break;
        case 3: __delay_ms(26); break;
    }
    i2c_readDataBlock(i2caddrW, 0x76, pressRAW, 3);
    return (uint32_t)(((pressRAW[0] << 16)+(pressRAW[1] << 8)) >> (8-oss));
}


double realPress(int oss){
    UP = getUpress(oss);
	X1 = ((UT-AC6) * (AC5/(pow(2,15))));
	X2 = ((MC*(pow(2,11))) / (X1+MD));
	B5 = X1+X2;
	B6 = B5-4000;
	X1 = (B2 * (B6*B6/(pow(2,12))))/(pow(2,11));
	X2 = AC2*B6/(pow(2,11));
	X3 = X1+X2;
	B3 = (((AC1*4+X3)<<oss)+2)/4;
	X1 = AC3*B6/pow(2,13);
	X2 = (B1 * (B6*B6/(pow(2,12))))/(pow(2,16));
	X3 = ((X1+X2)+2)/pow(2,2);
	B4 = AC4*(unsigned long)(X3+32768)/(pow(2,15));
	B7 = ((unsigned long)UP-B3)*(50000>>oss);
	if (B7<0x80000000) press = (B7*2)/B4;
	else press = (B7/B4)*2;
	X1 = (press/(pow(2,8)))*(press/(pow(2,8)));
	X1 = (X1*3038)/(pow(2,16));
	X2 = (-7357*press)/(pow(2,16));
	press = press + (X1+X2+3791)/(pow(2,4));  
    return press;
}


/**
 End of File
*/