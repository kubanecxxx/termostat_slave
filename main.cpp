#include "ch.hpp"
#include "hal.h"

#include "delayclass.h"
#include "rfmIncludeCpp.h"
#include "Wireless.h"

int main(void)
{
	halInit();
	chibios_rt::System::Init();
	chRegSetThreadName("Main");
	chThdSleepMilliseconds(1000);

	//init wireless module
	new Wireless;
	chThdSetPriority(NORMALPRIO - 1);

	while (TRUE)
	{
		delay_process::Play();
		chThdSleepMilliseconds(1);
	}
}

