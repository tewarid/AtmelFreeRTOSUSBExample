#include "asf.h"

void usbTask(void *pvParameters)
{
	int len;
	const int BUF_SIZE = 10;
	char buf[BUF_SIZE];

	while (true) {
		if (udi_cdc_is_rx_ready()) {
			
			// blocks until BUF_SIZE bytes are received
			len = udi_cdc_read_buf(buf, BUF_SIZE);
			
			if (len == BUF_SIZE) continue;
			
			while (!udi_cdc_is_tx_ready()) {
				// Fifo full
			}
			
			udi_cdc_write_buf(buf, BUF_SIZE);
		}
		//vTaskDelay(1); // slows things down quite significantly
	}
}

int main( void )
{
	// Configure Osc0 in crystal mode (i.e. use of an external crystal source, with
	// frequency FOSC0) with an appropriate startup time then switch the main clock
	// source to Osc0.
	pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);
	
	portDBG_TRACE("Starting the FreeRTOS AVR32 UC3 Demo...");
	
	udc_start();

	/* Start the demo tasks defined within this file, specifically the check
	task as described at the top of this file. */
	xTaskCreate(
		usbTask
		,  (const signed portCHAR *)"ErrCheck"
		,  configMINIMAL_STACK_SIZE
		,  NULL
		,  tskIDLE_PRIORITY
		,  NULL );

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* Will only get here if there was insufficient memory to create the idle
	task. */

	return 0;
}