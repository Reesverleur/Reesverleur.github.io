/**
  ******************************************************************************
  * @file    coms.h
  * @author  Rees P. Verleur
  * @brief   This file contains the class definition for the bluetooth
  * 			communication occuring over UART
  *
  ******************************************************************************
  * This contains the class and attribute definitions for the coms class. It
  * also includes function prototypes for the methods in the class. The coms
  * class includes 3 main methods these methods allow for syncing of the
  * protocol to a python program, a data request from the python program, and
  * a decoding method to convert the character data into usable types.
  *
  * The file also contains the function prototypes for the UART Tx and Rx
  * callback functions. These functions are defined in the same file for
  * convenience as this will be the only class to make use of them during the
  * robots operation.
  *
  ******************************************************************************
  */
#ifndef SRC_COMS_H_
#define SRC_COMS_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <numeric>
/**
  ******************************************************************************
  * @brief   Class definition for a bluetooth communication protocol
  *
  ******************************************************************************
  * This contains the attribute definitions and the function prototypes for the
  * coms class. The com port will operate using a custom standard to transmit
  * the required data as compactly as possible.
  *
  * After a begin and acknowledge byte the class will allow for the robot to
  * request data from the python code. The python program will then transmit the
  * data in the set format "R###T####00" This will correspond to a series of
  * movements the robot must make, first a rotation by some number of degrees,
  * then a  translation (forward move) by some amount of distance. These
  * quantities can be easily converted to motor angular positions by the system
  * using basic geometry.
  *
  ******************************************************************************
  */
class coms{
	private:
		int32_t 					cpr;				//!< Integer defining the cpr of the motor encoder
		float  					 	gear;				//!< Float defining gearing ratio of motor
		float   					wheelrad;			//!< Float defining wheel radius
		float						wheeldist;			//!< Float defining distance between wheels
	public:
		static UART_HandleTypeDef* 	uart;				//!< Pointer to UART object
		char 						start[5];			//!< start message to transmit to python program
		char						req[4];				//!< byte request message to transmit to python program
		static char					messbuff[10];		//!< data buffer to receive data into from python program
		int32_t						rotate;				//!< integer for degrees to rotate
		int32_t						translate;			//!< integer for amount to move

	coms::coms(UART_HandleTypeDef* huart);
	void coms::begin();
	void coms::request();
	void coms::decode();
};




void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);

#endif /* SRC_COMS_H_ */
