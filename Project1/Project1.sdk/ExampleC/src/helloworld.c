/******************************************************************************
 * Created by Jarni Vanmal
 * helloworld.c: Using PL switch, PL led and PS led
 *
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 ******************************************************************************
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "sleep.h"

//Project 1
#include "xparameters.h"
#include "xgpiops.h"
#include "xgpio.h"
#include "xstatus.h"

/* Minized */
u32 Input_Pin = 0;    // Switch button
u32 Output_Pin = 52;  // LED 1
u32 Output_Pin2 = 53; // LED 2

XGpioPs GpioPS;
XGpio GpioPL;

#define LED_CHANNEL1 1	// LED 1
#define LED_CHANNEL2 2  // LED 2
#define LED 0x01

int main()
{
    init_platform();

    print("Project 1: Using PL switch, PL led and PS led\n\r");
    print("By Jarni Vanmal\n\r");

    // Initialize
	XGpioPs_Config *GPIOConfigPtr;
	GPIOConfigPtr = XGpioPs_LookupConfig(XPAR_PS7_GPIO_0_DEVICE_ID);
	XGpioPs_CfgInitialize(&GpioPS, GPIOConfigPtr,GPIOConfigPtr->BaseAddr);

	// PL LED 1 AND 2
	XGpio_Initialize(&GpioPL, XPAR_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&GpioPL, LED_CHANNEL1, ~LED);
	XGpio_SetDataDirection(&GpioPL, LED_CHANNEL2, ~LED);

	// PS LED1
	XGpioPs_SetDirectionPin(&GpioPS, Output_Pin, 1);
	XGpioPs_SetOutputEnablePin(&GpioPS, Output_Pin, 1);
	// PS LED2
	XGpioPs_SetDirectionPin(&GpioPS, Output_Pin2, 1);
	XGpioPs_SetOutputEnablePin(&GpioPS, Output_Pin2, 1);
	// PS Button
	XGpioPs_SetDirectionPin(&GpioPS, Input_Pin, 0);

	u32 btnval = 0 ;
	int state = 0;

    for(;;)
    {
    	btnval = XGpioPs_ReadPin(&GpioPS, Input_Pin);

    	if(btnval == 1){
    		state++;
			if (state == 4){state = 0;}
    	}

    	switch(state)
		{
			case 0:
				XGpioPs_WritePin(&GpioPS, Output_Pin, 0);
				XGpioPs_WritePin(&GpioPS, Output_Pin2, 0);
				break;
			case 1:
				XGpioPs_WritePin(&GpioPS, Output_Pin, 1);
				XGpioPs_WritePin(&GpioPS, Output_Pin2, 0);
				break;
			case 2:
				XGpioPs_WritePin(&GpioPS, Output_Pin, 0);
				XGpioPs_WritePin(&GpioPS, Output_Pin2, 1);
				break;
			case 3:
				XGpioPs_WritePin(&GpioPS, Output_Pin, 1);
				XGpioPs_WritePin(&GpioPS, Output_Pin2, 1);
				break;
		}

    	if(XGpio_DiscreteRead(&GpioPL, LED_CHANNEL1)){
			XGpio_DiscreteClear(&GpioPL, LED_CHANNEL1, LED);
			XGpio_DiscreteWrite(&GpioPL, LED_CHANNEL2, LED);
		}
		else{
			XGpio_DiscreteWrite(&GpioPL, LED_CHANNEL1, LED);
			XGpio_DiscreteClear(&GpioPL, LED_CHANNEL2, LED);
		}

    	sleep_A9(1);
    }

    cleanup_platform();
    return 0;
}
