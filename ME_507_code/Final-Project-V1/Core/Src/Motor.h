/**
  ******************************************************************************
  * @file    Motor.h
  * @author  Rees P. Verleur
  * @brief   This file contains the class definition for the Motor Driver
  *
  ******************************************************************************
  * This contains the class and attribute definitions for the Motor class. It
  * also includes function prototypes for the methods in the class. The motor
  * driver includes 3 main methods along with some setter and getter methods
  * these methods allow for the setting of a duty cycle, the implementation of
  * a classic PID loop, and the implementation of a modified PID loop which
  * starts the integral gain late to prevent overshoot
  *
  ******************************************************************************
  */
#ifndef SRC_MOTOR_H_
#define SRC_MOTOR_H_
// Includes
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <numeric>
/**
  ******************************************************************************
  * @brief   Class definition for a motor driver/controller
  *
  ******************************************************************************
  * This contains the attribute definitions and the function prototypes for the
  * motor driver. The motors take a PWM signal from two channels representing
  * the forward and reverse channels. The encoders return two square waves which
  * are 90 degrees out of phase to allow for a full quadrature setup.
  *
  * In addition to the method allowing for setting a static duty cycle, the
  * class has two additional methods which when called on every pass through a
  * loop (no more than 10 ms between calls) allow for closed loop position
  * control. There are also methods to change the gains, to zero out the errors
  * and to enable/disable the motors.
  *
  ******************************************************************************
  */
class Motor
{
	public:
	TIM_HandleTypeDef* 	tim;				//!< Pointer to timer object controlling PWM signal for motor drive pins
	TIM_HandleTypeDef* 	entim;				//!< Pointer to timer object interpreting quadrature encoder feedback
	int32_t				FwdCh;				//!< Integer representing Forward channel of drive timer
	int32_t				RevCh;				//!< Integer representing Reverse channel of drive timer
	int32_t				En;					//!< Enable value for motor object
	int32_t				Duty;				//!< Current Duty Cycle for motor object
	int32_t				Pos;				//!< Current Position Value for motor object
	float				Kp;					//!< Proportional gain for control loop
	float				Ki;					//!< Integral gain for control loop
	float				Kd;					//!< Derivative gain for control loop
	int32_t				setpoint;			//!< Set Point for control loop
	int32_t				error;				//!< Current position error from set point
	int32_t				esum;				//!< integral of past error from set point
	int32_t				derror;				//!< derivative of past error from set point
	int16_t				oldticks;			//!< last timer count value
	int16_t 			ticks;				//!< timer count value


	Motor(TIM_HandleTypeDef* htim,TIM_HandleTypeDef* htim2,int32_t Ch1,int32_t Ch2);
	void Enable(int32_t newEn);
	void duty(int32_t newDuty);
	void Control(int32_t newSet);
	void setGains(float nKp,float nKi, float nKd);
	void zeroErrors();
	void VariableControl(int32_t newSet, float nKp, float nKi, int32_t intError);
};




#endif /* SRC_MOTOR_H_ */
