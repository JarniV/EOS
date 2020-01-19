/******************************************************************************
 * Created by Jarni Vanmal
 * freertos_hello_world.c: Ultrasoon
 ******************************************************************************/

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Xilinx includes. */
#include "xil_printf.h"
#include "xparameters.h"

#include "Ultrasoon.h"
#include "xil_io.h"

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

int main( void )
{
	const TickType_t x10seconds = pdMS_TO_TICKS( DELAY_10_SECONDS );

    print("Project 3: Ultrasoon\n\r");
    print("By Jarni Vanmal\n\r");

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

		sendData = ULTRASOON_mReadReg(XPAR_ULTRASOON_0_S00_AXI_BASEADDR, ULTRASOON_S00_AXI_SLV_REG0_OFFSET);

		xQueueSend( xQueue,	&sendData, 	0UL );
	}
}

static void prvReceive( void *pvParameters )
{
	u32 RecvData;

	for( ;; )
	{
		xQueueReceive( 	xQueue,	&RecvData,	portMAX_DELAY );
		if (RecvData <= 270){	// Ignoring errors
			xil_printf("The distance is %d cm.\n\r", (int) RecvData);
		}
	}
}

static void vTimerCallback( TimerHandle_t pxTimer )
{

	xil_printf( "Stopped after 10 seconds.");

	vTaskDelete( xSend );
	vTaskDelete( xReceive );
}

