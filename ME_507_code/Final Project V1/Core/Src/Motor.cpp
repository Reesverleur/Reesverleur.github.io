/**
  ******************************************************************************
  * @file    Motor.cpp
  * @author  Rees P. Verleur
  * @brief   This file contains the methods for the Motor class
  *
  ******************************************************************************
  * This contains the method definitions for the Motor class. These methods
  * allow for setting of important values, as well as direct control over
  * motor speed and a closed loop controller for position.
  ******************************************************************************
  */
// Includes
#include "Motor.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <numeric>

/**
  ******************************************************************************
  * @brief   Constructor for the Motor class
  *
  ******************************************************************************
  * This creates a Motor object connected to the timers specified
  *
  * @param	htim1	This is the pointer to the timer channel controlling the
  * 				PWM drive signals to the motor
  * @param  htim2	This is the pointer to the timer channel running in encoder
  * 				mode to measure the position of the motor
  * @param  Ch1		This is an integer representing the channel of the PWM timer
  * 				connected to the forward direction of the motor
  * @param  Ch2		This is an integer representing the channel of the PWM timer
  * 				connected to the reverse direction of the motor
  *
  ******************************************************************************
  */
// Constructor only requires a few of the class attributes
	Motor::Motor(TIM_HandleTypeDef* htim1,TIM_HandleTypeDef* htim2,int32_t Ch1,int32_t Ch2){
		tim	  	  = htim1;
		entim	  = htim2;
		FwdCh	  = Ch1;
		RevCh     = Ch2;
		En		  = 0;
		Duty	  = 0;
		Pos		  = 0;
		Kp		  = 0;
		Ki 		  = 0;
		Kd		  = 0;
		setpoint  = 0;
		error	  = 0;
		esum	  = 0;
		derror	  = 0;
		oldticks  = 0;
		ticks	  = 0;
	}

/**
  ******************************************************************************
  * @brief   Method to enable/disable motor
  *
  ******************************************************************************
  * This enables/disables the motor using a pseudo-boolean
  *
  * @param 	newEn	This is a pseudo-boolean controlling the enable status of
  * 				the motors. A value of 1 is enables the motors, a value of 0
  * 				disables the motors
  *
  ******************************************************************************
  */
// Enables/Disables the motor
	void Motor::Enable(int32_t newEn){
		En = newEn;
	}

/**
  ******************************************************************************
  * @brief   Method to set duty cycle of motor
  *
  ******************************************************************************
  * This sets the duty cycle of the PWM channels controlling the motor
  *
  * @param 	newDuty	This is the new desired duty cycle as a percent(-100 to 100)
  ******************************************************************************
  */
// Sets duty cycle based off of percentage value
	void Motor::duty(int32_t newDuty){
		Duty = newDuty;
		if(Duty>=0){
			__HAL_TIM_SET_COMPARE(tim,FwdCh,En*Duty*48);
			__HAL_TIM_SET_COMPARE(tim,RevCh,0);
		}else if(Duty<0){
			__HAL_TIM_SET_COMPARE(tim,RevCh,En*-1*Duty*48);
			__HAL_TIM_SET_COMPARE(tim,FwdCh,0);
		}
	}

/**
  ******************************************************************************
  * @brief   Method to implement a PID control loop for the motor object
  *
  ******************************************************************************
  * This implements a PID control loop for the object where the duty cycle is
  * updated each time this function is called. For stability, this should not be
  * less often than once every 20 ms.
  *
  * @param 	newSet	This is the new desired set point for the motor controller
  ******************************************************************************
  */
	void Motor::Control(int32_t newSet){
		setpoint= newSet;
		ticks = __HAL_TIM_GET_COUNTER(entim)-oldticks;
		if (ticks>32768){
			ticks =ticks-65535;
		}else if (ticks<-32768){
			ticks =ticks+65535;
		}
		Pos = Pos+ticks-oldticks;
		oldticks = ticks;

		derror = (setpoint-Pos)-error;
		error = error+derror;
		esum = esum+error;

		Duty = (Kp*error+Ki*esum+Kd*derror);
		if(Duty>4800){
			Duty = 4800;
		}else if(Duty<-4800){
			Duty = -4800;
		}

		if(Duty>=0){
			__HAL_TIM_SET_COMPARE(tim,FwdCh,En*Duty);
			__HAL_TIM_SET_COMPARE(tim,RevCh,0);
		}else if(Duty<0){
			__HAL_TIM_SET_COMPARE(tim,RevCh,En*-1*Duty);
			__HAL_TIM_SET_COMPARE(tim,FwdCh,0);
		}

	}

/**
  ******************************************************************************
  * @brief   Method to change PID gains for control loop
  *
  ******************************************************************************
  * This allows for the setting of the Proportional, Integral, and Derivative
  * gains for the control loop
  *
  * @param 	nKp	This is the new Proportional Gain value
  *
  * @param  nKi	This is the new Integral Gain value
  *
  * @param  nKd	This is the new Derivative Gain value
  ******************************************************************************
  */
	void Motor::setGains(float nKp,float nKi, float nKd){
		Kp = nKp;
		Ki = nKi;
		Kd = nKd;
	}
/**
  ******************************************************************************
  * @brief   Method to zero errors for control loop
  *
  ******************************************************************************
  * This allows for the zeroing of all error values when setting a new setpoint
  *
  ******************************************************************************
  */
	void Motor::zeroErrors(){
		error = 0;
		derror = 0;
		esum = 0;
	}

/**
  ******************************************************************************
  * @brief   Method to implement modified PI control loop
  *
  ******************************************************************************
  * This implements a modified PI control loop where the Integral gain can be
  * enabled after the error is below a set threshold. This allows for better
  * control for large set point values. As with the standard PID loop, this must
  * be called each time it is desired to update the actuation value.
  *
  * @param newSet	This is the new set point for the system
  *
  * @param nKp		This is the proportional gain to be implemented immediately
  *
  * @param nKi		This is the integral gain to be implemented after error falls
  * 				below a set threshold
  *
  * @param intError	This is the integral error threshold after which integration
  * 				of the error will begin.
  *
  ******************************************************************************
  */
	void Motor::VariableControl(int32_t newSet, float nKp, float nKi, int32_t intError){
		Kp = nKp;
		Ki = nKi;
		//Kd = 0;
		setpoint= newSet;
		ticks = __HAL_TIM_GET_COUNTER(entim)-oldticks;
		if (ticks>32768){
			ticks =ticks-65535;
		}else if (ticks<-32768){
			ticks =ticks+65535;
		}
		Pos = Pos+ticks-oldticks;
		oldticks = ticks;

		derror = (setpoint-Pos)-error;
		error = error+derror;
		if(error <= intError){
			esum = esum+error;
		}else{
			esum = 0;
		}


		Duty = (Kp*error+Ki*esum+Kd*derror);
		if(Duty>4800){
			Duty = 4800;
		}else if(Duty<-4800){
			Duty = -4800;
		}

		if(Duty>=0){
			__HAL_TIM_SET_COMPARE(tim,FwdCh,En*Duty);
			__HAL_TIM_SET_COMPARE(tim,RevCh,0);
		}else if(Duty<0){
			__HAL_TIM_SET_COMPARE(tim,RevCh,En*-1*Duty);
			__HAL_TIM_SET_COMPARE(tim,FwdCh,0);
		}

	}





