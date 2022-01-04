// ************************************************************************************************************************************ //
// Title: ABB Robot Control -> Input/Output
// Autor: Roman Parak
// Date : 8.5.2020
// Email: Roman.Parak@outlook.com
// ************************************************************************************************************************************ //

// ************************************************************************************************************************************ //
// -------------------------------------------------------------  LIBRARIES ----------------------------------------------------------- //
// ************************************************************************************************************************************ //
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

// ************************************************************************************************************************************ //
// -------------------------------------------------------------  DECLARATIONS -------------------------------------------------------- //
// ************************************************************************************************************************************ //
// UINT
_LOCAL UINT robot_state;
// INT
_LOCAL INT robot_counter, before_robot_counter;
// BOOL
_LOCAL BOOL power_on_off, motor_on_off;
_LOCAL BOOL wTO_smc;

// DEFINE
#define SERVO_OPEN 790
#define SERVO_CLOSE 570
#define SERVO_OPEN_B 1000
#define SERVO_CLOSE_B 0

// Declaration functions
void reset_data_init();

// ************************************************************************************************************************************ //
// -----------------------------------------------------------  INITIALIZATION -------------------------------------------------------- //
// ************************************************************************************************************************************ //
void _INIT ProgramInit(void)
{
	// err. state var.
	err_state_barcode = 0;
	
	// aux. variable -> turn on/turn off smc tube opener
	wTO_smc = 0;
	
	// Tube opener -> default param. (mode)
	g_YuMiLab.tube_opener.actual_state.Mode.Attached = 0;
	g_YuMiLab.tube_opener.actual_state.Mode.Close 	 = 0;
	g_YuMiLab.tube_opener.actual_state.Mode.Detached = 0;
	g_YuMiLab.tube_opener.actual_state.Mode.Open 	 = 0;
	
	// Servo Cover -> default param. (pos., move)
	g_YuMiLab.servo_cover.Position = SERVO_OPEN;
	g_YuMiLab.servo_cover.cmd.Move = 1;
	
	// Robot Counter
	robot_counter = 0;
	
	// Before robot counter
	before_robot_counter = 0;
	
	// Main State
	robot_state = 0;
}

