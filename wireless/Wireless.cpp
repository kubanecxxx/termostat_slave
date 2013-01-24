/**
 * @file Wireless.cpp
 * @author kubanec
 * @date 23.12.2012
 *
 */

#include "Wireless.h"
#include "delayclass.h"

using namespace rfm;

uint32_t Wireless::Temperature = 0;
uint32_t Wireless::TemperatureBojler = 0;
uint32_t Wireless::Relay = 0;
chibios_rt::Timer * Wireless::timer = NULL;

#define TIMER_TIME (S2ST(60))
#define HeatEnable()  	palSetPad(GPIOA,6)
#define HeatDisable()		palClearPad(GPIOA,6)

const ApplicationLayer::user_callbacks_t Wireless::callbacks[3] =
{
{ NULL, &TemperatureBojler },
{ relay_cb, &Relay },
{ NULL, &Temperature } };

static const I2CConfig i2conf =
{ OPMODE_I2C, 20000, STD_DUTY_CYCLE };

Wireless::Wireless()
{
	HeatDisable();

	rfm::ApplicationLayer::Init(SLAVE_ADDR, 0, NULL, callbacks, 3);
	new delay_class(RefreshTemperature, NULL, 1000);
	timer = new chibios_rt::Timer();
	chSysLock()
	;
	timer->Set(TIMER_TIME, DisableHeating, NULL);
	chSysUnlock()
	;

	palSetPadMode(GPIOA, 6, PAL_MODE_OUTPUT_PUSHPULL);

	//setup I2C for temperature measuring
	palSetPadMode(GPIOB, 10, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
	palSetPadMode(GPIOB, 11, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);

	uint8_t txbuf[3];
	i2cStart(&I2CD2, &i2conf);

	//sleep mode
	txbuf[0] = 1;
	txbuf[1] = 1;
	i2cMasterTransmit(&I2CD2, I2C_TEMP_ADDRESS, txbuf, 2, NULL, 0);
	asm("nop");
}

void Wireless::relay_cb(bool, rfm::ApplicationLayer::user_cb_packet_t, uint8_t,
		uint16_t, uint32_t)
{
	if (Relay)
		HeatEnable();
	else
		HeatDisable();

	if (timer->IsArmed())
	{
		chSysLock()
		;
		timer->Reset();
		chSysUnlock()
		;
	}
	chSysLock()
	;
	timer->Set(TIMER_TIME, DisableHeating, NULL);
	chSysUnlock()
	;
}

void Wireless::DisableHeating(void *)
{
	HeatDisable();
	chSysLockFromIsr()
	;
	timer->Set(TIMER_TIME, DisableHeating, NULL);
	chSysUnlockFromIsr()
	;
}

void Wireless::RefreshTemperature(void *)
{
	int16_t temp;
	uint8_t txbuf[2];
	uint8_t rxbuf[2];

	//wake up
	txbuf[0] = 1;
	txbuf[1] = 0;
	i2cMasterTransmit(&I2CD2, I2C_TEMP_ADDRESS, txbuf, 2, NULL, 0);
	chibios_rt::BaseThread::Sleep(MS2ST(200) );

	txbuf[0] = 0;
	i2cMasterTransmit(&I2CD2, I2C_TEMP_ADDRESS, txbuf, 1, rxbuf, 2);
	temp = rxbuf[0];
	TemperatureBojler = temp;

	//sleep mode
	txbuf[0] = 1;
	txbuf[1] = 1;
	i2cMasterTransmit(&I2CD2, I2C_TEMP_ADDRESS, txbuf, 2, NULL, 0);
}
