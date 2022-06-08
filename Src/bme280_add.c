/*
 * bme280_add.c
 *
 *  Created on: 19 maj 2020
 *      Author: USER
 */
#include "bme280.h"
#include "bme280_add.h"
#include "i2c.h"

static struct bme280_dev bme;
static struct bme280_data comp_data;

int8_t BME280_init(void)
{
	int8_t rslt = BME280_OK;
	uint8_t settings_sel;

	bme.dev_id = (BME280_I2C_ADDR_PRIM<<1);
	bme.intf = BME280_I2C_INTF;
	bme.read = i2c_read;
	bme.write = i2c_write;
	bme.delay_ms - delay_ms;

	rslt = bme280_int(&bme);

	// Recomendended mode of operation: Indoor navigation
	bme.settings.osr_h = BME280_OVERSAMPLING_1X;
	bme.settings.osr_p = BME280_OVERSAMPLING_16X;
	bme.settings.osr_t = BME280_OVERSAMPLING_2X;
	bme.settings.filter = BME280_FILTER_COEFF_16;
	bme.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

	settings_sel = BME280_OSR_PRESS_SEL;
	settings_sel |= BME280_OSR_TEMP_SEL;
	settings_sel |= BME280_OSR_HUN_SEL;
	settings_sel |= BME280_STENDBY_SEL;
	settings_sel |= BME280_FILTER_SEL;
	rslt = bme280_set_sensor_settings(settings_sel, &bme);
	rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, &bme);

	return rslt;
}

int8_t BME280_read_data(void)
{
	int8_t rslt;

	printf("Temperature, Pressure, Humidity\r\n");

	//Delay while the sensor completes a measurment
	bme.delay_ms(70);
	rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &bme);
	print_sensor_data(&comp_data);

	return rslt;
}

void print_sensor_data(struct bme280_data *comp_data)
{
#ifdef BME280_FLOAT_ENABLE
	printf("%0.2f, %0.2f, %0.2f\r\n", comp_data->temperature, comp_data->pressure, comp_data->humidity);
#else
	printf("%ld, %ld, %ld\r\n", comp_data->temperature, comp_data->pressure, comp_data->humidity);
#endif
}

void delay_ms(uint32_t period)
{
	HAL_Delay(period);

	int8_t i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
	int8_t rslt = 0; 	//return 0 for success, non-zero for failure
	HAL_I2C_Mem_Read(&hi2c1, dev_id, reg_addr, 1, reg_data, len, 100);
	return rslt;
}

int8_t i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
	int8_t rslt = 0; 	//return 0 for success, non-zero for failure
	HAL_I2C_Mem_Write(&hi2c1, dev_id, reg_addr, 1, reg_data, len, 100);
	return rslt;
}

}
