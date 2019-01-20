#include "pesq_calculate.h"
#include <stdio.h>
#include "pesq.h"

double PesqCalculate(const char* pRefFileName, const char* pDegFileName, int sampleRate)
{
	if (sampleRate != 8000 && sampleRate != 16000)
	{
		return 0.0;
	}

	SIGNAL_INFO ref_info;
	SIGNAL_INFO deg_info;
	ERROR_INFO err_info;

	long Error_Flag = 0;
    char* Error_Type = "Unknown error type.";

	err_info.pesq_mos = 0;

	strcpy(ref_info.path_name, "");
	ref_info.apply_swap = 0;
	strcpy(deg_info.path_name, "");
	deg_info.apply_swap = 0;
	ref_info.input_filter = 1;
	deg_info.input_filter = 1;
	err_info.mode = NB_MODE;

	strcpy(ref_info.path_name, pRefFileName);
	strcpy(deg_info.path_name, pDegFileName);

	strcpy(ref_info.file_name, ref_info.path_name);
	if (strrchr(ref_info.file_name, '\\') != NULL)
	{
		strcpy(ref_info.file_name, 1 + strrchr(ref_info.file_name, '\\'));
	}
	if (strrchr(ref_info.file_name, '/') != NULL)
	{
		strcpy(ref_info.file_name, 1 + strrchr(ref_info.file_name, '/'));
	}
	strcpy(deg_info.file_name, deg_info.path_name);
	if (strrchr(deg_info.file_name, '\\') != NULL)
	{
		strcpy(deg_info.file_name, 1 + strrchr(deg_info.file_name, '\\'));
	}
	if (strrchr(deg_info.file_name, '/') != NULL)
	{
		strcpy(deg_info.file_name, 1 + strrchr(deg_info.file_name, '/'));
	}

	if (sampleRate == 16000)
	{
		// wide band mode
		ref_info.input_filter = 2;
		deg_info.input_filter = 2;
		err_info.mode = WB_MODE;
	}

	select_rate((long)sampleRate, &Error_Flag, &Error_Type);
	pesq_measure(&ref_info, &deg_info, &err_info, &Error_Flag, &Error_Type);

	if (Error_Flag == 0)
	{
		if (err_info.mode == NB_MODE)
		{
			printf("\nP.862 Prediction (Raw MOS, MOS-LQO):  = %.3f\t%.3f\n", (double)err_info.pesq_mos, (double)err_info.mapped_mos);
		}
		else
		{
			printf("\nP.862.2 Prediction (MOS-LQO):  = %.3f\n", (double)err_info.mapped_mos);
		}
	}
	else
	{
		printf("An error of type %ld ", Error_Flag);
		if (Error_Type != NULL)
		{
			printf("(%s) occurred during processing.\n", Error_Type);
			return -1;//如果出错，返回 -1 作为mos分
		}
		else
		{
			printf("occurred during processing.\n");
		}
	}
	return (double)err_info.mapped_mos;
}
