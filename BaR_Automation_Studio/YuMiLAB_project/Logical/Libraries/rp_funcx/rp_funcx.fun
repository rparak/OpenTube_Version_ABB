
FUNCTION_BLOCK dec_to_multiple_usint
	VAR_INPUT
		Count_of_VAR : USINT;
		Decimal : LREAL;
	END_VAR
	VAR_OUTPUT
		Result_USINT_1 : USINT;
		Result_USINT_2 : USINT;
		Result_USINT_3 : USINT;
		Result_USINT_4 : USINT;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK multiple_usint_to_dec
	VAR_INPUT
		Count_of_VAR : USINT;
		USINT_1 : USINT;
		USINT_2 : USINT;
		USINT_3 : USINT;
		USINT_4 : USINT;
	END_VAR
	VAR_OUTPUT
		Result_Decimal : LREAL;
	END_VAR
END_FUNCTION_BLOCK

{REDUND_ERROR} FUNCTION_BLOCK jxcp_smc_fb (*TODO: Add your comment here*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Enable : {REDUND_UNREPLICABLE} BOOL;
		ModuleOK : BOOL;
		PROFINET_Mapping_IN : {REDUND_UNREPLICABLE} profinet_mapping_in_str;
		Input : jxcp_input_str;
		Set_Parameters : set_param_str;
	END_VAR
	VAR_OUTPUT
		Active : BOOL;
		PROFINET_Mapping_OUT : profinet_mapping_out_str;
		Output : jxcp_output_str;
	END_VAR
	VAR
		Internal : internal_param_str;
	END_VAR
END_FUNCTION_BLOCK
