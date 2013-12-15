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
void get_stat_info(options_t *options, char *arg)
{
    char *start, *stop;

    start = arg;
    stop = strchr(start, ':') + 1;
    
    if (stop)
    {
        stop += 1;
        stop[-1] = '\0';
        options->stat_filename = strdup(stop);
    }
    else
    {
        options->stat_filename = 0;
    }
    
    options->stat_count = atoi(start);
}

//=================================================================================================
void usage(void)
{
    fprintf(stderr,
        "it950x_cmd_rx, a simple program to control UT-100(B|D) receiver\n\n"
        "Use:\tit950x_cmd_rx [-options] [filename]\n"
        "\t-d device handle\n"
        "\t-f frequency_to_tune_to [Hz] (default 550 MHz)\n"
        "\t\t(suffixes k, M, G available)\n"
        "\t-B bandwidth [Hz] (default 6 MHz)\n"
        "\t\t(suffixes k, M, G available)\n"
        "\t-s Perform statistics <count>[:<filename>]\n"
        "\t-l Output limit [TS file: MBytes | Packet test: ms] (defaut 0: no limit)\n"
        "\t-o Output TS filename with packet test ('-' for standard output)\n"
        "\t-v verbose (set to get packet statistics display)\n"
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
    options->stat_count = -1;
    options->stat_filename = 0;
    options->packet_limit = 0;
    options->ts_filename = 0;
    options->verbose = 0;
    
    while ((opt = getopt(argc, argv, "d:f:B:s:o:l:vh")) != -1) 
    {
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
        case 's':
            get_stat_info(options, optarg);
            break;
        case 'l':
            options->packet_limit = atoi(optarg);
            break;
        case 'o':
            options->ts_filename = strdup(optarg);
            break;
        case 'v':
            options->verbose = 1;
            break;
        case 'h':
        default:
            usage();
            break;
        }
    }
}

//=================================================================================================
void print_options(options_t *options)
{
    fprintf(stderr, "Frequency ...............: %d kHz\n", options->frequency);
    fprintf(stderr, "Bandwidth ...............: %d kHz\n", options->bandwidth);
}

//=================================================================================================
void delete_options(options_t *options)
{
    if (options->stat_filename)
    {
        free(options->stat_filename);
    }
    
    if (options->ts_filename)
    {
        free(options->ts_filename);
    }
}
