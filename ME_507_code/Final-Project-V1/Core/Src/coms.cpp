/**
  ******************************************************************************
  * @file    coms.cpp
  * @author  Rees P. Verleur
  * @brief   This file contains the methods for the coms class
  *
  ******************************************************************************
  * This contains the method definitions for the coms class. These methods allow
  * for synchronization with a companion python program and for standardized
  * data transfer between python running on a pc and this program running on
  * the STM32F411.
  ******************************************************************************
  */
#include "coms.h"

/**
  ******************************************************************************
  * @brief   Constructor for the coms class
  *
  ******************************************************************************
  * This creates a coms object connected to the bluetooth uart channel
  *
  * @param	huart	This is the pointer to the UART object connected to the
  * 				Bluetooth module
  *
  ******************************************************************************
  */
coms::coms(UART_HandleTypeDef* huart){
	uart = huart;
	start = "begin";
	req = "data";
	rotate = 0;
	translate = 0;

	cpr = 24;
	gear = 46.85;
	wheeldist = 150;
	wheelrad = 30;
}

/**
  ******************************************************************************
  * @brief   Begin method for bluetooth uart channel
  *
  ******************************************************************************
  * This sends a standard start message to sync up with the python program. This
  * allows for future communications to be correctly interpreted on both ends
  *
  *
  ******************************************************************************
  */
void coms::begin(){
	HAL_UART_Transmit_IT(uart,(uint8_t*) &start,5);

}

/**
  ******************************************************************************
  * @brief	Data request method for bluetooth uart channel
  *
  ******************************************************************************
  * This sends a standard data request message to the python program prompting
  * that program to fetch data being hosted on a web server and transmit it over
  * UART
  *
  *
  ******************************************************************************
  */
void coms::request(){
	HAL_UART_Transmit_IT(uart,(uint8_t*) &req,4);
}

/**
  ******************************************************************************
  * @brief	Data decoder for bluetooth uart channel
  *
  ******************************************************************************
  * This decodes the character data in messbuff into usable types.
  *
  *
  ******************************************************************************
  */
void coms::decode(){
	if(messbuff[0] == 'R' && messbuff[4] = 'T'){
		rotate = (messbuff[1]-'0')*100+(messbuff[2]-'0')*10+(messbuff[3]-'0')*1;
		translate = (messbuff[5]-'0')*1000+(messbuff[6]-'0')*100+(messbuff[7]-'0')*10+(messbuff[8]-'0')*1;

	}
}

/**
  ******************************************************************************
  * @brief	Callback function for uart transmit interrupts
  *
  ******************************************************************************
  * This runs when a uart transmit complete interrupt is triggered
  * @param	huart	This is the pointer to the uart object which triggered the
  * 				interrupt. This input is handled by the system.
  *
  *
  ******************************************************************************
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart){
	if(huart == coms::uart){
		HAL_UART_Receive_IT(coms::uart,(uint8_t*) &(coms::messbuff),10);
	}
}


/**
  ******************************************************************************
  * @brief	Callback function for uart receive interrupts
  *
  ******************************************************************************
  * This runs when a uart receive complete interrupt is triggered
  * @param	huart	This is the pointer to the uart object which triggered the
  * 				interrupt. This input is handled by the system.
  *
  *
  ******************************************************************************
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart){
	if(huart == coms::uart){
		if(coms::messbuff == "stop000000"){
			//Handle E-stop condition
		}
	}
}
