/*
 * Copyright (C) 2010-2013 Motorola Mobility LLC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307, USA
 */

#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/export.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/input-polldev.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/poll.h>
#include <linux/regulator/consumer.h>
#include <linux/slab.h>
#include <linux/switch.h>
#include <linux/time.h>
#include <linux/uaccess.h>
#include <linux/wakelock.h>
#include <linux/workqueue.h>

#include <linux/stm401.h>


<<<<<<< HEAD
int stm401_load_brightness_table(struct stm401_data *ps_stm401,
		unsigned char *cmdbuff)
{
	int err = -ENOTTY;
	int index = 0;
	cmdbuff[0] = LUX_TABLE_VALUES;
	for (index = 0; index < LIGHTING_TABLE_SIZE; index++) {
		cmdbuff[(2 * index) + 1]
			= ps_stm401->pdata->lux_table[index] >> 8;
		cmdbuff[(2 * index) + 2]
			= ps_stm401->pdata->lux_table[index] & 0xFF;
	}
	err = stm401_i2c_write_no_reset(ps_stm401, cmdbuff,
=======
int stm401_load_brightness_table(struct stm401_data *ps_stm401)
{
	int err = -ENOTTY;
	int index = 0;
	stm401_cmdbuff[0] = LUX_TABLE_VALUES;
	for (index = 0; index < LIGHTING_TABLE_SIZE; index++) {
		stm401_cmdbuff[(2 * index) + 1]
			= ps_stm401->pdata->lux_table[index] >> 8;
		stm401_cmdbuff[(2 * index) + 2]
			= ps_stm401->pdata->lux_table[index] & 0xFF;
	}
	err = stm401_i2c_write_no_reset(ps_stm401, stm401_cmdbuff,
>>>>>>> f674d0881c3ecec6016d7aa8b91132f1d40432d4
				(2 * LIGHTING_TABLE_SIZE) + 1);
	if (err)
		return err;

<<<<<<< HEAD
	cmdbuff[0] = BRIGHTNESS_TABLE_VALUES;
	for (index = 0; index < LIGHTING_TABLE_SIZE; index++) {
		cmdbuff[index + 1]
				= ps_stm401->pdata->brightness_table[index];
	}
	err = stm401_i2c_write_no_reset(ps_stm401, cmdbuff,
=======
	stm401_cmdbuff[0] = BRIGHTNESS_TABLE_VALUES;
	for (index = 0; index < LIGHTING_TABLE_SIZE; index++) {
		stm401_cmdbuff[index + 1]
				= ps_stm401->pdata->brightness_table[index];
	}
	err = stm401_i2c_write_no_reset(ps_stm401, stm401_cmdbuff,
>>>>>>> f674d0881c3ecec6016d7aa8b91132f1d40432d4
			LIGHTING_TABLE_SIZE + 1);
	dev_dbg(&ps_stm401->client->dev, "Brightness tables loaded\n");
	return err;
}

<<<<<<< HEAD
void stm401_reset(struct stm401_platform_data *pdata, unsigned char *cmdbuff)
=======
void stm401_reset(struct stm401_platform_data *pdata)
>>>>>>> f674d0881c3ecec6016d7aa8b91132f1d40432d4
{
	dev_err(&stm401_misc_data->client->dev, "stm401_reset\n");
	msleep(stm401_i2c_retry_delay);
	gpio_set_value(pdata->gpio_reset, 0);
<<<<<<< HEAD
	msleep(STM401_RESET_DELAY);
	gpio_set_value(pdata->gpio_reset, 1);
	msleep(STM401_RESET_DELAY);
	stm401_detect_lowpower_mode(cmdbuff);

	if (!stm401_misc_data->in_reset_and_init) {
		/* sending reset to slpc hal */
		stm401_ms_data_buffer_write(stm401_misc_data, DT_RESET,
			NULL, 0);
	}
=======
	msleep(stm401_i2c_retry_delay);
	gpio_set_value(pdata->gpio_reset, 1);
	msleep(STM401_RESET_DELAY);
>>>>>>> f674d0881c3ecec6016d7aa8b91132f1d40432d4
}

int stm401_reset_and_init(void)
{
	struct stm401_platform_data *pdata;
	struct timespec current_time;
<<<<<<< HEAD
	unsigned int i;
	int err = 0, ret_err = 0;
	int mutex_locked = 0;
	int reset_attempts = 0;
	unsigned char *rst_cmdbuff = kmalloc(512, GFP_KERNEL);
	dev_dbg(&stm401_misc_data->client->dev, "stm401_reset_and_init\n");

	if (rst_cmdbuff == NULL)
		return -ENOMEM;

	wake_lock(&stm401_misc_data->reset_wakelock);

	stm401_misc_data->in_reset_and_init = true;

	pdata = stm401_misc_data->pdata;

	stm401_wake(stm401_misc_data);

	do {
		stm401_reset(pdata, rst_cmdbuff);

		/* check for sign of life */
		rst_cmdbuff[0] = REV_ID;
		err = stm401_i2c_write_read_no_reset(stm401_misc_data,
			rst_cmdbuff, 1, 1);
		if (err < 0)
			dev_err(&stm401_misc_data->client->dev, "stm401 not responding after reset (%d)",
				reset_attempts);
	} while (reset_attempts++ < 5 && err < 0);

	stm401_i2c_retry_delay = 200;

	rst_cmdbuff[0] = ACCEL_UPDATE_RATE;
	rst_cmdbuff[1] = stm401_g_acc_delay;
	err = stm401_i2c_write_no_reset(stm401_misc_data, rst_cmdbuff, 2);
	if (err < 0)
		ret_err = err;

	stm401_i2c_retry_delay = 13;

	rst_cmdbuff[0] = MAG_UPDATE_RATE;
	rst_cmdbuff[1] = stm401_g_mag_delay;
	err = stm401_i2c_write_no_reset(stm401_misc_data, rst_cmdbuff, 2);
	if (err < 0)
		ret_err = err;

	rst_cmdbuff[0] = GYRO_UPDATE_RATE;
	rst_cmdbuff[1] = stm401_g_gyro_delay;
	err = stm401_i2c_write_no_reset(stm401_misc_data, rst_cmdbuff, 2);
	if (err < 0)
		ret_err = err;

	rst_cmdbuff[0] = PRESSURE_UPDATE_RATE;
	rst_cmdbuff[1] = stm401_g_baro_delay;
	err = stm401_i2c_write_no_reset(stm401_misc_data, rst_cmdbuff, 2);
	if (err < 0)
		ret_err = err;

	rst_cmdbuff[0] = IR_GESTURE_RATE;
	rst_cmdbuff[1] = stm401_g_ir_gesture_delay;
	err = stm401_i2c_write_no_reset(stm401_misc_data, rst_cmdbuff, 2);
	if (err < 0)
		ret_err = err;

	rst_cmdbuff[0] = IR_RAW_RATE;
	rst_cmdbuff[1] = stm401_g_ir_raw_delay;
	err = stm401_i2c_write_no_reset(stm401_misc_data, rst_cmdbuff, 2);
	if (err < 0)
		ret_err = err;

	rst_cmdbuff[0] = NONWAKESENSOR_CONFIG;
	rst_cmdbuff[1] = stm401_g_nonwake_sensor_state & 0xFF;
	rst_cmdbuff[2] = (stm401_g_nonwake_sensor_state >> 8) & 0xFF;
	rst_cmdbuff[3] = stm401_g_nonwake_sensor_state >> 16;
	err = stm401_i2c_write_no_reset(stm401_misc_data, rst_cmdbuff, 4);
	if (err < 0)
		ret_err = err;

	rst_cmdbuff[0] = WAKESENSOR_CONFIG;
	rst_cmdbuff[1] = stm401_g_wake_sensor_state & 0xFF;
	rst_cmdbuff[2] = stm401_g_wake_sensor_state >> 8;
	err = stm401_i2c_write_no_reset(stm401_misc_data, rst_cmdbuff, 3);
	if (err < 0)
		ret_err = err;

	rst_cmdbuff[0] = ALGO_CONFIG;
	rst_cmdbuff[1] = stm401_g_algo_state & 0xFF;
	rst_cmdbuff[2] = stm401_g_algo_state >> 8;
	err = stm401_i2c_write_no_reset(stm401_misc_data, rst_cmdbuff, 3);
	if (err < 0)
		ret_err = err;

	rst_cmdbuff[0] = MOTION_DUR;
	rst_cmdbuff[1] = stm401_g_motion_dur;
	err = stm401_i2c_write_no_reset(stm401_misc_data, rst_cmdbuff, 2);
	if (err < 0)
		ret_err = err;

	rst_cmdbuff[0] = ZRMOTION_DUR;
	rst_cmdbuff[1] = stm401_g_zmotion_dur;
	err = stm401_i2c_write_no_reset(stm401_misc_data, rst_cmdbuff, 2);
=======
	int stm401_req_gpio;
	int stm401_req_value;
	unsigned int i;
	int err, ret_err = 0;

	if (stm401_misc_data->is_suspended)
		return ret_err;

	pdata = stm401_misc_data->pdata;

	if (stm401_misc_data->ap_stm401_handoff_ctrl) {
		stm401_req_gpio = pdata->gpio_mipi_req;
		stm401_req_value = gpio_get_value(stm401_req_gpio);
		if (stm401_req_value)
			gpio_set_value(stm401_req_gpio, 0);
	} else {
		stm401_req_value = 0;
	}

	stm401_reset(pdata);
	stm401_i2c_retry_delay = 200;

	stm401_cmdbuff[0] = ACCEL_UPDATE_RATE;
	stm401_cmdbuff[1] = stm401_g_acc_delay;
	err = stm401_i2c_write_no_reset(stm401_misc_data, stm401_cmdbuff, 2);
	if (err < 0)
		ret_err = err;

	stm401_i2c_retry_delay = 10;

	stm401_cmdbuff[0] = MAG_UPDATE_RATE;
	stm401_cmdbuff[1] = stm401_g_mag_delay;
	err = stm401_i2c_write_no_reset(stm401_misc_data, stm401_cmdbuff, 2);
	if (err < 0)
		ret_err = err;

	stm401_cmdbuff[0] = GYRO_UPDATE_RATE;
	stm401_cmdbuff[1] = stm401_g_gyro_delay;
	err = stm401_i2c_write_no_reset(stm401_misc_data, stm401_cmdbuff, 2);
	if (err < 0)
		ret_err = err;

	stm401_cmdbuff[0] = PRESSURE_UPDATE_RATE;
	stm401_cmdbuff[1] = stm401_g_baro_delay;
	err = stm401_i2c_write_no_reset(stm401_misc_data, stm401_cmdbuff, 2);
	if (err < 0)
		ret_err = err;

	stm401_cmdbuff[0] = STEP_COUNTER_UPDATE_RATE;
	stm401_cmdbuff[1] = (stm401_g_step_counter_delay>>8);
	stm401_cmdbuff[2] = stm401_g_step_counter_delay;
	err = stm401_i2c_write_no_reset(stm401_misc_data, stm401_cmdbuff, 3);
	if (err < 0)
		ret_err = err;

	stm401_cmdbuff[0] = NONWAKESENSOR_CONFIG;
	stm401_cmdbuff[1] = stm401_g_nonwake_sensor_state & 0xFF;
	stm401_cmdbuff[2] = (stm401_g_nonwake_sensor_state >> 8) & 0xFF;
	stm401_cmdbuff[3] = stm401_g_nonwake_sensor_state >> 16;
	err = stm401_i2c_write_no_reset(stm401_misc_data, stm401_cmdbuff, 4);
	if (err < 0)
		ret_err = err;

	stm401_cmdbuff[0] = WAKESENSOR_CONFIG;
	stm401_cmdbuff[1] = stm401_g_wake_sensor_state & 0xFF;
	stm401_cmdbuff[2] = stm401_g_wake_sensor_state >> 8;
	err = stm401_i2c_write_no_reset(stm401_misc_data, stm401_cmdbuff, 3);
	if (err < 0)
		ret_err = err;

	stm401_cmdbuff[0] = ALGO_CONFIG;
	stm401_cmdbuff[1] = stm401_g_algo_state & 0xFF;
	stm401_cmdbuff[2] = stm401_g_algo_state >> 8;
	err = stm401_i2c_write_no_reset(stm401_misc_data, stm401_cmdbuff, 3);
	if (err < 0)
		ret_err = err;

	stm401_cmdbuff[0] = MOTION_DUR;
	stm401_cmdbuff[1] = stm401_g_motion_dur;
	err = stm401_i2c_write_no_reset(stm401_misc_data, stm401_cmdbuff, 2);
	if (err < 0)
		ret_err = err;

	stm401_cmdbuff[0] = ZRMOTION_DUR;
	stm401_cmdbuff[1] = stm401_g_zmotion_dur;
	err = stm401_i2c_write_no_reset(stm401_misc_data, stm401_cmdbuff, 2);
>>>>>>> f674d0881c3ecec6016d7aa8b91132f1d40432d4
	if (err < 0)
		ret_err = err;

	for (i = 0; i < STM401_NUM_ALGOS; i++) {
		if (stm401_g_algo_requst[i].size > 0) {
<<<<<<< HEAD
			rst_cmdbuff[0] = stm401_algo_info[i].req_register;
			memcpy(&rst_cmdbuff[1],
				stm401_g_algo_requst[i].data,
				stm401_g_algo_requst[i].size);
			err = stm401_i2c_write_no_reset(stm401_misc_data,
				rst_cmdbuff,
				stm401_g_algo_requst[i].size + 1);
			if (err < 0)
				ret_err = err;
		}
	}
	rst_cmdbuff[0] = INTERRUPT_STATUS;
	stm401_i2c_write_read_no_reset(stm401_misc_data, rst_cmdbuff, 1, 3);
	rst_cmdbuff[0] = WAKESENSOR_STATUS;
	stm401_i2c_write_read_no_reset(stm401_misc_data, rst_cmdbuff, 1, 2);

	rst_cmdbuff[0] = PROX_SETTINGS;
	rst_cmdbuff[1]
		= (pdata->ct406_detect_threshold >> 8) & 0xff;
	rst_cmdbuff[2]
		= pdata->ct406_detect_threshold & 0xff;
	rst_cmdbuff[3] = (pdata->ct406_undetect_threshold >> 8) & 0xff;
	rst_cmdbuff[4] = pdata->ct406_undetect_threshold & 0xff;
	rst_cmdbuff[5]
		= (pdata->ct406_recalibrate_threshold >> 8) & 0xff;
	rst_cmdbuff[6] = pdata->ct406_recalibrate_threshold & 0xff;
	rst_cmdbuff[7] = pdata->ct406_pulse_count & 0xff;
	err = stm401_i2c_write_no_reset(stm401_misc_data, rst_cmdbuff, 8);
=======
			stm401_cmdbuff[0] = stm401_algo_info[i].req_register;
			memcpy(&stm401_cmdbuff[1],
				stm401_g_algo_requst[i].data,
				stm401_g_algo_requst[i].size);
			err = stm401_i2c_write_no_reset(stm401_misc_data,
				stm401_cmdbuff,
				stm401_g_algo_requst[i].size + 1);
		}
	}
	stm401_cmdbuff[0] = INTERRUPT_STATUS;
	stm401_i2c_write_read_no_reset(stm401_misc_data, stm401_cmdbuff, 1, 2);
	stm401_cmdbuff[0] = WAKESENSOR_STATUS;
	stm401_i2c_write_read_no_reset(stm401_misc_data, stm401_cmdbuff, 1, 2);

	stm401_cmdbuff[0] = PROX_SETTINGS;
	stm401_cmdbuff[1]
		= (pdata->ct406_detect_threshold >> 8) & 0xff;
	stm401_cmdbuff[2]
		= pdata->ct406_detect_threshold & 0xff;
	stm401_cmdbuff[3] = (pdata->ct406_undetect_threshold >> 8) & 0xff;
	stm401_cmdbuff[4] = pdata->ct406_undetect_threshold & 0xff;
	stm401_cmdbuff[5]
		= (pdata->ct406_recalibrate_threshold >> 8) & 0xff;
	stm401_cmdbuff[6] = pdata->ct406_recalibrate_threshold & 0xff;
	stm401_cmdbuff[7] = pdata->ct406_pulse_count & 0xff;
	err = stm401_i2c_write_no_reset(stm401_misc_data, stm401_cmdbuff, 8);
>>>>>>> f674d0881c3ecec6016d7aa8b91132f1d40432d4
	if (err < 0) {
		dev_err(&stm401_misc_data->client->dev,
			"unable to write proximity settings %d\n", err);
		ret_err = err;
	}

<<<<<<< HEAD
	err = stm401_load_brightness_table(stm401_misc_data, rst_cmdbuff);
	if (err < 0)
		ret_err = err;

	getnstimeofday(&current_time);
	current_time.tv_sec += stm401_time_delta;

	rst_cmdbuff[0] = AP_POSIX_TIME;
	rst_cmdbuff[1] = (unsigned char)(current_time.tv_sec >> 24);
	rst_cmdbuff[2] = (unsigned char)((current_time.tv_sec >> 16)
		& 0xff);
	rst_cmdbuff[3] = (unsigned char)((current_time.tv_sec >> 8)
		& 0xff);
	rst_cmdbuff[4] = (unsigned char)((current_time.tv_sec)
		& 0xff);
	err = stm401_i2c_write_no_reset(stm401_misc_data,
					rst_cmdbuff, 5);
	if (err < 0)
		ret_err = err;

	rst_cmdbuff[0] = MAG_CAL;
	memcpy(&rst_cmdbuff[1], stm401_g_mag_cal, STM401_MAG_CAL_SIZE);
	err = stm401_i2c_write_no_reset(stm401_misc_data, rst_cmdbuff,
=======
	err = stm401_load_brightness_table(stm401_misc_data);
	if (err < 0)
		ret_err = err;

	if (stm401_req_value) {
		getnstimeofday(&current_time);
		current_time.tv_sec += stm401_time_delta;

		stm401_cmdbuff[0] = AP_POSIX_TIME;
		stm401_cmdbuff[1] = (unsigned char)(current_time.tv_sec >> 24);
		stm401_cmdbuff[2] = (unsigned char)((current_time.tv_sec >> 16)
			& 0xff);
		stm401_cmdbuff[3] = (unsigned char)((current_time.tv_sec >> 8)
			& 0xff);
		stm401_cmdbuff[4] = (unsigned char)((current_time.tv_sec)
			& 0xff);
		err = stm401_i2c_write_no_reset(stm401_misc_data,
						stm401_cmdbuff, 5);
		if (err < 0)
			ret_err = err;

		if (stm401_g_control_reg_restore) {
			stm401_cmdbuff[0] = STM401_CONTROL_REG;
			memcpy(&stm401_cmdbuff[1], stm401_g_control_reg,
				STM401_CONTROL_REG_SIZE);
			err = stm401_i2c_write_no_reset(stm401_misc_data,
				stm401_cmdbuff,
				STM401_CONTROL_REG_SIZE);
			if (err < 0)
				ret_err = err;
		}

		gpio_set_value(stm401_req_gpio, 1);
	}

	stm401_cmdbuff[0] = MAG_CAL;
	memcpy(&stm401_cmdbuff[1], stm401_g_mag_cal, STM401_MAG_CAL_SIZE);
	err = stm401_i2c_write_no_reset(stm401_misc_data, stm401_cmdbuff,
>>>>>>> f674d0881c3ecec6016d7aa8b91132f1d40432d4
		STM401_MAG_CAL_SIZE);
	if (err < 0)
		ret_err = err;

	if (stm401_g_ir_config_reg_restore) {
<<<<<<< HEAD
		rst_cmdbuff[0] = IR_CONFIG;
		memcpy(&rst_cmdbuff[1], stm401_g_ir_config_reg,
			stm401_g_ir_config_reg[0]);
		err = stm401_i2c_write_no_reset(stm401_misc_data,
						rst_cmdbuff,
=======
		stm401_cmdbuff[0] = IR_CONFIG;
		memcpy(&stm401_cmdbuff[1], stm401_g_ir_config_reg,
			stm401_g_ir_config_reg[0]);
		err = stm401_i2c_write_no_reset(stm401_misc_data,
						stm401_cmdbuff,
>>>>>>> f674d0881c3ecec6016d7aa8b91132f1d40432d4
						stm401_g_ir_config_reg[0] + 1);
		if (err < 0)
			ret_err = err;
	}

	/* sending reset to slpc hal */
	stm401_ms_data_buffer_write(stm401_misc_data, DT_RESET,
		NULL, 0);

<<<<<<< HEAD
	mutex_locked = mutex_trylock(&stm401_misc_data->lock);
	stm401_quickpeek_reset_locked(stm401_misc_data);
	if (mutex_locked)
		mutex_unlock(&stm401_misc_data->lock);

	kfree(rst_cmdbuff);
	stm401_sleep(stm401_misc_data);
	stm401_misc_data->in_reset_and_init = false;
	wake_unlock(&stm401_misc_data->reset_wakelock);
=======
>>>>>>> f674d0881c3ecec6016d7aa8b91132f1d40432d4
	return ret_err;
}
