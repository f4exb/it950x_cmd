/**
 * Copyright (c) 2013 ITE Corporation. All rights reserved. 
 * 
 * Date:
 *    2013/10/15
 *
 * Abstract:
 *    ITEAPIEXTest_TX.cpp : Defines the entry point for the console application.
 *    This is a sample testkit for IT9500 device (modulator).
 */

#include "options.h"
#include "api.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

InputParam input_constellation_list [] = 
{
    { "4", "QPSK"},
    { "16", "QAM16"},
    { "64", "QAM64"}
};
size_t input_constellation_list_size = INPUT_LIST_SIZE(input_constellation_list);

InputParam input_coderate_list [] = 
{
    { "1/2", "1/2"},
    { "2/3", "2/3"},
    { "3/4", "3/4"},
    { "5/6", "5/6"},
    { "7/8", "7/8"}
};
size_t input_coderate_list_size = INPUT_LIST_SIZE(input_coderate_list);

InputParam input_guard_list [] = 
{
    {"1/32", "1/32"},
    {"1/16", "1/16"},
    {"1/8", "1/8"},
    {"1/4", "1/4"}
};
size_t input_guard_list_size = INPUT_LIST_SIZE(input_guard_list);

InputParam input_fft_size_list [] = 
{
    { "2k", "2k"},
    { "8k", "8k"}
};
size_t input_fft_size_list_size = INPUT_LIST_SIZE(input_fft_size_list);

//=================================================================================================
int get_input_param_index(InputParam input_param_list[], size_t input_param_list_size, char *option)
{
    int i;
    
    for (i=0; i<input_param_list_size; i++)
    {
        if (strcmp(input_param_list[i].option, option) == 0)
        {
            return i;
        }
    }
    
    return -1;
}

//=================================================================================================
char *get_input_param_string(InputParam input_param_list[], int option_index)
{
    return input_param_list[option_index].name;
}

//=================================================================================================
double atofs(char *f)
/* standard suffixes */
{
	char last;
	int len;
	double suff = 1.0;
	len = strlen(f);
	last = f[len-1];
	f[len-1] = '\0';
	switch (last) {
		case 'g':
		case 'G':
			suff *= 1e3;
		case 'm':
		case 'M':
			suff *= 1e3;
		case 'k':
		case 'K':
			suff *= 1e3;
			suff *= atof(f);
			f[len-1] = last;
			return suff;
	}
	f[len-1] = last;
	return atof(f);
}

//=================================================================================================
void get_dc_iq(options_t *options, char *arg)
{
    char *start, *stop;

    start = arg;
    stop = strchr(start, ':') + 1;
    stop[-1] = '\0';
    
    options->dc_i = atoi(start);
    options->dc_q = atoi(stop);
    options->dc_iq_specified = 1;
}

//=================================================================================================
void usage(void)
{
    fprintf(stderr,
        "it950x_cmd_tx, a simple program to control UT-100(A|C) transmitter\n\n"
        "Use:\tit950x_cmd_tx [-options] [filename]\n"
        "\t-d device handle\n"
        "\t-f frequency_to_tune_to [Hz] (default 550 MHz)\n"
        "\t\t(suffixes k, M, G available)\n"
        "\t-B bandwidth [Hz] (default 6 MHz)\n"
        "\t\t(suffixes k, M, G available)\n"
        "\t-R Specify TS data rate [bps] (default calculated from input TS file)\n"
        "\t   Specify M to set it to max detected rate\n"
        "\t\t(suffixes k, M, G available)\n"
        "\t-Q QAM modulation [4=QPSK, 16=QAM16, 64=QAM64] (default QPSK)\n"
        "\t-C FEC coderate [1/2, 2/3, 3/4, 5/6, 7/8] (default 1/2)\n"
        "\t-G Guard interval [1/32, 1/16, 1/8, 1/4] (default 1/32)\n"
        "\t-F FFT size [2k, 8k] (default 2k, set to 2k if bandwidth = 2 MHz)\n"
        "\t-g gain [dB] (default 0)\n"
        "\t-T TPS (Transmission Parameter Signalling) cell-id\n"
        "\t-I IQ calibration filename\n"
        "\t-D DC IQ calibration values semicolon separated as <dc_i>:<dc_q>\n"
        "\t-S Insert periodical custom packets for SI SDT table\n"
        "\t-i Input TS filename\n"
        "\t-l Loop on TS input\n"
        "\t-p print device type\n"
        "\t\n\n");
    exit(1);
}


