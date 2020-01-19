/******************************************************************************
 * Created by Jarni Vanmal
 * helloworld.c: Ultrasoon
 ******************************************************************************/

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "sleep.h"

#include "ultrasoon.h"
u32 data;


int main()
{
    init_platform();

    print("Project 3: Ultrasoon\n\r");
    print("By Jarni Vanmal");

    for(;;)
    {
    	data = ULTRASOON_mReadReg(XPAR_ULTRASOON_0_S00_AXI_BASEADDR, ULTRASOON_S00_AXI_SLV_REG0_OFFSET);
    	if (data <= 270){	// Ignoring errors
    		printf("The distance is %d cm.\n\r", (int) data);
    	}
    	sleep_A9(1);
    }

    cleanup_platform();
    return 0;
}



