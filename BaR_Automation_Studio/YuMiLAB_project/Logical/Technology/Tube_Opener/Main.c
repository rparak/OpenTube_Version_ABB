// ************************************************************************************************************************************ //
// Title: Tube Opener Control
// Autor: Roman Parak
// Date : 8.5.2020
// Email: Roman.Parak@outlook.com
// ************************************************************************************************************************************ //

// ************************************************************************************************************************************ //
// -------------------------------------------------------------  LIBRARIES ----------------------------------------------------------- //
// ************************************************************************************************************************************ //
#include <bur/plctypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "rp_funcx.h"

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

// ************************************************************************************************************************************ //
// -------------------------------------------------------------  DECLARATIONS -------------------------------------------------------- //
// ************************************************************************************************************************************ //
// JXCP1 {SMC} // STRUCT // #include "rp_funcx.h" -> jxcp_smc_fb.c
// Z Axis
_LOCAL struct jxcp_smc_fb jxcp_smc_fb_Z_lin; // node 1
_LOCAL struct jxcp_smc_fb jxcp_smc_fb_Z_rot; // node 3
_LOCAL struct jxcp_smc_fb jxcp_smc_fb_Z_gripp; // node 4
// X Axis
_LOCAL struct jxcp_smc_fb jxcp_smc_fb_X_lin; // node 2
_LOCAL struct jxcp_smc_fb jxcp_smc_fb_X_gripp; // node 5

// USINT
_LOCAL USINT test_tube_state, test_tube_state_before;
// BOOL
_LOCAL BOOL power_on, home;
_LOCAL BOOL start_tt, stop_tt;
_LOCAL BOOL attach_t, detach_t;
_LOCAL BOOL open_t, close_t;
// LREAL
_LOCAL LREAL tube_type_R_zG, tube_type_R_zP, tube_type_R_xG, tube_type_R_zPh;