// ************************************************************************************************************************************ //
// -----------------------------------------------------------  MAIN (Cyclic) --------------------------------------------------------- //
// ************************************************************************************************************************************ //
void _CYCLIC ProgramCyclic(void)
{
	/*******************************************************************************************************************/
	/*********************************************** SIGNALIZATION NOTE ************************************************/
	/*******************************************************************************************************************/
	/*************************************** Wait 0, In Progress 1, Successful 2 ***************************************/
	/*******************************************************************************************************************/
	
	switch(robot_state){
		case 0:
			{
				/*************************************** Start Stage ***************************************/

				// ----------------------------------------------------------------------------------- //
				// -------------------- Write actual signalization of each states -------------------- //
				// ----------------------------------------------------------------------------------- //
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Info, "Closed");
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
			
				//g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Info[robot_counter]  = 0;
				//g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Info[robot_counter]   = 0;
				//g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Info[robot_counter] = 0;

			}
			break;
		case 1:
			{
				/*************************************** Pick Tube Stage {Rack} ***************************************/
			
				// error state barcode -> reset
				err_state_barcode       = 0;
				// barcode scan next -> reset
				barcode_scan_state_next = 0;

				// default mode -> reset
				g_YuMiLab.tube_opener.actual_state.Mode.Open  = 0;
				g_YuMiLab.tube_opener.actual_state.Mode.Close = 0;
			
				// Condition -> counter
				if(before_robot_counter < 12){
					// copy
					before_robot_counter = robot_counter;
				}else{
					// call function -> reset
					reset_data_init();
				}
			
				// // robot actual state (signalization)
				g_YuMiLab.robot_m.actual_sate.In_Progress = robot_counter + 1;
				g_YuMiLab.robot_m.actual_sate.Successful  = robot_counter;
				g_YuMiLab.robot_m.actual_sate.Wait 		  = 12 - (robot_counter + 1);
			
				//servo_position = 790;
			
				// Condition -> Servo Cover (open/close)
				if(g_YuMiLab.servo_cover.actual_state.inPosition == 1 && g_YuMiLab.servo_cover.Actual_Position != SERVO_OPEN_B){
					// set -> open position var.
					g_YuMiLab.servo_cover.Position = SERVO_OPEN;
					// move
					g_YuMiLab.servo_cover.cmd.Move = 1;
				}
			
				// reset var. tube oepenr -> detached
				g_YuMiLab.tube_opener.actual_state.Mode.Detached = 0;
			
				// ----------------------------------------------------------------------------------- //
				// -------------------- Write actual signalization of each states -------------------- //
				// ----------------------------------------------------------------------------------- //
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_str, "In Progress");
				g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_int = 1;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_int = 0;
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
			
				// current sample -> info
				g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Info[robot_counter] = 1;
			
			}
			break;
		case 2:
			{
				/*************************************** Barcode Scan Stage  ***************************************/
			
				// ----------------------------------------------------------------------------------- //
				// -------------------- Write actual signalization of each states -------------------- //
				// ----------------------------------------------------------------------------------- //
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_str, "In Progress");
				g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_int = 1;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_int = 0;
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
			
				// current sample -> info
				g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Info[robot_counter] = 2;
			
			}
			break;
		case 3:
			{
				/*************************************** Place Tube Stage {Tube Opener} ***************************************/
			
				//servo_position = 570;
			
				// Condition -> Servo Cover (open/close)
				if(g_YuMiLab.servo_cover.actual_state.inPosition == 1 && g_YuMiLab.servo_cover.Actual_Position != SERVO_CLOSE_B){
					// set -> open position var.
					g_YuMiLab.servo_cover.Position = SERVO_CLOSE;
					// move
					g_YuMiLab.servo_cover.cmd.Move = 1;
				}
			
				// ----------------------------------------------------------------------------------- //
				// -------------------- Write actual signalization of each states -------------------- //
				// ----------------------------------------------------------------------------------- //
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_str, "In Progress");
				g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_int = 1;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_int = 0;
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
		
			}
			break;
		case 4:
			{
				/*************************************** Pick Pipette Stage  ***************************************/
			
				// open tube
				g_YuMiLab.tube_opener.cmd.Mode.Open_T = 1;
			
				//servo_position = 0;
			
				// mode -> attached
				g_YuMiLab.tube_opener.actual_state.Mode.Attached = 0;
			
				// ----------------------------------------------------------------------------------- //
				// -------------------- Write actual signalization of each states -------------------- //
				// ----------------------------------------------------------------------------------- //
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Info, "Open");
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_str, "In Progress");
				g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_int = 1;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_int = 0;
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
			
				// current sample -> info
				g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Info[robot_counter] = 1;
			
				// barcode scan -> set next 
				barcode_scan_state_next = 1;
			}
			break;
		case 5:
			{
				/*************************************** Place Pipette Stage {Tube Opener} ***************************************/
			
				// ----------------------------------------------------------------------------------- //
				// -------------------- Write actual signalization of each states -------------------- //
				// ----------------------------------------------------------------------------------- //
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_str, "In Progress");
				g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_int = 1;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_int = 0;
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
			
				// current sample -> info
				g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Info[robot_counter] = 2;
			
			}
			break;
		case 6:
			{
				/*************************************** Place Pipette Stage {Sample Tray} ***************************************/
				//servo_position = 790;
			
				// Condition -> Servo Cover (open/close)
				if(g_YuMiLab.servo_cover.actual_state.inPosition == 1 && g_YuMiLab.servo_cover.Actual_Position != SERVO_OPEN_B){
					// set -> open position var.
					g_YuMiLab.servo_cover.Position = SERVO_OPEN;
					// move
					g_YuMiLab.servo_cover.cmd.Move = 1;
				}
			
				// tube opener -> close
				g_YuMiLab.tube_opener.cmd.Mode.Closed_T = 1;
				// ----------------------------------------------------------------------------------- //
				// -------------------- Write actual signalization of each states -------------------- //
				// ----------------------------------------------------------------------------------- //
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_str, "In Progress");
				g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_int = 1;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_int = 0;
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
			
				// current sample -> info
				g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Info[robot_counter] = 1;
			
			}
			break;
		case 7:
			{
				/*************************************** Place Pipette Stage {Bin} ***************************************/
				//servo_position = 570;
			
				// Condition -> Servo Cover (open/close)
				if(g_YuMiLab.servo_cover.actual_state.inPosition == 1 && g_YuMiLab.servo_cover.Actual_Position != SERVO_CLOSE_B){
					// set -> open position var.
					g_YuMiLab.servo_cover.Position = SERVO_CLOSE;
					// move
					g_YuMiLab.servo_cover.cmd.Move = 1;
				}
			
				// ----------------------------------------------------------------------------------- //
				// -------------------- Write actual signalization of each states -------------------- //
				// ----------------------------------------------------------------------------------- //
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Info, "Closed");
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_str, "In Progres");
				g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_int = 1;
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
			
				// increase counter
				g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Info = robot_counter + 1;
			
				// current sample -> info
				g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Info[robot_counter] = 2;
			
			}
			break;
		case 8:
			{
				/*************************************** Pick Tube Stage {Tube Opener} ***************************************/
			
				//servo_position = 0;
			
				// ----------------------------------------------------------------------------------- //
				// -------------------- Write actual signalization of each states -------------------- //
				// ----------------------------------------------------------------------------------- //
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_str, "Wait");
				g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_int = 0;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_str, "In Progres");
				g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_int = 1;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_int = 2;
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
			}
			break;
		case 9:
			{
				/*************************************** Place Tube Stage {Rack} ***************************************/

				// ----------------------------------------------------------------------------------- //
				// -------------------- Write actual signalization of each states -------------------- //
				// ----------------------------------------------------------------------------------- //
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_str, "In Progres");
				g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_int = 1;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_int = 2;
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
			
				// -------------------- Counter ++ Stage -------------------- //
				
				// Condition -> set err. to current sample
				if(err_state_barcode == 1){
					// sample info
					g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Info[robot_counter]  = 3;
					g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Info[robot_counter]   = 3;
					g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Info[robot_counter] = 3;
					
					// reset err. state
					err_state_barcode = 0;
				}
				
				// reset mode -> null
				g_YuMiLab.tube_opener.cmd.Mode.Open_T   = 0;
				g_YuMiLab.tube_opener.cmd.Mode.Closed_T = 0;
			
				//robot_counter = g_YuMiLab.robot_m.counter;
			
				if(before_robot_counter == robot_counter){
					// increase counter -> +1
					robot_counter = robot_counter + 1;
				}
			}
			break;
		case 10:
			{
				/*************************************** Pick Tube Stage {Rack} ***************************************/
			
				// ----------------------------------------------------------------------------------- //
				// -------------------- Write actual signalization of each states -------------------- //
				// ----------------------------------------------------------------------------------- //
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_int = 2;
			
				(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_str, "Successful");
				g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_int = 2;
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
				// ----------------------------------------------------------------------------------- //
			
			}
			break;
	}
	
	// turn on/turn off smc tube opener
	if(wTO_smc == 1){
		// attached
		g_YuMiLab.tube_opener.actual_state.Mode.Attached = 1;
		// close
		g_YuMiLab.tube_opener.actual_state.Mode.Close = 1;
		// detached
		g_YuMiLab.tube_opener.actual_state.Mode.Detached = 1;
		// open
		g_YuMiLab.tube_opener.actual_state.Mode.Open = 1;
	}
	
	// Condition -> Start: Power On is true
	if(power_on_off == 1){
		if(g_YuMiLab.robot_m.cmd.in_position_1 == 1){
			robot_state = 0;
		}else if(g_YuMiLab.robot_m.cmd.in_position_2 == 1){
			robot_state = 1;
		}else if(g_YuMiLab.robot_m.cmd.in_position_3 == 1){
			robot_state = 2;
		}else if(g_YuMiLab.robot_m.cmd.in_position_4 == 1){
			robot_state = 3;
		}else if(g_YuMiLab.robot_m.cmd.in_position_5 == 1){
			robot_state = 4;
		}else if(g_YuMiLab.robot_m.cmd.in_position_6 == 1){
			robot_state = 5;
		}else if(g_YuMiLab.robot_m.cmd.in_position_7 == 1){
			robot_state = 6;
		}else if(g_YuMiLab.robot_m.cmd.in_position_8 == 1){
			robot_state = 7;
		}else if(g_YuMiLab.robot_m.cmd.in_position_9 == 1){
			robot_state = 8;
		}else if(g_YuMiLab.robot_m.cmd.in_position_10 == 1){
			robot_state = 9;
		}
	}
	
	// Mappview -> Tube parameters Type 0, 1
	if(g_YuMiLab.robot_m.parameters_info.Tube.type == 0){
		g_YuMiLab.robot_m.parameters_info.Tube.param_a = 25;
		g_YuMiLab.robot_m.parameters_info.Tube.param_b = 20;
		g_YuMiLab.robot_m.parameters_info.Tube.param_c = 100;
		
		g_YuMiLab.robot_m.parameters_info.Tube.capacity = 7;
	}else if(g_YuMiLab.robot_m.parameters_info.Tube.type == 1){
		g_YuMiLab.robot_m.parameters_info.Tube.param_a = 10;
		g_YuMiLab.robot_m.parameters_info.Tube.param_b = 15;
		g_YuMiLab.robot_m.parameters_info.Tube.param_c = 75;
		
		g_YuMiLab.robot_m.parameters_info.Tube.capacity = 3;
	}
	
	// Mappview -> Pipette parameters Type 0, 1
	if(g_YuMiLab.robot_m.parameters_info.Pipette.type == 0){
		g_YuMiLab.robot_m.parameters_info.Pipette.param_a = 10;
		g_YuMiLab.robot_m.parameters_info.Pipette.param_b = 5;
		g_YuMiLab.robot_m.parameters_info.Pipette.param_c = 80;
		
		g_YuMiLab.robot_m.parameters_info.Pipette.capacity = 3;
	}else if(g_YuMiLab.robot_m.parameters_info.Pipette.type == 1){
		g_YuMiLab.robot_m.parameters_info.Pipette.param_a = 15;
		g_YuMiLab.robot_m.parameters_info.Pipette.param_b = 10;
		g_YuMiLab.robot_m.parameters_info.Pipette.param_c = 100;
		
		g_YuMiLab.robot_m.parameters_info.Pipette.capacity = 5;
	}
	
	// Power on/off robot ABB
	if(power_on_off == 1){
		g_YuMiLab.robot_m.cmd.power_off = 0;
		g_YuMiLab.robot_m.cmd.power_on  = 1;
	}else if(power_on_off == 0){
		robot_state = 0;
		g_YuMiLab.robot_m.cmd.power_on  = 0;
		g_YuMiLab.robot_m.cmd.power_off = 1;
	}
	
	// Motor on/off robot ABB
	if(g_YuMiLab.robot_m.actual_sate.is_motor_on == 1){
		motor_on_off = 1;
	}else if(g_YuMiLab.robot_m.actual_sate.is_motor_off == 1){
		motor_on_off = 0;
	}
	
	// reset mappview parameters
	if(g_YuMiLab.scanner_m.reset_data_mv == 1){
		reset_data_init();
	}
}

