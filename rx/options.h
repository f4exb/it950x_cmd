/**
 * Abstract:
 *    Demodulator device control
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
    int stat_count;
    char *stat_filename;
    char *ts_filename;
    int packet_limit;
    int verbose;
    int relock;
} options_t;

int get_input_param_index(InputParam input_param_list[], size_t input_param_list_size, char *option);
char *get_input_param_string(InputParam input_param_list[], int option_index);
double atofs(char *f);
void usage(void);
void get_options(int argc, char **argv, options_t *options);
void print_options(options_t *options);
void delete_options(options_t *options);

#endif // __OPTIONS_H__
