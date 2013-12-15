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

#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <string.h>
#include "type.h"

typedef struct {
    char *option;
    char *name;
} InputParam;

extern InputParam input_constellation_list [];
extern InputParam input_coderate_list [];
extern InputParam input_guard_list [];
extern InputParam input_fft_size_list [];
extern size_t input_constellation_list_size;
extern size_t input_coderate_list_size;
extern size_t input_guard_list_size;
extern size_t input_fft_size_list_size;

#define INPUT_LIST_SIZE(x) sizeof(x)/sizeof(InputParam)

typedef struct s_options
{
    unsigned int dev_handle;
    Dword frequency;
    Word  bandwidth;
    Byte  constellation_index;
    Byte  coderate_index;
    Byte  interval_index;
    Byte  fftsize_index;
    int adjustoutputgain;
    int print_device_type;
    int tps_cellid_specified;
    Dword tps_cellid;
    char *iq_table_filename;
    int dc_iq_specified;
    int dc_i;
    int dc_q;
    int insert_sisdt_custom_packets;
    int ts_use_max_rate;
    ULONGLONG ts_data_rate;
    int ts_loop;
    char *ts_filename;
} options_t;

int get_input_param_index(InputParam input_param_list[], size_t input_param_list_size, char *option);
char *get_input_param_string(InputParam input_param_list[], int option_index);
double atofs(char *f);
void usage(void);
void get_options(int argc, char **argv, options_t *options);
void print_options(options_t *options);
void delete_options(options_t *options);

#endif // __OPTIONS_H__