// ************************************************************************************************************************************ //
// ------------------------------------------------------------  FUNCTIONS ------------------------------------------------------------ //
// ************************************************************************************************************************************ //

// **************** RESET INITIALIZATION DATA ********************** //
void reset_data_init(){
	// robot actual state
	g_YuMiLab.robot_m.actual_sate.In_Progress = 0;
	g_YuMiLab.robot_m.actual_sate.Successful = 0;
	g_YuMiLab.robot_m.actual_sate.Wait = 0;
	
	// Robot Counter
	robot_counter = 0;
	
	// Before robot counter
	before_robot_counter = 0;
	
	// err. state var.
	err_state_barcode = 0;
	
	// -------------------- DEFAULT STR. SIGNALIZATION -------------------- //
	(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_str, "Wait");
	g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Status_int = 0;
			
	(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_str, "Wait");
	g_YuMiLab.robot_m.info_stage.Stage_2_Barcode.Status_int = 0;
			
	(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_str, "Wait");
	g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Status_int = 0;
			
	(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_str, "Wait");
	g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Status_int = 0;
			
	(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_str, "Wait");
	g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Status_int = 0;
			
	(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_str, "Wait");
	g_YuMiLab.robot_m.info_stage.Stage_6_Bin.Status_int = 0;
			
	(UDINT)strcpy(g_YuMiLab.robot_m.info_stage.Stage_3_TubeO.Info, "Closed");
	
	
	g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Info[0] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Info[1] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Info[2] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Info[3] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Info[4] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Info[5] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Info[6] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Info[7] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Info[8] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Info[9] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Info[10] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_1_TubeR.Info[11] = 0;
	
	g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Info[0] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Info[1] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Info[2] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Info[3] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Info[4] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Info[5] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Info[6] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Info[7] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Info[8] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Info[9] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Info[10] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_4_PipR.Info[11] = 0;
	
	g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Info[0] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Info[1] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Info[2] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Info[3] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Info[4] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Info[5] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Info[6] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Info[7] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Info[8] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Info[9] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Info[10] = 0;
	g_YuMiLab.robot_m.info_stage.Stage_5_TroghP.Info[11] = 0;
} // end function
