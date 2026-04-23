/*
 * lib_motor_ui_enum.h
 *
 *  Created on: 2025/03/31
 *      Author: fryuk
 */

#ifndef LIB_MOTOR_UI_ENUM_H_
#define LIB_MOTOR_UI_ENUM_H_

typedef enum{
	ADJUST_PWR = 1,
	LOWER_LIMIT,
	ENC_P,
	ENC_I,
	ENC_D,
	CUR_P,
	CUR_I,
	CUR_D
}LibMotorParamEnum;

typedef enum{
	LPF_K = 48,
	LPF_T,
	PRINT
}LibMotorBordDataEnum;

typedef enum{
	ACTIVATE_LED1,
	ACTIVATE_LED2,
	ACTIVATE_LED3
}LibMotorBordFlagEnum;

typedef enum{
	ROTATE_SPEED = 9,
	ROTATE_PWR,
	BRAKE_PWR
}LibMotorMoveDataEnum;

typedef enum{
	ACTIVATE_CUR,
	ACTIVATE_ENC,
	ENC_DIRECTION
}LibMotorParamFlagEnum;


typedef enum{
	ACTIVATE_BRAKE = 3,
	ROTATE_WITH_BRAKE,
	ROTATE_DIRECTION
}LibMotorMoveDataFlagEnum;

typedef enum{
	M1,
	M2,
	M3,
	M4
}LibMotorNumEnum;



#endif /* LIB_MOTOR_UI_ENUM_H_ */
