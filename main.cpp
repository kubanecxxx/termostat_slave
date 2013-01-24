#include "ch.hpp"
#include "hal.h"

#include "delayclass.h"
#include "rfmIncludeCpp.h"
#include "Wireless.h"

void blik(void *)
{
	palTogglePad(GPIOA, 6);
}

int main(void)
{
	halInit();
	chibios_rt::System::Init();
	chRegSetThreadName("Main");
	chThdSleepMilliseconds(1000);

	palSetPadMode(GPIOA, 6, PAL_MODE_OUTPUT_PUSHPULL);
	new delay_class(blik, NULL, 500);

	//init wireless module
	new Wireless;
	chThdSetPriority(NORMALPRIO - 1);

	while (TRUE)
	{
		delay_process::Play();
		chThdSleepMilliseconds(1);
	}
}

