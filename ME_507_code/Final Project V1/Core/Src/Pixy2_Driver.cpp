
/**
  ******************************************************************************
  * @file    Pixy2_Driver.cpp
  * @author  Rees P. Verleur
  * @brief   This file contains the methods for the Pixy2 Class
  *
  ******************************************************************************
  * This contains the method definitions for the Pixy2 class. It also includes
  * the SPI Transmit and Receive callback function definitions
  *
  ******************************************************************************
  */
#include "Pixy2_Driver.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <numeric>

// Constructor only requires a few of the class attributes
/**
  ******************************************************************************
  * @brief   Constructor for the Pixy2 class
  *
  ******************************************************************************
  * This creates a Pixy2 object connected to the SPI port specified with the
  * cs pin as specified by the port and pin
  * @param	hspi	This is the pointer to the spi port connected to the Pixy2
  * @param	GPIOx	This is the pointer to the gpio port of the SPI CS pin
  * @param	GPIO_Pin	This is an integer representing the pin for the SPI CS
  * 					pin
  *
  ******************************************************************************
  */
	Pixy2::Pixy2(SPI_HandleTypeDef* hspi,GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin){
		spi	  	  = hspi;											// pointer to spi object
		SS_Port	  = GPIOx;											// pointer to ss port
		SS_Pin    = GPIO_Pin;										// pointer to ss pin

	}
// Updates block data for pixy2 object
/**
  ******************************************************************************
  * @brief   Method to get new block data from the camera
  *
  ******************************************************************************
  * This retrieves the data from the color sensor by sending a specific request
  * through the SPI protocol. It also interprets the result and matches it up
  * with the checksum value to be sure of fidelity
  * @param	sigmap	This is a bitmask representing the specific objects to
  * 				return data about
  * @param	maxblocks	This is an integer representing the maximum number of
  * 					objects to return data about objects are reported in
  * 					order of size with the largest first.
  *
  ******************************************************************************
  */
	void Pixy2::getBlock(uint8_t sigmap,uint8_t maxblocks){
		uint8_t blocReq[] = {0xae,0xc1,32,2,sigmap,maxblocks};		// block request data
		Length = 20;												// length of response
		HAL_GPIO_WritePin(SS_Port,SS_Pin,GPIO_PIN_RESET);			// drive ss pin low to enable communication
		HAL_SPI_Transmit_IT(spi,(uint8_t*) &blocReq,6);				// transmit block request return data is put in Data
		int16_t n;
		for(n=0;n<99;n++){											// iterate over all elements in Data
			if(Data[n] == 0xaf && Data[n+1] == 0xc1){				// check for first occurrence of  0xaec1
				StartByte = n;										// set as start byte
				int16_t csum = std::accumulate(Data+StartByte+6,Data+StartByte+20,0);
				if(csum == (Data[StartByte+5]<<8)+Data[StartByte+4]){
					memcpy(&Block,&Data[n],Length);					// copy the data to Block for stability
				}
				n = 100;											// exit out of for loop early
			}
		}

	}
// Sets coordinates from last captured block
/**
  ******************************************************************************
  * @brief   Method to set the coordinates of the next ball to collect
  *
  ******************************************************************************
  * Sets the coordinate values based on the last valid block read from the Pixy2
  *
  ******************************************************************************
  */
	void Pixy2::setCoords(){
		// Be sure there isn't an error state (0x0101)
		if(!(Block[8]==1 && Block[9]==1 && Block[10]==1 && Block[11]==1)){
			// If not, set coords from fetched block data
			Coords[0] = (Block[9]<<8)+Block[8];
			Coords[1] = (Block[11]<<8)+Block[10];
		} else{
			// otherwise return -1 for an error
			Coords[0] = -1;
			Coords[1] = -1;
		}
	}



/**
  ******************************************************************************
  * @brief   callback function for the SPI interrupt based transmit
  *
  ******************************************************************************
  * This function runs on the completion of the transmit operation for the SPI
  * components
  * @param	hspi	A pointer to the spi object which triggered the interrupt
  * 				this is passed into the function by the system.
  *
  ******************************************************************************
  */
	void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi){			// Transmit callback function
		if(hspi == Pixy2::spi){
			HAL_SPI_Receive_IT(hspi,(uint8_t*) &(Pixy2::Data),100);	// Start a receive command and put return data in Data
		}
	}
/**
  ******************************************************************************
  * @brief   callback function for the SPI interrupt based receive
  *
  ******************************************************************************
  * This function runs on the completion of the receive operation for the SPI
  * components
  * @param	hspi	A pointer to the spi object which triggered the interrupt
  * 				this is passed into the function by the system.
  *
  ******************************************************************************
  */
	void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi){			// Receive callback function
		if (hspi == Pixy2::spi){
			HAL_GPIO_WritePin(Pixy2::SS_Port,Pixy2::SS_Pin,GPIO_PIN_SET);	// Write ss pin high to disable communication
		}
	}