// ************************************************************************************************************************************ //
// -----------------------------------------------------------  INITIALIZATION -------------------------------------------------------- //
// ************************************************************************************************************************************ //
void _INIT ProgramInit(void)
{
	
	// ******************************************************************************* //
	// -------------------- INITIALIZATION JXCP1 (SMC) {PROFINET} -------------------- //
	// ******************************************************************************* //
	
	// NODE 1 // LESH16RK - 100 (Max force - 70)
	jxcp_smc_fb_Z_lin.Enable = 1;
	// ----- Boundaries (MAX/MIN) ----- //
	// Position
	jxcp_smc_fb_Z_lin.Set_Parameters.Boundary.Position.max = 100;
	jxcp_smc_fb_Z_lin.Set_Parameters.Boundary.Position.min = 0;
	// Speed
	jxcp_smc_fb_Z_lin.Set_Parameters.Boundary.Speed.max = 200;
	jxcp_smc_fb_Z_lin.Set_Parameters.Boundary.Speed.min = 10;
	// Acceleration
	jxcp_smc_fb_Z_lin.Set_Parameters.Boundary.Acceleration.max = 5000;
	jxcp_smc_fb_Z_lin.Set_Parameters.Boundary.Acceleration.min = 100;
	// Deceleration
	jxcp_smc_fb_Z_lin.Set_Parameters.Boundary.Deceleration.max = 5000;
	jxcp_smc_fb_Z_lin.Set_Parameters.Boundary.Deceleration.min = 100;
	// Set Mode - Normal/Cycle
	jxcp_smc_fb_Z_lin.Set_Parameters.Mode = CYCLE_MOVE_MODE;
	// Set Default Parameters
	jxcp_smc_fb_Z_lin.Input.Speed 		 = 200; //50
	jxcp_smc_fb_Z_lin.Input.Acceleration = 5000; // 1000
	jxcp_smc_fb_Z_lin.Input.Deceleration = 5000; // 1000
	jxcp_smc_fb_Z_lin.Input.Moving_Force = 100;
	jxcp_smc_fb_Z_lin.Input.IN_Position  = 0.5;
	
	// NODE 2 // LESH16LK-100 (Max force - 70)
	jxcp_smc_fb_X_lin.Enable = 1;
	// ----- Boundaries (MAX/MIN) ----- //
	// Position
	jxcp_smc_fb_X_lin.Set_Parameters.Boundary.Position.max = 100;
	jxcp_smc_fb_X_lin.Set_Parameters.Boundary.Position.min = 0;
	// Speed
	jxcp_smc_fb_X_lin.Set_Parameters.Boundary.Speed.max = 200;
	jxcp_smc_fb_X_lin.Set_Parameters.Boundary.Speed.min = 10;
	// Acceleration
	jxcp_smc_fb_X_lin.Set_Parameters.Boundary.Acceleration.max = 5000;
	jxcp_smc_fb_X_lin.Set_Parameters.Boundary.Acceleration.min = 100;
	// Deceleration
	jxcp_smc_fb_X_lin.Set_Parameters.Boundary.Deceleration.max = 5000;
	jxcp_smc_fb_X_lin.Set_Parameters.Boundary.Deceleration.min = 100;
	// Set Mode - Normal/Cycle
	jxcp_smc_fb_X_lin.Set_Parameters.Mode = CYCLE_MOVE_MODE;
	// Set Default Parameters
	jxcp_smc_fb_X_lin.Input.Speed 		 = 200;
	jxcp_smc_fb_X_lin.Input.Acceleration = 5000;
	jxcp_smc_fb_X_lin.Input.Deceleration = 5000;
	jxcp_smc_fb_X_lin.Input.Moving_Force = 100;
	jxcp_smc_fb_X_lin.Input.IN_Position  = 0.5;
	
	// NODE 3 // LER30K (Max force - 50)
	jxcp_smc_fb_Z_rot.Enable = 1;
	// ----- Boundaries (MAX/MIN) ----- //
	// Position
	jxcp_smc_fb_Z_rot.Set_Parameters.Boundary.Position.max = 320;
	jxcp_smc_fb_Z_rot.Set_Parameters.Boundary.Position.min = 0;
	// Speed
	jxcp_smc_fb_Z_rot.Set_Parameters.Boundary.Speed.max = 200;
	jxcp_smc_fb_Z_rot.Set_Parameters.Boundary.Speed.min = 10;
	// Acceleration
	jxcp_smc_fb_Z_rot.Set_Parameters.Boundary.Acceleration.max = 3000;
	jxcp_smc_fb_Z_rot.Set_Parameters.Boundary.Acceleration.min = 100;
	// Deceleration
	jxcp_smc_fb_Z_rot.Set_Parameters.Boundary.Deceleration.max = 3000;
	jxcp_smc_fb_Z_rot.Set_Parameters.Boundary.Deceleration.min = 100;
	// Set Mode - Normal/Cycle
	jxcp_smc_fb_Z_rot.Set_Parameters.Mode = CYCLE_MOVE_MODE;
	// Set Default Parameters
	jxcp_smc_fb_Z_rot.Input.Speed 		 = 200;
	jxcp_smc_fb_Z_rot.Input.Acceleration = 3000;
	jxcp_smc_fb_Z_rot.Input.Deceleration = 3000;
	jxcp_smc_fb_Z_rot.Input.Moving_Force = 100;
	jxcp_smc_fb_Z_rot.Input.IN_Position  = 0.5;
	
	// NODE 4 // LEHF32K2-32 (Max force - 100)
	jxcp_smc_fb_Z_gripp.Enable = 1;
	// ----- Boundaries (MAX/MIN) ----- //
	// Position
	jxcp_smc_fb_Z_gripp.Set_Parameters.Boundary.Position.max = 16;
	jxcp_smc_fb_Z_gripp.Set_Parameters.Boundary.Position.min = 0;
	// Speed
	jxcp_smc_fb_Z_gripp.Set_Parameters.Boundary.Speed.max = 100;
	jxcp_smc_fb_Z_gripp.Set_Parameters.Boundary.Speed.min = 10;
	// Acceleration
	jxcp_smc_fb_Z_gripp.Set_Parameters.Boundary.Acceleration.max = 2000;
	jxcp_smc_fb_Z_gripp.Set_Parameters.Boundary.Acceleration.min = 100;
	// Deceleration
	jxcp_smc_fb_Z_gripp.Set_Parameters.Boundary.Deceleration.max = 2000;
	jxcp_smc_fb_Z_gripp.Set_Parameters.Boundary.Deceleration.min = 100;
	// Set Mode - Normal/Cycle
	jxcp_smc_fb_Z_gripp.Set_Parameters.Mode = CYCLE_MOVE_MODE;
	// Set Default Parameters
	jxcp_smc_fb_Z_gripp.Input.Speed 	   = 100;
	jxcp_smc_fb_Z_gripp.Input.Acceleration = 2000;
	jxcp_smc_fb_Z_gripp.Input.Deceleration = 2000;
	jxcp_smc_fb_Z_gripp.Input.Moving_Force = 100;
	jxcp_smc_fb_Z_gripp.Input.IN_Position  = 0.5;
	
	// NODE 5 // LEHF10K2-16 (Max force - 100)
	jxcp_smc_fb_X_gripp.Enable = 1;
	// ----- Boundaries (MAX/MIN) ----- //
	// Position
	jxcp_smc_fb_X_gripp.Set_Parameters.Boundary.Position.max = 32;
	jxcp_smc_fb_X_gripp.Set_Parameters.Boundary.Position.min = 0;
	// Speed
	jxcp_smc_fb_X_gripp.Set_Parameters.Boundary.Speed.max = 80;
	jxcp_smc_fb_X_gripp.Set_Parameters.Boundary.Speed.min = 10;
	// Acceleration
	jxcp_smc_fb_X_gripp.Set_Parameters.Boundary.Acceleration.max = 2000;
	jxcp_smc_fb_X_gripp.Set_Parameters.Boundary.Acceleration.min = 100;
	// Deceleration
	jxcp_smc_fb_X_gripp.Set_Parameters.Boundary.Deceleration.max = 2000;
	jxcp_smc_fb_X_gripp.Set_Parameters.Boundary.Deceleration.min = 100;
	// Set Mode - Normal/Cycle
	jxcp_smc_fb_X_gripp.Set_Parameters.Mode = CYCLE_MOVE_MODE;
	// Set Default Parameters
	jxcp_smc_fb_X_gripp.Input.Speed 	   = 50;
	jxcp_smc_fb_X_gripp.Input.Acceleration = 1000;
	jxcp_smc_fb_X_gripp.Input.Deceleration = 1000;
	jxcp_smc_fb_X_gripp.Input.Moving_Force = 100;
	jxcp_smc_fb_X_gripp.Input.IN_Position   = 10;
//	jxcp_smc_fb_X_gripp.Input.Pushing_Speed = 5;
//	jxcp_smc_fb_X_gripp.Input.Trigger_LV    = 10;
//	jxcp_smc_fb_X_gripp.Input.Pushing_Force = 50;
	
	// before state
	test_tube_state_before = 0;
	
	// button -> Power, Home
	power_on = 0;
	home     = 0;
	
	// start/stop 
	start_tt = 0;
	stop_tt  = 0;
	
	// initialization -> object att/det, etc. signalization
	attach_t = 0;
	detach_t = 0;
	open_t   = 0;
	close_t  = 0;
	
	// initialization Z, X axes variable
	tube_type_R_zG = 10;
	tube_type_R_zP = 86;
	tube_type_R_xG = 15;
	tube_type_R_zPh = 0;
	
	// main state
	test_tube_state = 0;
}

