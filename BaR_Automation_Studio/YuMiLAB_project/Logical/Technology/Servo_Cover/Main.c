// ************************************************************************************************************************************ //
// Title: Servo Cover Control
// Autor: Roman Parak
// Date : 8.5.2020
// Email: Roman.Parak@outlook.com
// ************************************************************************************************************************************ //

// ************************************************************************************************************************************ //
// -------------------------------------------------------------  LIBRARIES ----------------------------------------------------------- //
// ************************************************************************************************************************************ //
#include <bur/plctypes.h>
#include <standard.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

// ************************************************************************************************************************************ //
// -------------------------------------------------------------  DECLARATIONS -------------------------------------------------------- //
// ************************************************************************************************************************************ //
// TON_typ
_LOCAL TON_typ TON_01;
// TIME         
_LOCAL TIME preset_time;  
// INT
_LOCAL INT servo_position;
// UINT
_LOCAL UINT servo_period;
// USINT
_LOCAL USINT servo_state;

// DEFINE
#define SERVO_OPEN 790
#define SERVO_CLOSE 570
#define SERVO_OPEN_B 1000
#define SERVO_CLOSE_B 0

// ************************************************************************************************************************************ //
// -----------------------------------------------------------  INITIALIZATION -------------------------------------------------------- //
// ************************************************************************************************************************************ //
_INIT void init(void)
{
	// initialization timer parameters
	// time
	preset_time  = 500;
	// period
	servo_period = 4000;
	
	// initialization global variable
	g_YuMiLab.servo_cover.actual_state.successful = 0;
	
	// main state
	servo_state = 0;
}

// ************************************************************************************************************************************ //
// -----------------------------------------------------------  MAIN (Cyclic) --------------------------------------------------------- //
// ************************************************************************************************************************************ //
_CYCLIC void cyclic(void)
{
	
	switch(servo_state){
		case 0:
			{
				/*************************************** INITIALIZATION STATE ******************************************/
				// 1 -> Move servo (Open, Close)
				if(g_YuMiLab.servo_cover.cmd.Move == 1){
					// change state
					servo_state = 1;	
				}
			}
			break;
		case 1:
			{
				/*************************************** MOVE SERVO COVER STATE ******************************************/
				// in position -> 0 (isMoving)
				g_YuMiLab.servo_cover.actual_state.inPosition = 0;
				// set position
				servo_position 								  = g_YuMiLab.servo_cover.Position;
		
				// turn on -> Timer
				TON_01.IN = 1;
				TON_01.PT = preset_time;

				// call function
				TON(&TON_01);

				if(TON_01.Q == 1){
					// in position -> 1 (stop)
					g_YuMiLab.servo_cover.actual_state.inPosition = 1;
					// change state
					servo_state = 2;
				}
			}
			break;
		case 2:
			{
				/*************************************** TURN OFF SERVO STATE ******************************************/
				// servo is stop!
				g_YuMiLab.servo_cover.cmd.Move = 0;
			
				// turn off -> Timer
				TON_01.IN = 0;
		
				// reset variables
				TON_01.Q  = 0;
				TON_01.M  = 0;
		
				// aux. condition
				if(servo_position == SERVO_OPEN){
					servo_position = SERVO_OPEN_B;
				}else if(servo_position == SERVO_CLOSE){
					servo_position = SERVO_CLOSE_B;
				}
			
				// set actual position
				g_YuMiLab.servo_cover.Actual_Position = servo_position;
			
				// change state -> initialization state
				servo_state = 0;
			}
			break;
	} // end sitch
}

