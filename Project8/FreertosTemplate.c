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
#define DELAY_500_MSECOND	500UL
#define DELAY_100_MSECOND	100UL
#define TIMER_CHECK_THRESHOLD	9

static void prvSend( void *pvParameters );
static void prvReceive( void *pvParameters );
static void prvExtra( void *pvParameters );

static TaskHandle_t xSend;
static TaskHandle_t xReceive;
static TaskHandle_t xExtra;

static QueueHandle_t xQueueU32 = NULL;
static QueueHandle_t xQueueChar = NULL;

static TimerHandle_t xTimer1 = NULL;
static TimerHandle_t xTimer2 = NULL;
static void vTimer1( TimerHandle_t pxTimer1 );
static void vTimer2( TimerHandle_t pxTimer2 );

int main( void )
{
	xil_printf("Jarni Vanmal\n\r");

    //TAskS
	xTaskCreate( prvSend, 	( const char * ) "Send", 	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &xSend );
	xTaskCreate( prvReceive,( const char * ) "Receive", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &xReceive );
	xTaskCreate( prvExtra, 	( const char * ) "Extra", 	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &xExtra );

	//QUEUES
	xQueueU32 =  xQueueCreate	( 	1, sizeof( u32 ) );		// Queue with U32					// 1 = queue length
	xQueueChar = xQueueCreate	( 	1, sizeof( char ) );	// Queue with Char value
	configASSERT( xQueueU32 );
	configASSERT( xQueueChar );

	//TIMERS
	const TickType_t x10seconds = pdMS_TO_TICKS( DELAY_10_SECONDS );
	const TickType_t x1second 	= pdMS_TO_TICKS( DELAY_1_SECOND );

	xTimer1 = xTimerCreate( (const char *) "Timer1",	x10seconds,	pdFALSE, (void *) TIMER_ID,	vTimer1);	//pdFALSE -> gebeurt maar 1 keer na 10 seconden
	xTimer2 = xTimerCreate( (const char *) "Timer2",	x1second,	pdTRUE, (void *) TIMER_ID,	vTimer2);	//pdTRUE  -> gebeurt elke seconde
	configASSERT( xTimer1 );
	configASSERT( xTimer2 );
	xTimerStart( xTimer1, 0 );
	xTimerStart( xTimer2, 0 );

	vTaskStartScheduler();

	for( ;; );
}


/*-----------------------------------------------------------*/
static void prvSend( void *pvParameters )
{
	const TickType_t x1second = pdMS_TO_TICKS( DELAY_1_SECOND );
	const TickType_t x500msecond = pdMS_TO_TICKS( DELAY_500_MSECOND );

	u32 sendDataU32 = 0;
	u32 sendDataChar = 0;

	for( ;; )
	{
		vTaskDelay( x1second );
		
		xQueueSend( xQueueU32,	&sendDataU32, 	0UL );
	}
}

static void prvReceive( void *pvParameters )
{
	u32 RecvDataU32;
	u32 RecvDataChar;

	for( ;; )
	{
		xQueueReceive( 	xQueueU32,	&RecvDataU32,	portMAX_DELAY );
		
		xil_printf( "Received %d\r\n", RecvDataU32 );
	}
}

static void prvExtra( void *pvParameters )
{
	for( ;; )
	{
		//Do something


		//vTaskSuspend	( xSend);
		//vTaskResume	( xSend);
		//vTaskSuspend	( xReceive);
		//vTaskResume	( xReceive);
	}
}

static void vTimer1( TimerHandle_t pxTimer )
{
	xil_printf( "Stopped after 10 seconds.\r\n");

	//Stop all tasks and timers
	vTaskDelete	( xSend );
	vTaskDelete	( xReceive );
	vTaskDelete	( xExtra );

	xTimerStop	( xTimer2, 0 );
}

static void vTimer2( TimerHandle_t pxTimer )
{
	xil_printf( "Timer 2 is activated.\r\n");

}
