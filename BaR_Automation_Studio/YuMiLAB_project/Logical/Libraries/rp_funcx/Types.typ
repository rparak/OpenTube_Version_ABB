
TYPE
	profinet_aux_m_str : 	STRUCT 
		IN_Aux : profinet_aux_in_str;
		OUT_Aux : profinet_aux_out_str;
	END_STRUCT;
	profinet_aux_out_str : 	STRUCT 
		BYTE_0 : ARRAY[0..5]OF BOOL;
		BYTE_1 : ARRAY[0..7]OF BOOL;
		BYTE_2 : BOOL;
	END_STRUCT;
	jxcp_main_read_enum : 
		(
		JXCP_R_INITIALIZATION_STATE := 0,
		JXCP_R_RW_STATE := 10,
		JXCP_R_ERROR_MODE_STATE := 20
		);
	profinet_aux_in_str : 	STRUCT 
		BYTE_0 : ARRAY[0..5]OF BOOL;
		BYTE_1 : ARRAY[0..7]OF BOOL;
		BYTE_2 : ARRAY[0..7]OF BOOL;
		BYTE_3 : ARRAY[0..7]OF BOOL;
	END_STRUCT;
	jxcp_main_enum : 
		(
		JXCP_INITIALIZATION_STATE := 0,
		JXCP_WAIT_STATE := 10,
		JXCP_NORMAL_MODE_STATE := 20,
		JXCP_CYCLE_MODE_STATE := 30,
		JXCP_ALARM_MODE_STATE := 40,
		JXCP_ERROR_MODE_STATE := 50
		);
	set_param_mode_enum : 
		(
		NORMAL_MOVE_MODE  := 0,
		CYCLE_MOVE_MODE := 1
		);
	internal_param_str : 	STRUCT 
		actual_state : jxcp_main_enum;
		before_state : jxcp_main_enum;
		actual_r_state : jxcp_main_read_enum;
		before_r_state : jxcp_main_read_enum;
		Profinet : profinet_aux_m_str;
	END_STRUCT;
	set_param_str : 	STRUCT 
		Start : BOOL;
		Stop : BOOL;
		Mode : set_param_mode_enum;
		Boundary : boundary_str;
	END_STRUCT;
	byte_3_in_str : 	STRUCT 
		Deceleration : BOOL;
		Pushing_Force : BOOL;
		Trigger_LV : BOOL;
		Pushing_Speed : BOOL;
		Moving_Force : BOOL;
		Area_out_1 : BOOL;
		Area_out_2 : BOOL;
		IN_Pos_Tolerance : BOOL;
	END_STRUCT;
	byte_2_in_str : 	STRUCT 
		Speed_Restriction : BOOL;
		Movement_Mode : BOOL;
		Speed : BOOL;
		Position : BOOL;
		Acceleration : BOOL;
	END_STRUCT;
	byte_1_in_str : 	STRUCT 
		HOLD : BOOL;
		SVON : BOOL;
		DRIVE : BOOL;
		RESET : BOOL;
		SETUP : BOOL;
		JOG_Neg : BOOL;
		JOG_Pos : BOOL;
		FLHGT : BOOL;
	END_STRUCT;
	byte_0_in_str : 	STRUCT 
		IN_0 : BOOL;
		IN_1 : BOOL;
		IN_2 : BOOL;
		IN_3 : BOOL;
		IN_4 : BOOL;
		IN_5 : BOOL;
	END_STRUCT;
	movement_mode_str : 	STRUCT 
		Absolute : BOOL;
		Relative : BOOL;
	END_STRUCT;
	alarm_str : 	STRUCT 
		Alarm_1 : USINT;
		Alarm_2 : USINT;
		Alarm_3 : USINT;
		Alarm_4 : USINT;
	END_STRUCT;
	byte_2_str : 	STRUCT 
		READY : BOOL;
	END_STRUCT;
	byte_1_str : 	STRUCT 
		BUSY : BOOL;
		SVRE : BOOL;
		SETON : BOOL;
		INP : BOOL;
		AREA : BOOL;
		WAREA : BOOL;
		ESTOP : BOOL;
		ALARM : BOOL;
	END_STRUCT;
	byte_0_str : 	STRUCT 
		OUT_0 : BOOL;
		OUT_1 : BOOL;
		OUT_2 : BOOL;
		OUT_3 : BOOL;
		OUT_4 : BOOL;
		OUT_5 : BOOL;
	END_STRUCT;
	jxcp_input_str : 	STRUCT 
		BYTE_0 : byte_0_in_str;
		BYTE_1 : byte_1_in_str;
		BYTE_2 : byte_2_in_str;
		BYTE_3 : byte_3_in_str;
		Start_Flag : BOOL;
		Movement_Mode : movement_mode_str;
		Speed : LREAL;
		Position : LREAL;
		Acceleration : LREAL;
		Deceleration : LREAL;
		Pushing_Force : LREAL;
		Trigger_LV : LREAL;
		Pushing_Speed : LREAL;
		Moving_Force : LREAL;
		AREA_1 : LREAL;
		AREA_2 : LREAL;
		IN_Position : LREAL;
	END_STRUCT;
	jxcp_output_str : 	STRUCT 
		BYTE_0 : byte_0_str;
		BYTE_1 : byte_1_str;
		BYTE_2 : byte_2_str;
		Current_Position : LREAL;
		Current_Speed : LREAL;
		Current_Pushing_Force : LREAL;
		Target_Position : LREAL;
		Warning : alarm_str;
	END_STRUCT;
	profinet_mapping_out_str : 	STRUCT 
		Output_Area : ARRAY[0..35]OF USINT;
	END_STRUCT;
	profinet_mapping_in_str : 	STRUCT 
		Input_Area : ARRAY[0..19]OF USINT;
	END_STRUCT;
	boundary_mm_str : 	STRUCT 
		max : LREAL;
		min : LREAL;
	END_STRUCT;
	boundary_str : 	STRUCT 
		Position : boundary_mm_str;
		Speed : boundary_mm_str;
		Acceleration : boundary_mm_str;
		Deceleration : boundary_mm_str;
	END_STRUCT;
END_TYPE
