/**
\file
\addtogroup doc_driver_i2c_example
\brief This file contains sample source codes to demonstrate the I2C SIMPLE Foundation Services APIs.

The example works like this:
1. In this example, we will use I2C SIMPLE drivers to communicate with EEPROM 24LC256
2. First, we will write a block of data to EEPROM. Then we will read the data we wrote into it.
3. We will use UART/EUSART to display what we wrote and read from the EEPROM
    
Hardware Connections
1. Connect the 24LC256 EEPROM to the device:
    - Add Pull up resistors as per the 24LC256 data sheet recommendations
    - Have a common ground connected
    - Connect the appropriate SCL and SDA pins

For this example application to work the following needs to be done:
1. Configure SCL and SDA pins in pin manager (Connected to SCL and SDA of EEPROM 24LC256)
2. Add Delay Foundation Services library
3. Configure the I2C Clock frequency to 100 KHz
4. Add and configure the EUSART/UART module as follows:
    - Enable EUSART: ticked
    - Enable Transmit: ticked
    - Redirect STDIO to USART: ticked
    - Select TX and RX pins in Pin Manager Grid View.
5. Connect the board to a laptop serial port.
6. Open a serial port terminal application (i.e. PuTTY or TeraTerm)
7. Configure the serial port application to the following settings:
    - Baud Speed: same as the UART configuration
    - Data: same as the UART configuration
    - Parity: none
    - Stop bits: same as the UART configuration
    - Flow control: none
    - Receive: Carriage Return
    - Transmit: Carriage Return
    - Local Echo: On
8. Call I2CSIMPLE_example() in the main function.

\copyright (c) 2020 Microchip Technology Inc. and its subsidiaries.
\page License
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "../mcc.h"
#include <stdio.h>

#define EEPROM_DEVICE_ADDRESS 0x50

/**
 *  \ingroup doc_driver_i2c_example
 *  \brief Call this function to run the example
 *  
 *  \param none
 *  
 *  \return none
 */
void I2CSIMPLE_example(void)
{
    // Note that the first 2 bytes of the data to be send will be EEPROM high and low Address respectively.
    // Since we are writing to locations starting from 0x0000, the first 2 bytes will be 0x00.
    // The actual data be written starts from 3rd index of the array
    uint8_t i=0;
    uint8_t sendData[17] = {0x00,0x00,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF};
    uint8_t receiveData[15];
	 
    i2c_writeNBytes(EEPROM_DEVICE_ADDRESS,sendData,sizeof(sendData)); // Writes sendData[] to EEPROM
    // Sent data is temporarily stored in the on-chip page buffer, and will be written into memory once the master has transmitted a Stop condition.
    // The delay is to make sure EEPROM gets enough time to write from on-chip page buffer to memory.
//    DELAY_milliseconds(10); 
	__delay_ms(10);
    //Before we start reading the EEPROM, we understand the EEPROM is currently pointing to the (last write location + 1).

    //For reading from EEPROM, we need to set the EEPROM address to the location we want to start reading from.
    // For this purpose, we will first send the EEPROM high and low address to point to the correct location.
    //For our example we are reading from 0x0000.
    i2c_writeNBytes(EEPROM_DEVICE_ADDRESS,sendData,2);

    //Once the eeprom address is set, we can simply start reading N number of bytes starting from that location.
    i2c_readNBytes(EEPROM_DEVICE_ADDRESS,receiveData,sizeof(receiveData));
	
    //Print data to serial terminal
    printf("EEPROM High Address:0x%x\n\r",sendData[0]);
    printf("EEPROM Low Address:0x%x\n\r",sendData[1]);
    printf("\nData to be written to EEPROM:\n\n\r");
    for(i=2;i<sizeof(sendData);i++)
    {
        printf("0x%x  ",sendData[i]);
        if((i%5)==1)
            printf("\n\r");
    }
    
    printf("\n\rEEPROM Read Data:\n\n\r");
    
    for(i=0;i<sizeof(receiveData);i++)
    {
        printf("0x%x  ",receiveData[i]);
        if(((i+1)%5)==0){
            printf("\n\r");
        }
    }
    printf("-----------------------------------------------\n\r");
    while (1){
    }
}
/**
 End of File
*/

