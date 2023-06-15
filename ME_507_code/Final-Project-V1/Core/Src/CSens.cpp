/**
  ******************************************************************************
  * @file    CSens.cpp
  * @author  Rees P. Verleur
  * @brief   This file contains the methods for the CSens class
  *
  ******************************************************************************
  * This contains the method definitions for the CSens class. It also includes
  * the I2C memory read callback function definition
  *
  ******************************************************************************
  */
#include "CSens.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>


/**
  ******************************************************************************
  * @brief   Constructor for the CSens class
  *
  ******************************************************************************
  * This creates a CSens object connected to the I2C port specified
  * @param	hi2c	This is the pointer to the I2C port connected to the Color
  * 				sensor
  *
  ******************************************************************************
  */
CSens::CSens(I2C_HandleTypeDef* hi2c){
	i2c = hi2c;
	HAL_I2C_Mem_Write(i2c,Address,0x00,I2C_MEMADD_SIZE_8BIT,Init,1,1000);
	HAL_Delay(5);
}

/**
  ******************************************************************************
  * @brief   Method to get new color data from the sensor
  *
  ******************************************************************************
  * This retrieves the data from the color sensor by using the HAL
  * implementation of the I2C communication protocol.
  *
  ******************************************************************************
  */
void CSens::getCdata(){
	HAL_I2C_Mem_Read_IT(i2c,Address,MemAddr,I2C_MEMADD_SIZE_8BIT,Cdata,Length);
}

/**
  ******************************************************************************
  * @brief   Callback function for I2C memory read
  *
  ******************************************************************************
  * This is called when the memory read operation is complete. It processes the
  * read data into the appropriate attributes.
  *
  * @param	hi2c	A pointer to the I2C object which triggered the interrupt
  ******************************************************************************
  */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef* hi2c){				// Memory read callback function
		if(hi2c == CSens::i2c){
			CSens::Clr_Val = (CSens::Cdata[20]<<8)+CSens::Cdata[19];	// Clear data is little endian
			CSens::Red_Val = (CSens::Cdata[21]<<8)+CSens::Cdata[22];	// RGB data is big endian
			CSens::Grn_Val = (CSens::Cdata[23]<<8)+CSens::Cdata[24];
			CSens::Blu_Val = (CSens::Cdata[25]<<8)+CSens::Cdata[26];
		}
}