//=================================================================================================
void get_options(int argc, char **argv, options_t *options)
{
    int option_index;
    int opt;
    
    // set defaults
    options->dev_handle = 0;
    options->frequency = 550000;
    options->bandwidth = 6000;
    options->constellation_index = 0;
    options->coderate_index = 0;
    options->interval_index = 0;
    options->fftsize_index = 0;
    options->adjustoutputgain = 0;
    options->print_device_type = 0;
    options->tps_cellid_specified = 0;
    options->tps_cellid = 0;
    options->iq_table_filename = 0;
    options->dc_iq_specified = 0;
    options->dc_i = 0;
    options->dc_q = 0;
    options->insert_sisdt_custom_packets = 0;
    options->ts_use_max_rate = 0;
    options->ts_data_rate = 0;
    options->ts_loop = 0;
    options->ts_filename = 0;
    
    while ((opt = getopt(argc, argv, "d:f:B:R:Q:C:G:F:g:T:I:D:S:i:lph")) != -1) {
        switch (opt) {
        case 'd':
            options->dev_handle = atoi(optarg);
            break;
        case 'f':
            options->frequency = (Dword) (atofs(optarg)/1000.0); // stored in kHz
            break;
        case 'B':
            options->bandwidth = (Word) (atofs(optarg)/1000.0); // stored in kHz
            break;
        case 'R':
            if (strcmp("M", optarg) == 0)
            {
                options->ts_use_max_rate = 1;
            }
            else
            {
                options->ts_data_rate = (ULONGLONG) atofs(optarg); 
            }
            break;
        case 'Q':
            if ((option_index = get_input_param_index(input_constellation_list, input_constellation_list_size, optarg)) < 0)
            {
                fprintf(stderr, "QAM option \"%s\" not recognized. Sticking with default\n", optarg);
            }
            else
            {
                options->constellation_index = option_index;
            }
            break;
        case 'C':
            if ((option_index = get_input_param_index(input_coderate_list, input_coderate_list_size, optarg)) < 0)
            {
                fprintf(stderr, "FEC coderate \"%s\" not recognized. Sticking with default\n", optarg);
            }
            else
            {
                options->coderate_index = option_index;
            }
            break;
        case 'G':
            if ((option_index = get_input_param_index(input_guard_list, input_guard_list_size, optarg)) < 0)
            {
                fprintf(stderr, "Guard interval \"%s\" not recognized. Sticking with default\n", optarg);
            }
            else
            {
                options->interval_index = option_index;
            }
            break;
        case 'F':
            if ((option_index = get_input_param_index(input_fft_size_list, input_fft_size_list_size, optarg)) < 0)
            {
                fprintf(stderr, "FFT size \"%s\" not recognized. Sticking with default\n", optarg);
            }
            else
            {
                options->fftsize_index = option_index;
            }
            break;
        case 'g':
            options->adjustoutputgain = atoi(optarg);
            break;
        case 'T':
            options->tps_cellid = (Dword) atoi(optarg);
            options->tps_cellid_specified = 1;
            break;
        case 'I':
            options->iq_table_filename = strdup(optarg);
            break;
        case 'D':
            get_dc_iq(options, optarg);
            break;
        case 'S':
            options->insert_sisdt_custom_packets = 1;
            break;
        case 'i':
            options->ts_filename = strdup(optarg);
            break;
        case 'p':
            options->print_device_type = 1;
            break;
        case 'l':
            options->ts_loop = 1;
            break;
        case 'h':
        default:
            usage();
            break;
        }
    }
    
    if (options->bandwidth < 3000)
    {
        options->fftsize_index = 0;
    }
    
}

//=================================================================================================
void print_options(options_t *options)
{
    fprintf(stderr, "Frequency ...............: %d kHz\n", options->frequency);
    fprintf(stderr, "Bandwidth ...............: %d kHz\n", options->bandwidth);
    fprintf(stderr, "Modulation ..............: %s\n", get_input_param_string(input_constellation_list, options->constellation_index));
    fprintf(stderr, "FEC coderate ............: %s\n", get_input_param_string(input_coderate_list, options->coderate_index));    
    fprintf(stderr, "Guard interval ..........: %s\n", get_input_param_string(input_guard_list, options->interval_index));
    fprintf(stderr, "FFT size ................: %s\n", get_input_param_string(input_fft_size_list, options->fftsize_index));
}

//=================================================================================================
void delete_options(options_t *options)
{
    if (options->iq_table_filename)
    {
        free(options->iq_table_filename);
    }
    
    if (options->ts_filename)
    {
        free(options->ts_filename);
    }
}
