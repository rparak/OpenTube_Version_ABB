
TYPE
	to_act_mode_str : 	STRUCT 
		Attached : BOOL;
		Detached : BOOL;
		Open : BOOL;
		Close : BOOL;
	END_STRUCT;
	to_mode_str : 	STRUCT 
		Attach_T : BOOL;
		Detach_T : BOOL;
		Open_T : BOOL;
		Closed_T : BOOL;
	END_STRUCT;
	to_cmd_str : 	STRUCT 
		Power : BOOL;
		Home : BOOL;
		Start_Move : BOOL;
		Stop_Move : BOOL;
		Mode : to_mode_str;
		Reset : BOOL;
	END_STRUCT;
	to_state_str : 	STRUCT 
		PowerOn : BOOL;
		isHome : BOOL;
		Mode : to_act_mode_str;
	END_STRUCT;
	tube_opener_m_str : 	STRUCT 
		cmd : to_cmd_str;
		actual_state : to_state_str;
	END_STRUCT;
	tube_pipette_param_str : 	STRUCT 
		param_c : INT;
		param_b : INT;
		param_a : INT;
		capacity : INT;
		type : USINT;
	END_STRUCT;
	robot_tp_param_str : 	STRUCT 
		Tube : tube_pipette_param_str;
		Pipette : tube_pipette_param_str;
	END_STRUCT;
	robot_as_str : 	STRUCT 
		In_Progress : USINT;
		Wait : USINT;
		Successful : USINT;
		is_home : BOOL;
		is_motor_on : BOOL;
		is_motor_off : BOOL;
	END_STRUCT;
	robot_st_3_str : 	STRUCT 
		Status_str : STRING[80];
		Status_int : USINT;
		Info : USINT;
	END_STRUCT;
	robot_st_2_str : 	STRUCT 
		Status_str : STRING[80];
		Status_int : USINT;
		Info : STRING[80];
	END_STRUCT;
	robot_st_1_str : 	STRUCT 
		Status_str : STRING[80];
		Status_int : USINT;
	END_STRUCT;
	robot_st_tp_str : 	STRUCT 
		Status_str : STRING[80];
		Status_int : USINT;
		Barcode_str : ARRAY[0..11]OF STRING[128];
		Info : ARRAY[0..11]OF USINT;
	END_STRUCT;
	robot_stage_str : 	STRUCT 
		Stage_1_TubeR : robot_st_tp_str;
		Stage_2_Barcode : robot_st_1_str;
		Stage_3_TubeO : robot_st_2_str;
		Stage_4_PipR : robot_st_tp_str;
		Stage_5_TroghP : robot_st_tp_str;
		Stage_6_Bin : robot_st_3_str;
	END_STRUCT;
	robot_main_str : 	STRUCT 
		cmd : robot_cmd_main_str;
		info_stage : robot_stage_str;
		actual_sate : robot_as_str;
		parameters_info : robot_tp_param_str;
		counter : USINT;
	END_STRUCT;
	robot_cmd_main_str : 	STRUCT 
		change_position : BOOL;
		in_position_1 : BOOL;
		in_position_2 : BOOL;
		in_position_3 : BOOL;
		in_position_4 : BOOL;
		in_position_5 : BOOL;
		in_position_6 : BOOL;
		in_position_7 : BOOL;
		in_position_8 : BOOL;
		in_position_9 : BOOL;
		in_position_10 : BOOL;
		in_position_11 : BOOL;
		start_move : BOOL;
		home : BOOL;
		pp_to_main : BOOL;
		power_off : BOOL;
		power_on : BOOL;
		stop_move : BOOL;
	END_STRUCT;
	barcode_cmd_main_str : 	STRUCT 
		write_FeID : BOOL;
		scan : BOOL;
	END_STRUCT;
	feid_cmd_str : 	STRUCT 
		set : BOOL;
		reset : BOOL;
		tray_id : STRING[128];
		enable_mv : BOOL;
	END_STRUCT;
	servo_cover_m_str : 	STRUCT 
		cmd : sc_cmd_str;
		actual_state : sc_actS_str;
		Position : INT;
		Actual_Position : INT;
	END_STRUCT;
	main_global_str : 	STRUCT 
		scanner_m : barcode_str;
		robot_m : robot_main_str;
		tube_opener : tube_opener_m_str;
		servo_cover : servo_cover_m_str;
	END_STRUCT;
	rw_data_barcode_str : 	STRUCT 
		barcode_id : STRING[128];
		tray_id : STRING[128];
		error_code : INT;
		barcode_scanner_module_ok : BOOL;
		command : INT;
		tray_position_row : INT;
		tray_position_column : INT;
	END_STRUCT;
	barcode_data_mv_str : 	STRUCT 
		index : ARRAY[0..11]OF INT;
		barcode_id : ARRAY[0..11]OF STRING[128];
		index_row : ARRAY[0..11]OF INT;
		index_column : ARRAY[0..11]OF INT;
		time : ARRAY[0..11]OF STRING[128];
		err_info : ARRAY[0..11]OF INT;
	END_STRUCT;
	barcode_str : 	STRUCT 
		data_mv : barcode_data_mv_str;
		actual_tray_id : STRING[128];
		actual_barcode_id : STRING[128];
		rw_data_barcode : rw_data_barcode_str;
		reset_data_mv : BOOL;
		feid_cmd_str : feid_cmd_str;
		cmd : barcode_cmd_main_str;
		scan_err : BOOL;
	END_STRUCT;
	sc_cmd_str : 	STRUCT 
		Move : BOOL;
	END_STRUCT;
	sc_actS_str : 	STRUCT 
		inPosition : BOOL;
		successful : BOOL;
	END_STRUCT;
END_TYPE
