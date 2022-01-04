// ************************************************************************************************************************************ //
// Title: Data processing -> Barcode Scanner
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

#include <astime.h>

// ************************************************************************************************************************************ //
// -------------------------------------------------------------  DECLARATIONS -------------------------------------------------------- //
// ************************************************************************************************************************************ //
// BOOL
_LOCAL BOOL enable;
// UINT
_LOCAL UINT status;
_LOCAL UINT barcode_state;
// DATE_AND_TIME
_LOCAL DATE_AND_TIME DT1;
// DTGetTime_typ
_LOCAL DTGetTime_typ DTGetTime_1;
// INT
_LOCAL INT scan_counter;

// Declaration functions
void reset_data_mv();
void reset_data_init();

// ************************************************************************************************************************************ //
// -----------------------------------------------------------  INITIALIZATION -------------------------------------------------------- //
// ************************************************************************************************************************************ //
void _INIT ProgramInit(void)
{
	// Read/Write Data from Rpi
	(UDINT)strcpy(g_YuMiLab.scanner_m.rw_data_barcode.barcode_id, "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.rw_data_barcode.tray_id, "");
	
	// Barcode parameters
	g_YuMiLab.scanner_m.rw_data_barcode.command    = 0;
	g_YuMiLab.scanner_m.rw_data_barcode.error_code = 0;
	g_YuMiLab.scanner_m.rw_data_barcode.barcode_scanner_module_ok = 0;
	g_YuMiLab.scanner_m.rw_data_barcode.tray_position_column = 3;
	g_YuMiLab.scanner_m.rw_data_barcode.tray_position_row    = 4;
	
	// Actual Data
	(UDINT)strcpy(g_YuMiLab.scanner_m.actual_barcode_id, "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.actual_tray_id, "");
	
	
	// Data MappView {history} -> Initialization
	reset_data_mv();
	
	// FeID set/reset
	g_YuMiLab.scanner_m.feid_cmd_str.set = 0;
	g_YuMiLab.scanner_m.feid_cmd_str.reset = 0;
	g_YuMiLab.scanner_m.feid_cmd_str.enable_mv = 1;
	
	// Feid command
	(UDINT)strcpy(g_YuMiLab.scanner_m.feid_cmd_str.tray_id,"");

	// Main State
	barcode_state = 0;
	
	// Scan counter
	scan_counter = 0;
}

// ************************************************************************************************************************************ //
// -----------------------------------------------------------  MAIN (Cyclic) --------------------------------------------------------- //
// ************************************************************************************************************************************ //
void _CYCLIC ProgramCyclic(void)
{
	
	switch(barcode_state){
		case 0:
			{
				/*************************************** INITIALIZATION STATE ******************************************/
			
				// err-states: 
				/*
				ok = 0
				eID_not_in_database = 1
				tray_warning_FeID_used = 2  # no new DB entry added for sample_tray, using existing
				tray_not_created = 3  # cannot add new sample_tray
				tray_position_not_assigned = 4
				service_connection_lost = 5  # cannot access DB
				*/
				// cmd {0} -> nothing ok, cmd {1} - write FeID , cmd {2} -> scan, cmd {3} ->  write row, column
				
			
				//(UDINT)strcpy(g_YuMiLab.scanner_m.rw_data_barcode.barcode_id, "");
			
				// reset scan err.
				g_YuMiLab.scanner_m.scan_err = 0;
			
				// Copy
				g_YuMiLab.scanner_m.cmd.write_FeID = g_YuMiLab.scanner_m.feid_cmd_str.set;
			
				// Condition -> Scanner module is ok!
				if(g_YuMiLab.scanner_m.rw_data_barcode.barcode_scanner_module_ok == 1){
					// reset error code
					g_YuMiLab.scanner_m.rw_data_barcode.error_code = 0;
					
					if(g_YuMiLab.scanner_m.cmd.write_FeID == 1){
						//cmd {1} - write FeID
						barcode_state = 1;
					}else if(g_YuMiLab.scanner_m.cmd.scan == 1){
						//cmd {2} -> scan
						(UDINT)strcpy(g_YuMiLab.scanner_m.rw_data_barcode.barcode_id, "None");
						// change state
						barcode_state = 3;
					}
				}
			}
			break;
		
		case 1:
			{
				/*************************************** WRITE FEID (1) STATE ******************************************/
				// reset write (Feid) command
				g_YuMiLab.scanner_m.cmd.write_FeID = 0;
			
				// mappview -> enable
				g_YuMiLab.scanner_m.feid_cmd_str.enable_mv = 0;
			
				// copy string
				(UDINT)strcpy(g_YuMiLab.scanner_m.actual_tray_id ,g_YuMiLab.scanner_m.feid_cmd_str.tray_id);
				(UDINT)strcpy(g_YuMiLab.scanner_m.rw_data_barcode.tray_id,g_YuMiLab.scanner_m.feid_cmd_str.tray_id);
			
				// command set -> 1
				g_YuMiLab.scanner_m.rw_data_barcode.command = 1;
			
				// change state -> check result
				barcode_state = 2;
			
			}
			break;
		case 2:
			{
				/*************************************** WRITE FEID (1a) STATE ******************************************/
				// Err. Condition -> module, err. code -> see bellow (info)
				if(g_YuMiLab.scanner_m.rw_data_barcode.barcode_scanner_module_ok == 0 || g_YuMiLab.scanner_m.rw_data_barcode.error_code > 0){
					// err. state var.
					err_state_barcode = 1;
					// command -> null
					g_YuMiLab.scanner_m.rw_data_barcode.command = 0;
					// change state -> initialization
					barcode_state = 0;
				}
			
				// Condition -> cmd null
				if(g_YuMiLab.scanner_m.rw_data_barcode.command == 0){
					// change state -> initialization
					barcode_state = 0;
				}
			}
			break;
		case 3:
			{
				/*************************************** START SCAN (1) STATE ******************************************/
			
				// scan var. -> null
				g_YuMiLab.scanner_m.cmd.scan = 0;
			
				// command -> 2
				g_YuMiLab.scanner_m.rw_data_barcode.command = 2;
			
				// changa state
				barcode_state = 4;
			
				// Err. Condition -> module
				if(g_YuMiLab.scanner_m.rw_data_barcode.barcode_scanner_module_ok == 0){
					// err. state var.
					err_state_barcode = 1;
					// command -> null
					g_YuMiLab.scanner_m.rw_data_barcode.command = 0;
					// change state -> initialization
					barcode_state = 0;
				}
			}
			break;
		
		case 4:
			{
				/*************************************** START SCAN (1a) STATE ******************************************/
			
				if(g_YuMiLab.scanner_m.rw_data_barcode.command == 0){
					//cmd {3} -> write row, column
					barcode_state = 5;
				}else if(g_YuMiLab.scanner_m.rw_data_barcode.error_code > 0 || (barcode_scan_state_next == 1 && g_YuMiLab.scanner_m.rw_data_barcode.command > 0)){
					// Err. Condition -> module, err. code -> see bellow (info)
					// err. state var.
					err_state_barcode = 1;
					// command -> null
					g_YuMiLab.scanner_m.rw_data_barcode.command = 0;
					
					// continue
					barcode_state = 5;
				}
			}
			break;
		case 5:
			{
				/*************************************** START SCAN (2) STATE ******************************************/
			
				// read barcode id
				(UDINT)strcpy(g_YuMiLab.scanner_m.actual_barcode_id , g_YuMiLab.scanner_m.rw_data_barcode.barcode_id);
				// write to table
				(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.barcode_id[scan_counter],g_YuMiLab.scanner_m.rw_data_barcode.barcode_id);
				// write column, row -> index of the current barcode ID
				g_YuMiLab.scanner_m.rw_data_barcode.tray_position_column = g_YuMiLab.scanner_m.data_mv.index_column[scan_counter];
				g_YuMiLab.scanner_m.rw_data_barcode.tray_position_row    = g_YuMiLab.scanner_m.data_mv.index_row[scan_counter];
			
				// Get actual time
				DTGetTime_1.enable = 1;
				// Call function
				DTGetTime(&DTGetTime_1);
				// Write time to the table
				ascDT(DTGetTime_1.DT1,(UDINT)&g_YuMiLab.scanner_m.data_mv.time[scan_counter], 128);   
			
				// turn off function block
				DTGetTime_1.enable = 0;
				
				// Err. Condition -> err. code
				if(g_YuMiLab.scanner_m.rw_data_barcode.error_code > 0){
					// err. state var.
					err_state_barcode = 1;
					// error info -> with current index
					g_YuMiLab.scanner_m.data_mv.err_info[scan_counter] = 1;
				}
			
				// command -> 3
				g_YuMiLab.scanner_m.rw_data_barcode.command = 3;
			
				// change state
				barcode_state = 6;
		
			}
			break;
		
		case 6:
			{
				/*************************************** START SCAN (3) STATE ******************************************/
			
				// Err. Condition -> module, err. code -> see bellow (info)
				if(g_YuMiLab.scanner_m.rw_data_barcode.barcode_scanner_module_ok == 0 || g_YuMiLab.scanner_m.rw_data_barcode.error_code > 0){
					// err. state var.
					err_state_barcode = 1;
					
					// check counter max var.
					if(scan_counter < 11){
						// increase var. + 1
						scan_counter = scan_counter + 1;
					}else {
						// reset counter
						scan_counter = 0;
					}
					
					// command -> null
					g_YuMiLab.scanner_m.rw_data_barcode.command = 0;
					
					// change state -> initialization
					barcode_state = 0;
				}else{
			
					// Condition -> Command null (info from barcode app.)?
					if(g_YuMiLab.scanner_m.rw_data_barcode.command == 0){
						// check counter max var.
						if(scan_counter < 11){
							// increase var. + 1
							scan_counter = scan_counter + 1;
						}else {
							// reset counter
							scan_counter = 0;
						}
						
						// change state -> initialization
						barcode_state = 0;
					}else if(barcode_scan_state_next == 1 && g_YuMiLab.scanner_m.rw_data_barcode.command > 0){
						// Condition -> Command > 0 (no information from barcode app.) and aux. var from another task (Robot_m.c)
						// err. state var.
						err_state_barcode = 1;
						// command -> null
						g_YuMiLab.scanner_m.rw_data_barcode.command = 0;
						
						// check counter max var.
						if(scan_counter < 11){
							// increase var. + 1
							scan_counter = scan_counter + 1;
						}else {
							// reset counter
							scan_counter = 0;
						}
					
						// change state -> initialization
						barcode_state = 0;
					}
				}
				
				// Copy counter: Local var. -> Global var.
				g_YuMiLab.robot_m.counter = scan_counter;
			}
			break;
	}
	
	// function for reset current Feid
	if(g_YuMiLab.scanner_m.feid_cmd_str.reset == 1){
		// Feid -> null
		(UDINT)strcpy(g_YuMiLab.scanner_m.feid_cmd_str.tray_id,"");
		// enable textbox
		g_YuMiLab.scanner_m.feid_cmd_str.enable_mv = 1;
	}

	// reset mappview data
	if(g_YuMiLab.scanner_m.reset_data_mv == 1){
		// call function
		reset_data_init();
	}
}

// ************************************************************************************************************************************ //
// ------------------------------------------------------------  FUNCTIONS ------------------------------------------------------------ //
// ************************************************************************************************************************************ //

// **************** RESET DATA MAPPVIEW ********************** //
void reset_data_mv(){
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.barcode_id[0], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.barcode_id[1], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.barcode_id[2], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.barcode_id[3], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.barcode_id[4], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.barcode_id[5], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.barcode_id[6], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.barcode_id[7], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.barcode_id[8], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.barcode_id[9], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.barcode_id[10], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.barcode_id[11], "");
	
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.time[0], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.time[1], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.time[2], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.time[3], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.time[4], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.time[5], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.time[6], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.time[7], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.time[8], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.time[9], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.time[10], "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.data_mv.time[11], "");
	
	g_YuMiLab.scanner_m.data_mv.err_info[0] = 0;
	g_YuMiLab.scanner_m.data_mv.err_info[1] = 0;
	g_YuMiLab.scanner_m.data_mv.err_info[2] = 0;
	g_YuMiLab.scanner_m.data_mv.err_info[3] = 0;
	g_YuMiLab.scanner_m.data_mv.err_info[4] = 0;
	g_YuMiLab.scanner_m.data_mv.err_info[5] = 0;
	g_YuMiLab.scanner_m.data_mv.err_info[6] = 0;
	g_YuMiLab.scanner_m.data_mv.err_info[7] = 0;
	g_YuMiLab.scanner_m.data_mv.err_info[8] = 0;
	g_YuMiLab.scanner_m.data_mv.err_info[9] = 0;
	g_YuMiLab.scanner_m.data_mv.err_info[10] = 0;
	g_YuMiLab.scanner_m.data_mv.err_info[11] = 0;
	
	g_YuMiLab.scanner_m.data_mv.index[0] = 1;
	g_YuMiLab.scanner_m.data_mv.index[1] = 2;
	g_YuMiLab.scanner_m.data_mv.index[2] = 3;
	g_YuMiLab.scanner_m.data_mv.index[3] = 4;
	g_YuMiLab.scanner_m.data_mv.index[4] = 5;
	g_YuMiLab.scanner_m.data_mv.index[5] = 6;
	g_YuMiLab.scanner_m.data_mv.index[6] = 7;
	g_YuMiLab.scanner_m.data_mv.index[7] = 8;
	g_YuMiLab.scanner_m.data_mv.index[8] = 9;
	g_YuMiLab.scanner_m.data_mv.index[9] = 10;
	g_YuMiLab.scanner_m.data_mv.index[10] = 11;
	g_YuMiLab.scanner_m.data_mv.index[11] = 12;
	
	g_YuMiLab.scanner_m.data_mv.index_column[0] = 1;
	g_YuMiLab.scanner_m.data_mv.index_column[1] = 2;
	g_YuMiLab.scanner_m.data_mv.index_column[2] = 3;
	g_YuMiLab.scanner_m.data_mv.index_column[3] = 1;
	g_YuMiLab.scanner_m.data_mv.index_column[4] = 2;
	g_YuMiLab.scanner_m.data_mv.index_column[5] = 3;
	g_YuMiLab.scanner_m.data_mv.index_column[6] = 1;
	g_YuMiLab.scanner_m.data_mv.index_column[7] = 2;
	g_YuMiLab.scanner_m.data_mv.index_column[8] = 3;
	g_YuMiLab.scanner_m.data_mv.index_column[9] = 1;
	g_YuMiLab.scanner_m.data_mv.index_column[10] = 2;
	g_YuMiLab.scanner_m.data_mv.index_column[11] = 3;
	
	g_YuMiLab.scanner_m.data_mv.index_row[0]    = 1;
	g_YuMiLab.scanner_m.data_mv.index_row[1]    = 1;
	g_YuMiLab.scanner_m.data_mv.index_row[2]    = 1;
	g_YuMiLab.scanner_m.data_mv.index_row[3]    = 2;
	g_YuMiLab.scanner_m.data_mv.index_row[4]    = 2;
	g_YuMiLab.scanner_m.data_mv.index_row[5]    = 2;
	g_YuMiLab.scanner_m.data_mv.index_row[6]    = 3;
	g_YuMiLab.scanner_m.data_mv.index_row[7]    = 3;
	g_YuMiLab.scanner_m.data_mv.index_row[8]    = 3;
	g_YuMiLab.scanner_m.data_mv.index_row[9]    = 4;
	g_YuMiLab.scanner_m.data_mv.index_row[10]   = 4;
	g_YuMiLab.scanner_m.data_mv.index_row[11]   = 4;
} // end function

// **************** RESET INITIALIZATION DATA ********************** //
void reset_data_init(){
	// Read/Write Data from Rpi
	(UDINT)strcpy(g_YuMiLab.scanner_m.rw_data_barcode.barcode_id, "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.rw_data_barcode.tray_id, "");
	
	// Barcode parameters
	g_YuMiLab.scanner_m.rw_data_barcode.command    = 0;
	g_YuMiLab.scanner_m.rw_data_barcode.error_code = 0;
	g_YuMiLab.scanner_m.rw_data_barcode.barcode_scanner_module_ok = 0;
	g_YuMiLab.scanner_m.rw_data_barcode.tray_position_column = 3;
	g_YuMiLab.scanner_m.rw_data_barcode.tray_position_row    = 4;
	
	// Actual Data
	(UDINT)strcpy(g_YuMiLab.scanner_m.actual_barcode_id, "");
	(UDINT)strcpy(g_YuMiLab.scanner_m.actual_tray_id, "");
	
	
	// Data MappView {history} -> Initialization
	reset_data_mv();
	
	// FeID set/reset
	g_YuMiLab.scanner_m.feid_cmd_str.set = 0;
	g_YuMiLab.scanner_m.feid_cmd_str.reset = 0;
	g_YuMiLab.scanner_m.feid_cmd_str.enable_mv = 1;
	
	// Feid command
	(UDINT)strcpy(g_YuMiLab.scanner_m.feid_cmd_str.tray_id,"");

	// Main state
	barcode_state = 0;
	
	// Scan Counter
	scan_counter = 0;
} // end function

