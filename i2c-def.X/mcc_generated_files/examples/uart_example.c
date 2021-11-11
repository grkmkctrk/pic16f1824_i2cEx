/**
\file
\addtogroup doc_driver_uart_example
\brief This file contains sample source codes to demonstrate the UART Foundation Services APIs.

For this example application to work the following needs to be done:
1. Configure the EUSART module as follows:
    - Enable EUSART: ticked
    - Enable Transmit: ticked
    - Enable Receive: ticked
    - Enable EUSART Interrupts: ticked
    - Redirect STDIO to USART: ticked
    - Configure other settings as desired.
    - Select TX and RX pins in Pin Manager: Grid View.
2. Connect the board to a laptop serial port.
3. Open a serial port terminal application (i.e. PuTTY or TeraTerm)
4. Configure the serial port application to the following settings:
    - Baud Speed: same as the UART configuration
    - Data: same as the UART configuration
    - Parity: none
    - Stop bits: same as the UART configuration
    - Flow control: none
    - Receive: Carriage Return
    - Transmit: Carriage Return
    - Local Echo: On
5. Call UART_example_<USE_CASE>() from main().   

This example works like this:
1. Upon reset, the application will display "START" to test UART_Write function.
2. The program will then print the following to test printf support:
    Hello World! 1\n
    Hello World! 2\n
    Hello World! 3\n
3. The program will then display "Receive test: Type a message then press enter (max 16 chars):"
4. Type a message no longer than 16 characters.
5. The program will then print "You typed:" and echo back your message. This will ensure that the UART_Read function is working.
6. If everything is working fine, the program will print "OK."

Note: To test other instances, feel free to modify this file.
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

#include "../drivers/uart.h" 
#include "uart_example.h"
#include <string.h>
#include <stdio.h>

static uint8_t rx[16];
char *string = "Transmit test\n";

/**
 *  \ingroup doc_driver_uart_example
    Call this function from main to send a string of characters. This can be a light-weight alternative to printf, e.g. for a string of ASCII characters.
    @param none
*/
void UART_example_WRITE_STRING(char *str)
{
    uint8_t count = 0; 
	
    while (count < strlen(str)){
        uart[UART0].Write(str[count]);
        count++;
    }   
}
/**
 End of File
 */
