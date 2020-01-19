/******************************************************************************
 * Created by Jarni Vanmal
 * helloworld.c: Seven segment
 ******************************************************************************/

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

#include "sevenSegmentIP.h"
#include "xparameters.h"
#include "xil_io.h"
#include "sleep.h"

#define A  0b001000000
#define B  0b010000000
#define C  0b000010000
#define D  0b000100000
#define E  0b000000100
#define F  0b000001000
#define G  0b000000001
#define DP 0b000000010

int segment7value(int number);

int main()
{
    init_platform();

    print("Project 5: Seven Segment\n\r");
    print("By Jarni Vanmal");

    for(;;)
    {
    	for(int i=0; i<7; i++)
		{
			SEVENSEGMENTIP_mWriteReg(XPAR_SEVENSEGMENTIP_0_S00_AXI_BASEADDR, 0, segment7value(i) );
			xil_printf("Value %d\n\r" , i);
			sleep_A9(1);
		}
    }

    cleanup_platform();
    return 0;
}

int segment7value(int number)
{
	int value=0;

	switch (number)	{
		case 0:	value = A + B + C + D + E + F;		break;
		case 1:	value = B + C;						break;
		case 2:	value = A + B + G + E + D;			break;
		case 3:	value = A + B + G + C + D;			break;
		case 4:	value = F + G + B + C;		   	 	break;
		case 5:	value = A + F + G + C + D;			break;
		case 6:	value = A + F + G + E + C + D;		break;
		case 7:	value = A + B + C;					break;
		case 8:	value = A + B + C + D + E + F + G;	break;
		case 9:	value = A + B + C + D + F + G;		break;
		case 10:value = E + F + A + B + G + C;		break;
		case 11:value = F + G + C + D + E;			break;
		case 12:value = A + F + E + D;				break;
		case 13:value = B + G + E + D + C;			break;
		case 14:value = A + F + G + E + D;			break;
		case 15:value = A + F + G + E;				break;
		default:value = DP;
	}
	return value;
}