/**
  ******************************************************************************
  * @file    CSens.h
  * @author  Rees P. Verleur
  * @brief   This file contains the class definition for the Color Sensor Driver
  *
  ******************************************************************************
  * This contains the class and attribute definitions for the CSens class. It
  * also includes function prototypes for the methods in the class and function
  * prototypes for the I2C Memory Rx callback function which is required for the
  * operation of of the code.
  *
  ******************************************************************************
  */

#ifndef SRC_CSENS_H_
#define SRC_CSENS_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/**
  ******************************************************************************
  * @brief   Class definition for an I2C color Sensor Driver
  *
  ******************************************************************************
  * This contains the attribute definitions and the function prototypes for the
  * I2C based color sensor driver. The color sensor gives 16 bit numbers for
  * each of 4 channels (clear, red, green, blue). The class accesses the
  * appropriate locations in the memory of the I2C device and sets class
  * attributes accordingly.
  *
  * The communication protocol for this class does not appear to match the
  * standard I2C protocol. It appears that memory can only be accessed starting
  * from byte 0. Additionally the memory read command must request data starting
  * from byte 3 in order for the last 8 bytes (which contain the color data) to
  * be populated.
  *
  * The color scales also appear to be very non-linear with the red channel
  * having a much higher response than the blue and green.
  *
  ******************************************************************************
  */
class CSens{

	private:								// Private variables
		int16_t Address = 0x0029 << 1;		//!< I2C Address of color sensor
		int16_t MemAddr = 0x0003;			//!< Memory address to request data from
		int16_t Length	= 27;				//!< Number of bytes to fetch
		uint8_t Init[1]	= {0b00000011};		//!< Initialization byte to be written to 0x00

	public:									// Public variables
		static uint8_t Cdata[27];			//!< Data collected from device
		static I2C_HandleTypeDef* i2c;		//!< Pointer to i2c object
		static uint16_t Clr_Val;			//!< 16 bit color value for clear channel
		static uint16_t Red_Val;			//!< 16 bit color value for red channel
		static uint16_t Grn_Val;			//!< 16 bit color value for green channel
		static uint16_t Blu_Val;			//!< 16 bit color value for blue channel

		CSens(I2C_HandleTypeDef* hi2c);		// Constructor

		void getCdata();					// Function to get new color data from sensor
};

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef* hi2c);	//i2c memory read callback function

#endif /* SRC_CSENS_H_ */
