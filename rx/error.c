#include "error.h"

#include <string.h>

char *DTV_Error(int dtv_errno)
{
    if (dtv_errno == ERR_NO_ERROR)
        return "ERR_NO_ERROR"; 
    if (dtv_errno == ERR_RESET_TIMEOUT)
        return "ERR_NO_ERROR";
    if (dtv_errno == ERR_WRITE_REG_TIMEOUT)
        return "ERR_WRITE_REG_TIMEOUT";
    if (dtv_errno == ERR_WRITE_TUNER_TIMEOUT)
        return "ERR_WRITE_TUNER_TIMEOUT";
    if (dtv_errno == ERR_WRITE_TUNER_FAIL)
        return "ERR_WRITE_TUNER_FAIL";
    if (dtv_errno == ERR_READ_TUNER_TIMEOUT)
        return "ERR_READ_TUNER_TIMEOUT";
    if (dtv_errno == ERR_RSD_COUNTER_NOT_READY)
        return "ERR_RSD_COUNTER_NOT_READY";
    if (dtv_errno == ERR_VTB_COUNTER_NOT_READY)
        return "ERR_VTB_COUNTER_NOT_READY";
    if (dtv_errno == ERR_FEC_MON_NOT_ENABLED)
        return "ERR_FEC_MON_NOT_ENABLED";
    if (dtv_errno == ERR_INVALID_DEV_TYPE)
        return "ERR_INVALID_DEV_TYPE";
    if (dtv_errno == ERR_INVALID_TUNER_TYPE)
        return "ERR_INVALID_TUNER_TYPE";
    if (dtv_errno == ERR_OPEN_FILE_FAIL)
        return "ERR_OPEN_FILE_FAIL";
    if (dtv_errno == ERR_WRITEFILE_FAIL)
        return "ERR_WRITEFILE_FAIL";
    if (dtv_errno == ERR_READFILE_FAIL)
        return "ERR_READFILE_FAIL";
    if (dtv_errno == ERR_CREATEFILE_FAIL)
        return "ERR_CREATEFILE_FAIL";
    if (dtv_errno == ERR_MALLOC_FAIL)
        return "ERR_MALLOC_FAIL";
    if (dtv_errno == ERR_INVALID_FILE_SIZE)
        return "ERR_INVALID_FILE_SIZE";
    if (dtv_errno == ERR_INVALID_READ_SIZE)
        return "ERR_INVALID_READ_SIZE";
    if (dtv_errno == ERR_LOAD_FW_DONE_BUT_FAIL)
        return "ERR_LOAD_FW_DONE_BUT_FAIL";
    if (dtv_errno == ERR_NOT_IMPLEMENTED)
        return "ERR_NOT_IMPLEMENTED";
    if (dtv_errno == ERR_WRITE_MBX_TUNER_TIMEOUT)
        return "ERR_WRITE_MBX_TUNER_TIMEOUT";
    if (dtv_errno == ERR_DIV_MORE_THAN_8_CHIPS)
        return "ERR_DIV_MORE_THAN_8_CHIPS";
    if (dtv_errno == ERR_DIV_NO_CHIPS)
        return "ERR_DIV_NO_CHIPS";
    if (dtv_errno == ERR_SUPER_FRAME_CNT_0)
        return "ERR_SUPER_FRAME_CNT_0";
    if (dtv_errno == ERR_INVALID_FFT_MODE)
        return "ERR_INVALID_FFT_MODE";
    if (dtv_errno == ERR_INVALID_CONSTELLATION_MODE)
        return "ERR_INVALID_FFT_MODE";
    if (dtv_errno == ERR_RSD_PKT_CNT_0)
        return "ERR_RSD_PKT_CNT_0";
    if (dtv_errno == ERR_FFT_SHIFT_TIMEOUT)
        return "ERR_FFT_SHIFT_TIMEOUT";
    if (dtv_errno == ERR_WAIT_TPS_TIMEOUT)
        return "ERR_WAIT_TPS_TIMEOUT";
    if (dtv_errno == ERR_INVALID_BW)
        return "ERR_INVALID_BW";
    if (dtv_errno == ERR_INVALID_BUF_LEN)
        return "ERR_INVALID_BUF_LEN";
    if (dtv_errno == ERR_NULL_PTR)
        return "ERR_NULL_PTR";
    if (dtv_errno == ERR_MT_TUNE_FAIL)
        return "ERR_MT_TUNE_FAIL";
    if (dtv_errno == ERR_MT_OPEN_FAIL)
        return "ERR_MT_OPEN_FAIL";
    if (dtv_errno == ERR_INVALID_AGC_VOLT)
        return "ERR_INVALID_AGC_VOLT";
    if (dtv_errno == ERR_CMD_NOT_SUPPORTED)
        return "ERR_CMD_NOT_SUPPORTED";
    if (dtv_errno == ERR_CE_NOT_READY)
        return "ERR_CE_NOT_READY";
    if (dtv_errno == ERR_READ_TUNER_FAIL)
        return "ERR_READ_TUNER_FAIL";
    if (dtv_errno == ERR_EMBX_INT_NOT_CLEARED)
        return "ERR_EMBX_INT_NOT_CLEARED";
    if (dtv_errno == ERR_INV_PULLUP_VOLT)
        return "ERR_INV_PULLUP_VOLT";
    if (dtv_errno == ERR_FREQ_OUT_OF_RANGE)
        return "ERR_FREQ_OUT_OF_RANGE";
    if (dtv_errno == ERR_MT_NOT_AVAILABLE)
        return "ERR_MT_NOT_AVAILABLE";
    if (dtv_errno == ERR_BACK_TO_BOOTCODE_FAIL)
        return "ERR_BACK_TO_BOOTCODE_FAIL";
    if (dtv_errno == ERR_GET_BUFFER_VALUE_FAIL)
        return "ERR_GET_BUFFER_VALUE_FAIL";
    if (dtv_errno == ERR_MEM_ALLOC_FAIL)
        return "ERR_MEM_ALLOC_FAIL";
    if (dtv_errno == ERR_INVALID_POS)
        return "ERR_INVALID_POS";
    if (dtv_errno == ERR_DYNA_TOP_FAIL)
        return "ERR_DYNA_TOP_FAIL";
    if (dtv_errno == ERR_INVALID_INDEX)
        return "ERR_INVALID_INDEX";
    if (dtv_errno == ERR_WAIT_PVIT_TIMEOUT)
        return "ERR_WAIT_PVIT_TIMEOUT";
    if (dtv_errno == ERR_FUNC_INTERRUPTED)
        return "ERR_FUNC_INTERRUPTED";
    if (dtv_errno == ERR_XTAL_NOT_SUPPORT)
        return "ERR_XTAL_NOT_SUPPORT";
    if (dtv_errno == ERR_CANT_FIND_ORIG_TOPS)
        return "ERR_CANT_FIND_ORIG_TOPS";
    if (dtv_errno == ERR_INVALID_REG_VALUE)
        return "ERR_INVALID_REG_VALUE";
    if (dtv_errno == ERR_TUNER_NOT_SUPPORT)
        return "ERR_TUNER_NOT_SUPPORT";
    if (dtv_errno == ERR_UNDEFINED_SAW_BW)
        return "ERR_UNDEFINED_SAW_BW";
    if (dtv_errno == ERR_INVALID_CHIP_REV)
        return "ERR_INVALID_CHIP_REV";
    if (dtv_errno == ERR_WAIT_CONST_TIMEOUT)
        return "ERR_WAIT_CONST_TIMEOUT";
    if (dtv_errno == ERR_COUNTER_NOT_AVAILABLE)
        return "ERR_COUNTER_NOT_AVAILABLE";
    if (dtv_errno == ERR_LOADFW_COMP_FAIL)
        return "ERR_LOADFW_COMP_FAIL";
    if (dtv_errno == ERR_CANT_FIND_EEPROM)
        return "ERR_CANT_FIND_EEPROM";
    if (dtv_errno == ERR_TUNER_TYPE_NOT_SUPPORT)
        return "ERR_TUNER_TYPE_NOT_SUPPORT";
    if (dtv_errno == ERR_INV_MISC_REG)
        return "ERR_INV_MISC_REG";
    if (dtv_errno == ERR_CANT_FIND_USB_DEV)
        return "ERR_CANT_FIND_USB_DEV";
    if (dtv_errno == ERR_INVALID_XTAL_FREQ)
        return "ERR_INVALID_XTAL_FREQ";

//***********************************************************************
// Error Code of Parallel Module
//***********************************************************************
    if (dtv_errno == ERR_I2C_NO_ERROR)
        return "ERR_I2C_NO_ERROR";
    if (dtv_errno == ERR_I2C_NULL_HANDLE)
        return "ERR_I2C_NULL_HANDLE";
    if (dtv_errno == ERR_I2C_DONT_SUPPORT)
        return "ERR_I2C_DONT_SUPPORT";

//***********************************************************************
// Error Code of Common Module
//***********************************************************************
    if (dtv_errno == ERR_COM_NO_ERROR)
        return "ERR_COM_NO_ERROR";
    if (dtv_errno == ERR_COM_DATA_HIGH_FAIL)
        return "ERR_COM_DATA_HIGH_FAIL";
    if (dtv_errno == ERR_COM_CLK_HIGH_FAIL)
        return "ERR_COM_CLK_HIGH_FAIL";
    if (dtv_errno == ERR_COM_WRITE_NO_ACK)
        return "ERR_COM_WRITE_NO_ACK";
    if (dtv_errno == ERR_COM_DATA_LOW_FAIL)
        return "ERR_COM_DATA_LOW_FAIL";

//***********************************************************************
// Error Code of USB20 Module
//***********************************************************************
    if (dtv_errno == ERR_USB_NO_ERROR)
        return "ERR_USB_NO_ERROR";
    if (dtv_errno == ERR_USB_NULL_HANDLE)
        return "ERR_USB_NULL_HANDLE";
    if (dtv_errno == ERR_USB_WRITEFILE_FAIL)
        return "ERR_USB_WRITEFILE_FAIL";
    if (dtv_errno == ERR_USB_READFILE_FAIL)
        return "ERR_USB_READFILE_FAIL";
    if (dtv_errno == ERR_USB_INVALID_READ_SIZE)
        return "ERR_USB_INVALID_READ_SIZE";
    if (dtv_errno == ERR_USB_BAD_STATUS)
        return "ERR_USB_BAD_STATUS";
    if (dtv_errno == ERR_USB_INVALID_SN)
        return "ERR_USB_INVALID_SN";
    if (dtv_errno == ERR_USB_INVALID_PKT_SIZE)
        return "ERR_USB_INVALID_PKT_SIZE";
    if (dtv_errno == ERR_USB_INVALID_HEADER)
        return "ERR_USB_INVALID_HEADER";
    if (dtv_errno == ERR_USB_NO_IR_PKT)
        return "ERR_USB_NO_IR_PKT";
    if (dtv_errno == ERR_USB_INVALID_IR_PKT)
        return "ERR_USB_INVALID_IR_PKT";
    if (dtv_errno == ERR_USB_INVALID_DATA_LEN)
        return "ERR_USB_INVALID_DATA_LEN";

    if (dtv_errno == ERR_USB_EP4_READFILE_FAIL)
        return "ERR_USB_EP4_READFILE_FAIL";
    if (dtv_errno == ERR_USB_EP$_INVALID_READ_SIZE)
        return "ERR_USB_EP$_INVALID_READ_SIZE";

    if (dtv_errno == ERR_USB_BOOT_INVALID_PKT_TYPE)
        return "ERR_USB_BOOT_INVALID_PKT_TYPE";

    if (dtv_errno == ERR_USB_BOOT_BAD_CONFIG_HEADER)
        return "ERR_USB_BOOT_BAD_CONFIG_HEADER";
    if (dtv_errno == ERR_USB_BOOT_BAD_CONFIG_SIZE)
        return "ERR_USB_BOOT_BAD_CONFIG_SIZE";
    if (dtv_errno == ERR_USB_BOOT_BAD_CONFIG_SN)
        return "ERR_USB_BOOT_BAD_CONFIG_SN";
    if (dtv_errno == ERR_USB_BOOT_BAD_CONFIG_SUBTYPE)
        return "ERR_USB_BOOT_BAD_CONFIG_SUBTYPE";
    if (dtv_errno == ERR_USB_BOOT_BAD_CONFIG_VALUE)
        return "ERR_USB_BOOT_BAD_CONFIG_VALUE";
    if (dtv_errno == ERR_USB_BOOT_BAD_CONFIG_CHKSUM)
        return "ERR_USB_BOOT_BAD_CONFIG_CHKSUM";

    if (dtv_errno == ERR_USB_BOOT_BAD_CONFIRM_HEADER)
        return "ERR_USB_BOOT_BAD_CONFIRM_HEADER";
    if (dtv_errno == ERR_USB_BOOT_BAD_CONFIRM_SIZE)
        return "ERR_USB_BOOT_BAD_CONFIRM_SIZE";
    if (dtv_errno == ERR_USB_BOOT_BAD_CONFIRM_SN)
        return "ERR_USB_BOOT_BAD_CONFIRM_SN";
    if (dtv_errno == ERR_USB_BOOT_BAD_CONFIRM_SUBTYPE)
        return "ERR_USB_BOOT_BAD_CONFIRM_SUBTYPE";
    if (dtv_errno == ERR_USB_BOOT_BAD_CONFIRM_VALUE)
        return "ERR_USB_BOOT_BAD_CONFIRM_VALUE";
    if (dtv_errno == ERR_USB_BOOT_BAD_CONFIRM_CHKSUM)
        return "ERR_USB_BOOT_BAD_CONFIRM_CHKSUM";

    if (dtv_errno == ERR_USB_BOOT_BAD_BOOT_HEADER)
        return "ERR_USB_BOOT_BAD_BOOT_HEADER";
    if (dtv_errno == ERR_USB_BOOT_BAD_BOOT_SIZE)
        return "ERR_USB_BOOT_BAD_BOOT_SIZE";
    if (dtv_errno == ERR_USB_BOOT_BAD_BOOT_SN)
        return "ERR_USB_BOOT_BAD_BOOT_SN";
    if (dtv_errno == ERR_USB_BOOT_BAD_BOOT_PATTERN_01)
        return "ERR_USB_BOOT_BAD_BOOT_PATTERN_01";
    if (dtv_errno == ERR_USB_BOOT_BAD_BOOT_PATTERN_10)
        return "ERR_USB_BOOT_BAD_BOOT_PATTERN_10";
    if (dtv_errno == ERR_USB_BOOT_BAD_BOOT_CHKSUM)
        return "ERR_USB_BOOT_BAD_BOOT_CHKSUM";

    if (dtv_errno == ERR_USB_INVALID_BOOT_PKT_TYPE)
        return "ERR_USB_INVALID_BOOT_PKT_TYPE";
    if (dtv_errno == ERR_USB_BOOT_BAD_CONFIG_VAlUE)
        return "ERR_USB_BOOT_BAD_CONFIG_VAlUE";

    if (dtv_errno == ERR_USB_COINITIALIZEEX_FAIL)
        return "ERR_USB_COINITIALIZEEX_FAIL";
    if (dtv_errno == ERR_USB_COCREATEINSTANCE_FAIL)
        return "ERR_USB_COCREATEINSTANCE_FAIL";
    if (dtv_errno == ERR_USB_COCREATCLSEENUMERATOR_FAIL)
        return "ERR_USB_COCREATCLSEENUMERATOR_FAIL";
    if (dtv_errno == ERR_USB_QUERY_INTERFACE_FAIL)
        return "ERR_USB_QUERY_INTERFACE_FAIL";
    if (dtv_errno == ERR_USB_PKSCTRL_NULL)
        return "ERR_USB_PKSCTRL_NULL";
    if (dtv_errno == ERR_USB_INVALID_HANDLE)
        return "ERR_USB_INVALID_HANDLE";
    if (dtv_errno == ERR_USB_TOO_MUCH_WRITE_DATA)
        return "ERR_USB_TOO_MUCH_WRITE_DATA";
    if (dtv_errno == ERR_USB_NO_BURST_READ)
        return "ERR_USB_NO_BURST_READ";
    if (dtv_errno == ERR_USB_NULL_PENUM)
        return "ERR_USB_NULL_PENUM";

    return "<error not registered>";
}

