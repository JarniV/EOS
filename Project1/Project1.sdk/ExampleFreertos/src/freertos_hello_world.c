/******************************************************************************
 * Created by Jarni Vanmal
 * freertos_hello_world.c: Using PL switch, PL led and PS led
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
#define TIMER_CHECK_THRESHOLD	9

static void prvSend( void *pvParameters );
static void prvReceive( void *pvParameters );
static void vTimerCallback( TimerHandle_t pxTimer );

static TaskHandle_t xSend;
static TaskHandle_t xReceive;

static QueueHandle_t xQueue = NULL;
static TimerHandle_t xTimer = NULL;

//--------------------------------------------------------------------
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
//--------------------------------------------------------------------

int main( void )
{
	const TickType_t x10seconds = pdMS_TO_TICKS( DELAY_10_SECONDS );

    print("Project 1: Using PL switch, PL led and PS led\n\r");
    print("By Jarni Vanmal\n\r");

    //--------------------------------------------------------------------
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
    //--------------------------------------------------------------------

	xTaskCreate( prvSend, ( const char * ) "Send", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &xSend );
	xTaskCreate( prvReceive, ( const char * ) "Receive", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xReceive );

	xQueue = xQueueCreate( 	1, sizeof( u32 ) );	// We use u32 data in the queue for this project
	configASSERT( xQueue );

	xTimer = xTimerCreate( (const char *) "Timer",	x10seconds,	pdFALSE, (void *) TIMER_ID,	vTimerCallback);
	configASSERT( xTimer );
	xTimerStart( xTimer, 0 );

	vTaskStartScheduler();

	for( ;; );
}


/*-----------------------------------------------------------*/
static void prvSend( void *pvParameters )
{
	const TickType_t x1second = pdMS_TO_TICKS( DELAY_1_SECOND );
	u32 sendData;

	for( ;; )
	{
		vTaskDelay( x1second );

		sendData = XGpioPs_ReadPin(&GpioPS, Input_Pin);

		xQueueSend( xQueue,	&sendData, 	0UL );
	}
}

static void prvReceive( void *pvParameters )
{
	u32 RecvData;
	int state = 0;

	for( ;; )
	{
		xQueueReceive( 	xQueue,	&RecvData,	portMAX_DELAY );

		xil_printf( "Received %d\r\n", RecvData );

    	if(RecvData == 1){
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
	}
}

static void vTimerCallback( TimerHandle_t pxTimer )
{
	xil_printf( "Stopped after 10 seconds.");

	vTaskDelete( xSend );
	vTaskDelete( xReceive );
}

