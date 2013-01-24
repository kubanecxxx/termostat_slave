/**
 * @file Wireless.h
 * @author kubanec
 * @date 23.12.2012
 *
 */

#ifndef WIRELESS_H_
#define WIRELESS_H_

#include "rfmIncludeCpp.h"

#define SLAVE_ADDR 5
#define RELAY_ADDR 1
#define TEMP_ADDR 0
#define I2C_TEMP_ADDRESS 0b1001000
#define I2C_TEMP_ADDRESS_BOJLER 0b1001111

class Wireless
{
public:
	Wireless();

private:
	const static rfm::ApplicationLayer::user_callbacks_t callbacks[3];

	static void relay_cb(bool ok, rfm::ApplicationLayer::user_cb_packet_t type, uint8_t address,
			uint16_t modbus_addr, uint32_t modbus_data);
	static void RefreshTemperature(void * arg);
	static void DisableHeating(void * arg);
	static uint32_t Temperature;
	static uint32_t TemperatureBojler;
	static uint32_t Relay;
	static chibios_rt::Timer * timer;

};

#endif /* WIRELESS_H_ */
