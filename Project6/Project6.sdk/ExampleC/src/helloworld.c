/******************************************************************************
 * Created by Jarni Vanmal
 * helloworld.c: Using Buttons
 ******************************************************************************/

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "sleep.h"

#include "xgpio_l.h"
#define BUTTON_CHANNEL	1

int main()
{
    init_platform();

    print("Project 6: Using Buttons\n\r");
    print("By Jarni Vanmal");

    u32 button;

    for(;;)
    {
    	button = XGpio_ReadReg(XPAR_GPIO_0_BASEADDR, ((BUTTON_CHANNEL - 1) * XGPIO_CHAN_OFFSET) + XGPIO_DATA_OFFSET);

    	switch(button)
    	{
    		case 1: xil_printf("Button 1\n\r"); break;
    		case 2: xil_printf("Button 2\n\r"); break;
    		case 4: xil_printf("Button 3\n\r"); break;
    		case 8: xil_printf("Button 4\n\r"); break;
    	//	case 16:xil_printf("Button 5\n\r"); break;
    	}
    	sleep_A9(1);
    }

    cleanup_platform();
    return 0;
}
