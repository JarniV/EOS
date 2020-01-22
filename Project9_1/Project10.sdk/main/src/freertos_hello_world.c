/******************************************************************************
 * Created by Jarni Vanmal
 ******************************************************************************/

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Xilinx includes. */
#include "xil_printf.h"
#include "xparameters.h"

#define TIMER_ID	1
#define DELAY_10_SECONDS	10000UL
#define DELAY_1_SECOND		1000UL
#define DELAY_500_MSECOND	250UL
#define DELAY_100_MSECOND	100UL
#define TIMER_CHECK_THRESHOLD	9

static void prvSend( void *pvParameters );
static void prvReceive( void *pvParameters );
static void prvExtra( void *pvParameters );

static TaskHandle_t xSend;
static TaskHandle_t xReceive;
static TaskHandle_t xExtra;

static QueueHandle_t xQueueButton = NULL;
static QueueHandle_t xQueueUltrasoon = NULL;

#include <stdio.h>
#include "ultrasoon.h"
#include "DobbelSteen.h"
#include "xgpiops.h"
#include "xgpio_l.h"
#include "xil_io.h"

#define BUTTON_CHANNEL	1

u32 Input_Pin = 0;    // Switch button
XGpioPs GpioPS;

int main( void )
{
	xil_printf("Jarni Vanmal\n\r");

    //TAskS
	xTaskCreate( prvSend, 	( const char * ) "Send", 	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &xSend );
	xTaskCreate( prvReceive,( const char * ) "Receive", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &xReceive );
	xTaskCreate( prvExtra, 	( const char * ) "Extra", 	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &xExtra );

	//QUEUES
	xQueueButton 	=  xQueueCreate		( 	1, sizeof( u32 ) );		// Queue with U32					// 1 = queue length
	xQueueUltrasoon =  xQueueCreate		( 	1, sizeof( u32 ) );		// Queue with Char value
	configASSERT( xQueueButton );
	configASSERT( xQueueUltrasoon );

	vTaskStartScheduler();

	for( ;; );
}


/*-----------------------------------------------------------*/
static void prvSend( void *pvParameters )
{
	const TickType_t x500msecond = pdMS_TO_TICKS( DELAY_500_MSECOND );

	u32 sendDataButton = 0;
	u32 sendDataUltrasoon = 0;

	for( ;; )
	{
		vTaskDelay( x500msecond );

		sendDataButton = XGpio_ReadReg(XPAR_GPIO_0_BASEADDR, ((BUTTON_CHANNEL - 1) * XGPIO_CHAN_OFFSET) + XGPIO_DATA_OFFSET);
		xQueueSend( xQueueButton,	&sendDataButton, 	0UL );

		sendDataUltrasoon = ULTRASOON_mReadReg(XPAR_ULTRASOON_0_S00_AXI_BASEADDR, ULTRASOON_S00_AXI_SLV_REG0_OFFSET);
		if (sendDataUltrasoon <= 270){	// Ignoring errors
			xQueueSend( xQueueUltrasoon,	&sendDataUltrasoon, 	0UL );
		}

	}
}

static void prvReceive( void *pvParameters )
{
	u32 RecvDataButton;
	u32 RecvDataUltrasoon;

	int number = 0;

	for( ;; )
	{
		xQueueReceive( 	xQueueButton,	&RecvDataButton,	portMAX_DELAY );
		xQueueReceive( 	xQueueUltrasoon,	&RecvDataUltrasoon,	portMAX_DELAY );

		if(RecvDataUltrasoon <= 10 ){number = 0;}
		else{
			switch(RecvDataButton)
			{
				case 1: number++; break;
				case 2: number--; break;
			}
		}

		if		(number <= 0){ number = 0;}
		else if	(number >= 6){ number = 6;}

		DOBBELSTEEN_mWriteReg(XPAR_DOBBELSTEEN_0_S00_AXI_BASEADDR, 0,	number << 28);

	}
}

static void prvExtra( void *pvParameters )
{

	XGpioPs_Config *GPIOConfigPtr;
	GPIOConfigPtr = XGpioPs_LookupConfig(XPAR_PS7_GPIO_0_DEVICE_ID);
	XGpioPs_CfgInitialize(&GpioPS, GPIOConfigPtr,GPIOConfigPtr->BaseAddr);

	XGpioPs_SetDirectionPin(&GpioPS, Input_Pin, 0);

	const TickType_t x500msecond = pdMS_TO_TICKS( DELAY_500_MSECOND );

	_Bool toggle = TRUE;
	u32 temp;

	for( ;; )
	{
		vTaskDelay( x500msecond );

		temp = XGpioPs_ReadPin(&GpioPS, Input_Pin);

		if		(temp && toggle)
		{
			xil_printf("Pause");
			vTaskSuspend	( xSend);
			vTaskSuspend	( xReceive);
			toggle = FALSE;
		}
		else if	(temp && !toggle)
		{
			xil_printf("Resume");
			vTaskResume	( xSend);
			vTaskResume	( xReceive);
			toggle = TRUE;
		}
	}
}