// ************************************************************************************************************************************ //
// -----------------------------------------------------------  MAIN (Cyclic) --------------------------------------------------------- //
// ************************************************************************************************************************************ //
void _CYCLIC ProgramCyclic(void)
{
	switch(test_tube_state){
		case 0:
			{
				/*************************************** INITIALIZATION STATE ******************************************/
				
				// set actual state
				test_tube_state_before = 0;
			
				// Power On -> Condition
				if(power_on == 1){
					// Active each SMC Axes 
					if(jxcp_smc_fb_Z_lin.Active == 1 && jxcp_smc_fb_X_lin.Active == 1 && jxcp_smc_fb_Z_rot.Active == 1 
					&& jxcp_smc_fb_Z_gripp.Active == 1 && jxcp_smc_fb_X_gripp.Active == 1){
						// change state -> power
						test_tube_state = 1;
					}
				}
			}
			break;
		case 1:
			{
				/*************************************** POWER ON STATE ******************************************/
				// set actual state
				test_tube_state_before = 1;
			
				// Power On
				jxcp_smc_fb_Z_lin.Input.BYTE_1.SVON = 1;
				jxcp_smc_fb_X_lin.Input.BYTE_1.SVON = 1;
				jxcp_smc_fb_Z_rot.Input.BYTE_1.SVON = 1;
				jxcp_smc_fb_Z_gripp.Input.BYTE_1.SVON = 1;
				jxcp_smc_fb_X_gripp.Input.BYTE_1.SVON = 1;
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					
					// err. state
					test_tube_state = 100;
				}else {
					// Condition -> Power On is True
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.SVRE == 1 && jxcp_smc_fb_X_lin.Output.BYTE_1.SVRE == 1 && jxcp_smc_fb_Z_rot.Output.BYTE_1.SVRE == 1
					&& jxcp_smc_fb_Z_gripp.Output.BYTE_1.SVRE == 1 && jxcp_smc_fb_X_gripp.Output.BYTE_1.SVRE == 1){
						// set var. PowerOn
						g_YuMiLab.tube_opener.actual_state.PowerOn = 1;
						// Home -> Conditon
						if(home == 1){
							// change state -> home
							test_tube_state = 2;
						}
					}
				}
			}
			break;
		case 2:
			{
				/*************************************** HOME (1) STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 2;
			
				// home -> null
				home = 0;
			
				// Home
				jxcp_smc_fb_Z_lin.Input.BYTE_1.SETUP = 1;
				jxcp_smc_fb_X_lin.Input.BYTE_1.SETUP = 1;
				jxcp_smc_fb_Z_rot.Input.BYTE_1.SETUP = 1;
				jxcp_smc_fb_Z_gripp.Input.BYTE_1.SETUP = 1;
				jxcp_smc_fb_X_gripp.Input.BYTE_1.SETUP = 1;
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else {
					// Condition -> Busy (homing)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.BUSY == 1
					|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.BUSY == 1){
						test_tube_state = 3;
					}
				}
			}
			break;
		case 3:
			{
				/*************************************** HOME (2) STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 3;
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else {
					// Condition -> Home is True
					if((jxcp_smc_fb_Z_lin.Output.BYTE_1.SETON == 1 && jxcp_smc_fb_Z_lin.Output.BYTE_1.INP == 1) && (jxcp_smc_fb_X_lin.Output.BYTE_1.SETON == 1 && jxcp_smc_fb_X_lin.Output.BYTE_1.INP == 1) 
						&& (jxcp_smc_fb_Z_rot.Output.BYTE_1.SETON == 1 && jxcp_smc_fb_Z_rot.Output.BYTE_1.INP == 1) && (jxcp_smc_fb_Z_gripp.Output.BYTE_1.SETON == 1 && jxcp_smc_fb_Z_gripp.Output.BYTE_1.INP == 1)
					&& (jxcp_smc_fb_X_gripp.Output.BYTE_1.SETON == 1 && jxcp_smc_fb_X_gripp.Output.BYTE_1.INP == 1)){
						g_YuMiLab.tube_opener.actual_state.isHome = 1;
						// change state -> control
						test_tube_state = 5;
					}
				}
			}
			break;
		case 5:
			{
				/*************************************** WAIT STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 5;
			
				// stop var. reset
				stop_tt = 0;
			
				// home -> turn off
				jxcp_smc_fb_Z_lin.Input.BYTE_1.SETUP = 0;
				jxcp_smc_fb_X_lin.Input.BYTE_1.SETUP = 0;
				jxcp_smc_fb_Z_rot.Input.BYTE_1.SETUP = 0;
				jxcp_smc_fb_Z_gripp.Input.BYTE_1.SETUP = 0;
				jxcp_smc_fb_X_gripp.Input.BYTE_1.SETUP = 0;
			
				if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else{
					// start cyclic move
					if(start_tt == 1){
						// default var. -> detach command
						g_YuMiLab.tube_opener.cmd.Mode.Detach_T = 1;
						
						// change state
						test_tube_state = 6;	
					}
				}
			}
			break;
		case 6:
			{
				/*************************************** CYCLIC MOVE CMD STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 6;
			
				// start var. reset
				start_tt = 0;
			
				if(detach_t == 1 && g_YuMiLab.tube_opener.actual_state.Mode.Detached != 1){
					// Detach Tube
					test_tube_state = 10;
				}else if(attach_t == 1 && g_YuMiLab.tube_opener.actual_state.Mode.Attached != 1){
					// Attach Tube
					test_tube_state = 20;
				}else if(open_t == 1 && g_YuMiLab.tube_opener.actual_state.Mode.Open != 1){
					// Open Tube
					test_tube_state = 30;
				}else if(close_t == 1 && g_YuMiLab.tube_opener.actual_state.Mode.Close != 1){
					// Close Tube
					test_tube_state = 60;
				}
			
				if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else{
					if(stop_tt == 1){
						// stop cyclic move
						test_tube_state = 5;	
					}
				}
			}
			break;
		case 10:
			{
				/*************************************** DETACH TUBE (1) STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 10;
			
				// reset variable -> detach
				g_YuMiLab.tube_opener.cmd.Mode.Detach_T = 0;
			
				jxcp_smc_fb_Z_lin.Input.Position = tube_type_R_zPh;
				jxcp_smc_fb_X_lin.Input.Position = 100;
				jxcp_smc_fb_Z_rot.Input.Position = 90;
				jxcp_smc_fb_Z_gripp.Input.Position = 16;
				jxcp_smc_fb_X_gripp.Input.Pushing_Speed = 0;
				jxcp_smc_fb_X_gripp.Input.Trigger_LV    = 0;
				jxcp_smc_fb_X_gripp.Input.Pushing_Force = 0;
				jxcp_smc_fb_X_gripp.Input.IN_Position   = 0.5;
				jxcp_smc_fb_X_gripp.Input.Position = 32;
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> Busy (moving)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.BUSY == 1
					|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.BUSY == 1){
						test_tube_state = 11;
					}
				}
			}
			break;
		case 11:
			{
				/*************************************** DETACH TUBE (1a) STATE ******************************************/
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> In Position (each axes)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_Z_rot.Output.BYTE_1.INP == 1
					&& jxcp_smc_fb_Z_gripp.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_gripp.Output.BYTE_1.INP == 1){
						// signalization -> detached!
						g_YuMiLab.tube_opener.actual_state.Mode.Detached = 1;
						// change state
						test_tube_state = 6;
					}
				}
			}
			break;
		case 20:
			{
				/*************************************** ATTACH TUBE (1) STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 20;
			
				// reset variable -> attach
				g_YuMiLab.tube_opener.cmd.Mode.Attach_T = 0;
			
				jxcp_smc_fb_Z_lin.Input.Position = tube_type_R_zPh;
				jxcp_smc_fb_X_lin.Input.Position = 100;
				jxcp_smc_fb_Z_rot.Input.Position = 90;
				jxcp_smc_fb_Z_gripp.Input.Position = 16;
				jxcp_smc_fb_X_gripp.Input.Pushing_Speed = 5;
				jxcp_smc_fb_X_gripp.Input.Trigger_LV    = 10;
				jxcp_smc_fb_X_gripp.Input.Pushing_Force = 50;
				jxcp_smc_fb_X_gripp.Input.IN_Position   = 10;
				jxcp_smc_fb_X_gripp.Input.Position = tube_type_R_xG;
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> Busy (moving)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.BUSY == 1
					|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.BUSY == 1){
						test_tube_state = 21;
					}
				}
			}
			break;
		case 21:
			{
				/*************************************** ATTACH TUBE (1a) STATE ******************************************/
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> In Position (each axes)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_Z_rot.Output.BYTE_1.INP == 1
					&& jxcp_smc_fb_Z_gripp.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_gripp.Output.BYTE_1.INP == 1){
						
						g_YuMiLab.tube_opener.actual_state.Mode.Attached = 1;
						
						test_tube_state = 6;
					}
				}
			}
			break;
		case 30:
			{
				/*************************************** OPEN TUBE (1) STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 30;
			
				// reset variable -> open tube
				g_YuMiLab.tube_opener.cmd.Mode.Open_T = 0;
			
				jxcp_smc_fb_Z_lin.Input.Position = tube_type_R_zPh;
				jxcp_smc_fb_X_lin.Input.Position = 0;
				jxcp_smc_fb_Z_rot.Input.Position = 90;
				jxcp_smc_fb_Z_gripp.Input.Position = 16;
				jxcp_smc_fb_X_gripp.Input.Position = tube_type_R_xG;
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> Busy (moving)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.BUSY == 1
					|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.BUSY == 1){
						test_tube_state = 31;
					}
				}	
			}
			break;
		case 31:
			{
				/*************************************** OPEN TUBE (1a) STATE ******************************************/
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> In Position (each axes)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_Z_rot.Output.BYTE_1.INP == 1
					&& jxcp_smc_fb_Z_gripp.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_gripp.Output.BYTE_1.INP == 1){
						test_tube_state = 32;
					}
				}
			}
			break;
		case 32:
			{
				/*************************************** OPEN TUBE (2) STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 32;
			
				// reset variable -> open tube
				g_YuMiLab.tube_opener.cmd.Mode.Open_T = 0;
			
				// set position parameters of each axes
				jxcp_smc_fb_Z_lin.Input.Position = tube_type_R_zP;
				jxcp_smc_fb_X_lin.Input.Position = 0;
				jxcp_smc_fb_Z_rot.Input.Position = 90;
				jxcp_smc_fb_Z_gripp.Input.Position = 16;
				jxcp_smc_fb_X_gripp.Input.Position = tube_type_R_xG;
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> Busy (moving)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.BUSY == 1
					|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.BUSY == 1){
						test_tube_state = 33;
					}
				}
			}
			break;
		case 33:
			{
				/*************************************** OPEN TUBE (2a) STATE ******************************************/
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> In Position (each axes)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_Z_rot.Output.BYTE_1.INP == 1
					&& jxcp_smc_fb_Z_gripp.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_gripp.Output.BYTE_1.INP == 1){
						test_tube_state = 34;
					}
				}
			}
			break;
		case 34:
			{
				/*************************************** OPEN TUBE (3) STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 34;
			
				// reset variable -> open tube
				g_YuMiLab.tube_opener.cmd.Mode.Open_T = 0;
			
				// set position parameters of some axes
				jxcp_smc_fb_Z_lin.Input.Position = tube_type_R_zP;
				jxcp_smc_fb_X_lin.Input.Position = 0;
				jxcp_smc_fb_Z_rot.Input.Position = 90;
				// set force parameters of some axes
				jxcp_smc_fb_Z_gripp.Input.Pushing_Speed = 5;
				jxcp_smc_fb_Z_gripp.Input.Trigger_LV    = 80;
				jxcp_smc_fb_Z_gripp.Input.Pushing_Force = 100;
				jxcp_smc_fb_Z_gripp.Input.IN_Position   = 10;
				jxcp_smc_fb_Z_gripp.Input.Position = tube_type_R_zG;
				jxcp_smc_fb_X_gripp.Input.Position = tube_type_R_xG;
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> Busy (moving)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.BUSY == 1
					|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.BUSY == 1){
						test_tube_state = 35;
					}
				}
			}
			break;
		case 35:
			{
				/*************************************** OPEN TUBE (3a) STATE ******************************************/
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> In Position (each axes)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_Z_rot.Output.BYTE_1.INP == 1
					&& jxcp_smc_fb_Z_gripp.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_gripp.Output.BYTE_1.INP == 1){
						test_tube_state = 36;
					}
				}
			}
			break;
		case 36:
			{
				/*************************************** OPEN TUBE (4) STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 36;
			
				// reset variable -> open tube
				g_YuMiLab.tube_opener.cmd.Mode.Open_T = 0;
			
				// set position parameters of each axes
				jxcp_smc_fb_Z_lin.Input.Position = tube_type_R_zPh;
				jxcp_smc_fb_X_lin.Input.Position = 0;
				jxcp_smc_fb_Z_rot.Input.Position = 90;
				jxcp_smc_fb_Z_gripp.Input.Position = tube_type_R_zG;
				jxcp_smc_fb_X_gripp.Input.Position = tube_type_R_xG;
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> Busy (moving)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.BUSY == 1
					|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.BUSY == 1){
						test_tube_state = 37;
					}
				}
			}
			break;
		case 37:
			{
				/*************************************** OPEN TUBE (4a) STATE ******************************************/
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> In Position (each axes)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_Z_rot.Output.BYTE_1.INP == 1
					&& jxcp_smc_fb_Z_gripp.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_gripp.Output.BYTE_1.INP == 1){
						test_tube_state = 38;
					}
				}
			}
			break;
		case 38:
			{
				/*************************************** OPEN TUBE (5) STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 38;
			
				// reset variable -> open tube
				g_YuMiLab.tube_opener.cmd.Mode.Open_T = 0;
			
				// set position parameters of each axes
				jxcp_smc_fb_Z_lin.Input.Position = tube_type_R_zPh;
				jxcp_smc_fb_X_lin.Input.Position = 100;
				jxcp_smc_fb_Z_rot.Input.Position = 90;
				jxcp_smc_fb_Z_gripp.Input.Position = tube_type_R_zG;
				jxcp_smc_fb_X_gripp.Input.Position = tube_type_R_xG;
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> Busy (moving)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.BUSY == 1
					|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.BUSY == 1){
						test_tube_state = 39;
					}
				}
			}
			break;
		case 39:
			{
				/*************************************** OPEN TUBE (5a) STATE ******************************************/
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> In Position (each axes)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_Z_rot.Output.BYTE_1.INP == 1
					&& jxcp_smc_fb_Z_gripp.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_gripp.Output.BYTE_1.INP == 1){
						g_YuMiLab.tube_opener.actual_state.Mode.Open = 1;
						
						test_tube_state = 6;
					}
				}
			}
			break;
		case 50:
			{
				/*************************************** POWER OFF STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 50;
			
				// start/stop var. reset
				start_tt = 0;
				stop_tt  = 0;
			
				// power off -> each axes
				jxcp_smc_fb_Z_lin.Input.BYTE_1.SVON = 0;
				jxcp_smc_fb_X_lin.Input.BYTE_1.SVON = 0;
				jxcp_smc_fb_Z_rot.Input.BYTE_1.SVON = 0;
				jxcp_smc_fb_Z_gripp.Input.BYTE_1.SVON = 0;
				jxcp_smc_fb_X_gripp.Input.BYTE_1.SVON = 0;
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else {
					// power off -> is true
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.SVRE == 0 && jxcp_smc_fb_X_lin.Output.BYTE_1.SVRE == 0 && jxcp_smc_fb_Z_rot.Output.BYTE_1.SVRE == 0
					&& jxcp_smc_fb_Z_gripp.Output.BYTE_1.SVRE == 0 && jxcp_smc_fb_X_gripp.Output.BYTE_1.SVRE == 0){
						// change state -> initialization
						test_tube_state = 0;
					}
				}
			}
			break;
		case 60:
			{
				/*************************************** CLOSE TUBE (1) STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 60;
			
				// reset variable -> closed tube
				g_YuMiLab.tube_opener.cmd.Mode.Closed_T = 0;
			
				//set position parameters of some axes
				jxcp_smc_fb_Z_lin.Input.Position = tube_type_R_zPh;
				jxcp_smc_fb_X_lin.Input.Position = 0;
				jxcp_smc_fb_Z_rot.Input.Position = 90;
				// set force parameters of some axes
				jxcp_smc_fb_Z_gripp.Input.Pushing_Speed = 0;
				jxcp_smc_fb_Z_gripp.Input.Trigger_LV    = 0;
				jxcp_smc_fb_Z_gripp.Input.Pushing_Force = 0;
				jxcp_smc_fb_Z_gripp.Input.IN_Position   = 0.5;
				jxcp_smc_fb_Z_gripp.Input.Position = jxcp_smc_fb_Z_gripp.Output.Current_Position;
				jxcp_smc_fb_X_gripp.Input.Position = tube_type_R_xG;
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> Busy (moving)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.BUSY == 1
					|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.BUSY == 1){
						test_tube_state = 61;
					}
				}
			}
			break;
		case 61:
			{
				/*************************************** CLOSE TUBE (1a) STATE ******************************************/
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> In Position (each axes)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_Z_rot.Output.BYTE_1.INP == 1
					&& jxcp_smc_fb_Z_gripp.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_gripp.Output.BYTE_1.INP == 1){
						test_tube_state = 62;
					}
				}
			}
			break;
		case 62:
			{
				/*************************************** CLOSE TUBE (2) STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 62;
			
				// reset variable -> closed tube
				g_YuMiLab.tube_opener.cmd.Mode.Closed_T = 0;
			
				// set position parameters of each axes
				jxcp_smc_fb_Z_lin.Input.Position = tube_type_R_zP;
				jxcp_smc_fb_X_lin.Input.Position = 0;
				jxcp_smc_fb_Z_rot.Input.Position = 90;
				jxcp_smc_fb_Z_gripp.Input.Position = jxcp_smc_fb_Z_gripp.Output.Current_Position;
				jxcp_smc_fb_X_gripp.Input.Position = tube_type_R_xG;
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> Busy (moving)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.BUSY == 1
					|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.BUSY == 1){
						test_tube_state = 63;
					}
				}
			}
			break;
		case 63:
			{
				/*************************************** CLOSE TUBE (2a) STATE ******************************************/
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> In Position (each axes)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_Z_rot.Output.BYTE_1.INP == 1
					&& jxcp_smc_fb_Z_gripp.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_gripp.Output.BYTE_1.INP == 1){
						test_tube_state = 64;
					}
				}
			}
			break;
		case 64:
			{
				/*************************************** CLOSE TUBE (3) STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 64;
			
				// reset variable -> closed tube
				g_YuMiLab.tube_opener.cmd.Mode.Closed_T = 0;
			
				// set position parameters of each axes
				jxcp_smc_fb_Z_lin.Input.Position = tube_type_R_zP;
				jxcp_smc_fb_X_lin.Input.Position = 0;
				jxcp_smc_fb_Z_rot.Input.Position = 90;
				jxcp_smc_fb_Z_gripp.Input.Position = 16;
				jxcp_smc_fb_X_gripp.Input.Position = tube_type_R_xG;
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> Busy (moving)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.BUSY == 1
					|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.BUSY == 1){
						test_tube_state = 65;
					}
				}
			}
			break;
		case 65:
			{
				/*************************************** CLOSE TUBE (3a) STATE ******************************************/
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> In Position (each axes)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_Z_rot.Output.BYTE_1.INP == 1
					&& jxcp_smc_fb_Z_gripp.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_gripp.Output.BYTE_1.INP == 1){
						test_tube_state = 66;
					}
				}
			}
			break;
		case 66:
			{
				/*************************************** CLOSE TUBE (4) STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 66;
			
				// reset variable -> closed tube
				g_YuMiLab.tube_opener.cmd.Mode.Closed_T = 0;
			
				// set position parameters of each axes
				jxcp_smc_fb_Z_lin.Input.Position = tube_type_R_zPh;
				jxcp_smc_fb_X_lin.Input.Position = 0;
				jxcp_smc_fb_Z_rot.Input.Position = 90;
				jxcp_smc_fb_Z_gripp.Input.Position = 16;
				jxcp_smc_fb_X_gripp.Input.Position = tube_type_R_xG;
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> Busy (moving)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.BUSY == 1
					|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.BUSY == 1){
						test_tube_state = 67;
					}
				}
			}
			break;
		case 67:
			{
				/*************************************** CLOSE TUBE (4a) STATE ******************************************/
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> In Position (each axes)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_Z_rot.Output.BYTE_1.INP == 1
					&& jxcp_smc_fb_Z_gripp.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_gripp.Output.BYTE_1.INP == 1){
						test_tube_state = 68;
					}
				}
			}
			break;
		case 68:
			{
				/*************************************** CLOSE TUBE (5) STATE ******************************************/
			
				// set actual state
				test_tube_state_before = 68;
			
				// reset variable -> closed tube
				g_YuMiLab.tube_opener.cmd.Mode.Closed_T = 0;
			
				// set position parameters of each axes
				jxcp_smc_fb_Z_lin.Input.Position = tube_type_R_zPh;
				jxcp_smc_fb_X_lin.Input.Position = 100;
				jxcp_smc_fb_Z_rot.Input.Position = 90;
				jxcp_smc_fb_Z_gripp.Input.Position = 16;
				jxcp_smc_fb_X_gripp.Input.Position = tube_type_R_xG;
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> Busy (moving)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.BUSY == 1
					|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.BUSY == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.BUSY == 1){
						test_tube_state = 69;
					}
				}
			}
			break;
		case 69:
			{
				/*************************************** CLOSE TUBE (5a) STATE ******************************************/
			
				// Condition -> Check Alarm
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 1
				|| jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 1 || jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 1){
					// err. state
					test_tube_state = 100;
				}else if(power_on == 0){
					// power off -> change state
					test_tube_state = 50;
				}else if(home == 1){
					// home -> repeat
					test_tube_state = 2;
				}else {
					// Condition -> In Position (each axes)
					if(jxcp_smc_fb_Z_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_lin.Output.BYTE_1.INP == 1 && jxcp_smc_fb_Z_rot.Output.BYTE_1.INP == 1
					&& jxcp_smc_fb_Z_gripp.Output.BYTE_1.INP == 1 && jxcp_smc_fb_X_gripp.Output.BYTE_1.INP == 1){
						
						g_YuMiLab.tube_opener.actual_state.Mode.Close = 1;
						
						test_tube_state = 6;
					}
				}
			}
			break;
		case 100:
			{
				// reset alarm -> each  axes
				jxcp_smc_fb_Z_lin.Input.BYTE_1.RESET = 1;
				jxcp_smc_fb_X_lin.Input.BYTE_1.RESET = 1;
				jxcp_smc_fb_Z_rot.Input.BYTE_1.RESET = 1;
				jxcp_smc_fb_Z_gripp.Input.BYTE_1.RESET = 1;
				jxcp_smc_fb_X_gripp.Input.BYTE_1.RESET = 1;
			
				// Condition -> Alarm (T or F each  axes)?
				if(jxcp_smc_fb_Z_lin.Output.BYTE_1.ALARM == 0 && jxcp_smc_fb_X_lin.Output.BYTE_1.ALARM == 0 && jxcp_smc_fb_Z_rot.Output.BYTE_1.ALARM == 0
				&& jxcp_smc_fb_Z_gripp.Output.BYTE_1.ALARM == 0 && jxcp_smc_fb_X_gripp.Output.BYTE_1.ALARM == 0){
					// null reset var.
					jxcp_smc_fb_Z_lin.Input.BYTE_1.RESET = 0;
					jxcp_smc_fb_X_lin.Input.BYTE_1.RESET = 0;
					jxcp_smc_fb_Z_rot.Input.BYTE_1.RESET = 0;
					jxcp_smc_fb_Z_gripp.Input.BYTE_1.RESET = 0;
					jxcp_smc_fb_X_gripp.Input.BYTE_1.RESET = 0;
					// change state -> before state
					test_tube_state = test_tube_state_before;
				}else{
					// change state -> null reset var.
					test_tube_state = 101;
				}
			}
			break;
		case 101:
			{
				// reset alarm -> null
				jxcp_smc_fb_Z_lin.Input.BYTE_1.RESET = 0;
				jxcp_smc_fb_X_lin.Input.BYTE_1.RESET = 0;
				jxcp_smc_fb_Z_rot.Input.BYTE_1.RESET = 0;
				jxcp_smc_fb_Z_gripp.Input.BYTE_1.RESET = 0;
				jxcp_smc_fb_X_gripp.Input.BYTE_1.RESET = 0;
			
				// change err. {repeat}
				test_tube_state = 100;
			}
			break;
		
	}
	
	// Command -> Power
	if(g_YuMiLab.tube_opener.cmd.Power == 1){
		power_on = 1;
	}else{
		g_YuMiLab.tube_opener.actual_state.PowerOn = 0;
		power_on = 0;
	}
	// Command -> Home
	if(g_YuMiLab.tube_opener.cmd.Home == 1){
		g_YuMiLab.tube_opener.actual_state.isHome = 0;
		
		home = 1;
	}
	
	// Copy: Global Commands -> Local var.
	// Start/Stop
	start_tt = g_YuMiLab.tube_opener.cmd.Start_Move;
	stop_tt  = g_YuMiLab.tube_opener.cmd.Stop_Move;
	// object att/det, etc. signalization
	attach_t = g_YuMiLab.tube_opener.cmd.Mode.Attach_T;
	detach_t = g_YuMiLab.tube_opener.cmd.Mode.Detach_T;
	open_t   = g_YuMiLab.tube_opener.cmd.Mode.Open_T;
	close_t  = g_YuMiLab.tube_opener.cmd.Mode.Closed_T;
	
	// Reset each -> edge
	if(g_YuMiLab.tube_opener.cmd.Reset == 1){
		g_YuMiLab.tube_opener.actual_state.Mode.Attached = 0;
		g_YuMiLab.tube_opener.actual_state.Mode.Detached = 0;
		g_YuMiLab.tube_opener.actual_state.Mode.Open = 0;
		g_YuMiLab.tube_opener.actual_state.Mode.Close = 0;
		
		g_YuMiLab.tube_opener.cmd.Reset = 0;
	}
	
	// Copy: Local var. -> Global Parameters {start individual axes}
	jxcp_smc_fb_Z_lin.Set_Parameters.Start 	 = start_tt;
	jxcp_smc_fb_X_lin.Set_Parameters.Start 	 = start_tt;
	jxcp_smc_fb_Z_rot.Set_Parameters.Start 	 = start_tt;
	jxcp_smc_fb_Z_gripp.Set_Parameters.Start = start_tt;
	jxcp_smc_fb_X_gripp.Set_Parameters.Start = start_tt;
	// Copy: Local var. -> Global Parameters {stop individual axes}
	jxcp_smc_fb_Z_lin.Set_Parameters.Stop  	 = stop_tt;
	jxcp_smc_fb_X_lin.Set_Parameters.Stop  	 = stop_tt;
	jxcp_smc_fb_Z_rot.Set_Parameters.Stop  	 = stop_tt;
	jxcp_smc_fb_Z_gripp.Set_Parameters.Stop  = stop_tt;
	jxcp_smc_fb_X_gripp.Set_Parameters.Stop  = stop_tt;
	
	// -------------------- CALL FUNCTION -------------------- //
	jxcp_smc_fb(&jxcp_smc_fb_Z_lin);
	jxcp_smc_fb(&jxcp_smc_fb_X_lin);
	jxcp_smc_fb(&jxcp_smc_fb_Z_rot);
	jxcp_smc_fb(&jxcp_smc_fb_Z_gripp);
	jxcp_smc_fb(&jxcp_smc_fb_X_gripp);
}
