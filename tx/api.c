/**
 * Copyright (c) 2013 ITE Technologies, Inc. All rights reserved.
 * 
 * Date:
 *    2013/10/15
 *
 * Module Name:
 *    api.cpp
 *
 * Abstract:
 *    Defines the entry point for the DLL application.
 */

#include "api.h"

int g_hDriver[256] = {0};           // Support number of 256 devices.
int g_hSPIDriverHandle = 0;
int g_hPrimaDriverHandle = 0;

//===========================================================================
// The client calls this function to get SDIO/USB/SPI device handle.
// @param void
// Return:  int if successful, INVALID_HANDLE_VALUE indicates failure.
//===========================================================================
int g_ITEAPI_GetDriverHandle(Byte handleNum)
{
	int hDriver = 0, ret = 0;
	char* devName = "";
	
	ret = asprintf(&devName, "/dev/usb-it950x%d", handleNum);
    hDriver = open(devName, O_RDWR);

	if (hDriver > INVALID_HANDLE_VALUE)
		printf("\nOpen /dev/usb-it950x%d ok\n", handleNum);
	else
		printf("\nOpen /dev/usb-it950x%d fail\n", handleNum);

	return hDriver;
}


//===========================================================================
// The client calls this function to close device.
// @param   device handle
// Return:  TRUE indicates success. FALSE indicates failure. 
//===========================================================================
int g_ITEAPI_CloseDriverHandle(
    IN  int hObject)
{
    //here tell driver stop to read data
    //g_ITEAPI_StopCapture();
    return (close(hObject));
}

Dword g_ITEAPI_TxDeviceInit(
	IN Byte DevNo)
{
    Dword dwError = ERR_NO_ERROR;
    TxModDriverInfo DriverInfo;
	
    g_hDriver[DevNo] = g_ITEAPI_GetDriverHandle(DevNo);

    if (g_hDriver[DevNo] == INVALID_HANDLE_VALUE){
        dwError = -ERR_USB_INVALID_HANDLE;
		return dwError; 
	}

    //g_ITEAPI_TxPowerCtl(1, DevNo);

    /* Check driver is loaded correctly */
    dwError = g_ITEAPI_TxGetDrvInfo(&DriverInfo, DevNo);

	if(dwError == ERR_NO_ERROR)
		printf("g_ITEAPI_TxGetDrvInfo ok\n");
	else
		printf("g_ITEAPI_TxGetDrvInfo fail\n");


	return (dwError);
}

Dword g_ITEAPI_TxGetDeviceInfo(
	OUT PDEVICE_INFO pDeviceInfo,
	IN Byte DevNo)
{
    Dword dwError = ERR_NO_ERROR;
	Word ChipType = 0;    
	
	if((dwError = g_ITEAPI_TxGetChipType(&ChipType, DevNo)) == ERR_NO_ERROR)
		printf("g_ITE_TxGetChipType ok\n");
	else
		printf("g_ITE_TxGetChipType fail\n");	
		
	pDeviceInfo->ProductID = ChipType;
	
	return (dwError);
}

Dword g_ITEAPI_TxDeviceExit(
	IN Byte DevNo)
{
    Dword dwError = ERR_NO_ERROR;

    //g_ITEAPI_TxPowerCtl(0, DevNo);
    //g_ITEAPI_ResetPIDTable(DevNo);
    //g_ITEAPI_DisablePIDTbl(DevNo);
	//g_ITEAPI_TxSetModeEnable(0, DevNo);
    if (g_hDriver[DevNo])
        g_ITEAPI_CloseDriverHandle(g_hDriver[DevNo]);
	
    return (dwError);
}

Dword g_ITEAPI_TxPowerCtl(
    IN  Byte byCtrl,
    IN  Byte DevNo)
{
	
    Dword dwError = ERR_NO_ERROR;
    int result = 0;
    TxControlPowerSavingRequest request;

    if (g_hDriver[DevNo] > 0) {
        request.chip = 0;
        request.control = byCtrl;
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_CONTROLPOWERSAVING, (void *)&request);
        dwError = (Dword) request.error;
    }
    else {
        dwError = ERR_NOT_IMPLEMENTED;
    }
	
    return dwError;
}

Dword g_ITEAPI_TxSetChannelModulation(	
	IN MODULATION_PARAM ChannelModulation_Setting,
	IN Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxSetModuleRequest request;
	Byte transmissionMode = ChannelModulation_Setting.transmissionMode;
	Byte constellation = ChannelModulation_Setting.constellation;
	Byte interval = ChannelModulation_Setting.interval;
	Byte highCodeRate = ChannelModulation_Setting.highCodeRate;

	if (g_hDriver[DevNo] > 0) {
        request.chip = 0;
		request.transmissionMode = transmissionMode;
        request.constellation = constellation;
		request.interval = interval;
		request.highCodeRate = highCodeRate;
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_SETMODULE, (void *)&request);
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}

    return (dwError);
}

Dword g_ITEAPI_TxSetChannel(	
	IN Dword bfrequency,
	IN Word bandwidth,
	IN Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxAcquireChannelRequest request;

	if (g_hDriver[DevNo] > 0) {
        request.chip = 0;
		request.frequency = (__u32) bfrequency;
        request.bandwidth = bandwidth;
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_ACQUIRECHANNEL, (void *)&request);
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}

    return (dwError);
}

Dword g_ITEAPI_TxSetModeEnable(
	IN Byte OnOff, 
	IN Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxModeRequest request;
	
	if (g_hDriver[DevNo] > 0) {
        request.OnOff = OnOff;
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_ENABLETXMODE, (void *)&request);
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}

    return (dwError);
}

Dword g_ITEAPI_TxSetDeviceType(
	IN Byte DeviceType, 
	IN Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxSetDeviceTypeRequest request;

	if (g_hDriver[DevNo] > 0) {
		request.DeviceType = DeviceType;
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_SETDEVICETYPE, (void *)&request);
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}

    return (dwError);
}

Dword g_ITEAPI_TxGetDeviceType(
	OUT Byte *DeviceType,
	IN Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxGetDeviceTypeRequest request;

	if (g_hDriver[DevNo] > 0) {
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_GETDEVICETYPE, (void *)&request);
        *DeviceType = request.DeviceType;
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}

    return (dwError);
}

Dword g_ITEAPI_TxAdjustOutputGain(
	IN int Gain_value,
	OUT int *Out_Gain_value,
	IN Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxSetGainRequest request;
    
	if (g_hDriver[DevNo] > 0) {
		request.GainValue = Gain_value;
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_ADJUSTOUTPUTGAIN, (void *)&request);
        *Out_Gain_value = request.GainValue;
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}

    return (dwError);
}

Dword g_ITEAPI_TxControlPidFilter(
	Byte control,
	Byte enable,
	IN Byte DevNo)
{
    Dword dwError = ERR_NO_ERROR;
    int result;
    TxControlPidFilterRequest request;

    //RETAILMSG( 1, (TEXT("g_ITEAPI_EnablePIDTbl\n\r") ));

    if (g_hDriver[DevNo] > 0) {
        request.control = control;
        request.enable = enable;
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_CONTROLPIDFILTER, (void *)&request);
        dwError = (Dword) request.error;
    }
    else {
        dwError = ERR_NOT_IMPLEMENTED;
    }

    return(dwError);
}

Dword g_ITEAPI_TxAddPID(
    IN Byte byIndex,            // 0 ~ 31
	IN Word wProgId,            // pid number
	IN Byte DevNo)
{	
    Dword dwError = ERR_NO_ERROR;
    int result;
    AddPidAtRequest request;
    Pid pid;
	memset(&pid, 0, sizeof(pid));
    pid.value = (Word)wProgId;

    if (g_hDriver[DevNo] > 0) {
        request.chip = 0;
        request.pid = pid;
        request.index = byIndex;

        //RETAILMSG( 1, (TEXT("g_ITEAPI_AddPID - Index = %d, Value = %d\n\r"), request.index, request.pid.value));

        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_ADDPIDAT, (void *)&request);
        dwError = (Dword) request.error;
    }
    else {
        dwError = ERR_NOT_IMPLEMENTED;
    }

    return (dwError);
}

Dword g_ITEAPI_TxResetPidFilter(
	IN Byte DevNo)
{
    Dword dwError = ERR_NO_ERROR;
    int result;
    ResetPidRequest request;

    //RETAILMSG( 1, (TEXT("g_ITEAPI_ResetPIDTable\n\r")));

    if (g_hDriver[DevNo] > 0) {
        request.chip = 0;
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_RESETPID, (void *)&request);
        dwError = (Dword) request.error;
    }
    else {
        dwError = ERR_NOT_IMPLEMENTED;
    }

    return(dwError);
}

Dword g_ITEAPI_StartTransfer(
	IN Byte DevNo)
{

	Dword dwError = ERR_NO_ERROR;
    int result;

    //here tell driver begin to read data
    result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_STARTTRANSFER);

    return(dwError);
}

Dword g_ITEAPI_StartTransfer_CMD(
	IN Byte DevNo)
{

	Dword dwError = ERR_NO_ERROR;
    int result;

    //here tell driver begin to read data
    result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_STARTTRANSFER_CMD);

    return(dwError);
}

Dword g_ITEAPI_StopTransfer(
	IN Byte DevNo)
{
    Dword dwError = ERR_NO_ERROR;
    int result;

    result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_STOPTRANSFER);

    return(dwError);
}

Dword g_ITEAPI_StopTransfer_CMD(
	IN Byte DevNo)
{
    Dword dwError = ERR_NO_ERROR;
    int result;

    result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_STOPTRANSFER_CMD);

    return(dwError);
}

Dword g_ITEAPI_TxGetDrvInfo(
    OUT PTxModDriverInfo pDriverInfo,
    IN  Byte DevNo)
{
    Dword dwError = ERR_NO_ERROR;
    int result;

    if (g_hDriver[DevNo] > 0) {
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_GETDRIVERINFO, (void *)pDriverInfo);
        dwError = (Dword) pDriverInfo->error;
    }
    else {
        dwError = ERR_NOT_IMPLEMENTED;
    }

    return dwError;	
}

Dword g_ITEAPI_TxSendTSData(
    OUT Byte* pBuffer,
    IN  OUT Dword pdwBufferLength,
    IN  Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
	int Len = 0;
	
    if (g_hDriver[DevNo] > 0) {
		Len =  write(g_hDriver[DevNo], pBuffer, pdwBufferLength);
    }
    else {
        dwError = -ERR_NOT_IMPLEMENTED;
    }

    return Len;
}

Dword g_ITEAPI_TxAddPIDEx(
    IN  DTVPid pid,
    IN  Byte DevNo)
{	
    Dword dwError = ERR_NO_ERROR;
    int result;
    AddPidAtRequest request;

    if (g_hDriver[DevNo] > 0) {
        request.chip = 0;
        memcpy(&request.pid, &pid, sizeof(Pid));
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_ADDPIDAT, (void *)&request);
        dwError = (Dword) request.error;
    }
    else {
        dwError = ERR_NOT_IMPLEMENTED;
    }

    return (dwError);
}

Dword g_ITEAPI_TxReadRegOFDM(
    IN  Dword dwRegAddr,
    OUT Byte* pbyData,
    IN  Byte DevNo)
{
    Dword dwError = ERR_NO_ERROR;
    int result;
    TxReadRegistersRequest request;

    if (g_hDriver[DevNo] > 0) {
        request.chip = 0;
        request.processor = Processor_OFDM;
        request.registerAddress = (__u32) dwRegAddr;
        request.bufferLength = 1;
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_READREGISTERS, (void *)&request);
		*pbyData = request.buffer[0];
        dwError = (Dword) request.error;
    }
    else {
        dwError = ERR_NOT_IMPLEMENTED;
    }

    return dwError;
}

Dword g_ITEAPI_TxWriteRegOFDM(
    IN  Dword dwRegAddr,
    IN  Byte byData,
    IN  Byte DevNo)
{
    Dword dwError = ERR_NO_ERROR;
    int result;
    TxWriteRegistersRequest request;

    if (g_hDriver[DevNo] > 0) {
        request.chip = 0;
        request.processor = Processor_OFDM;
        request.registerAddress = (__u32) dwRegAddr;
        request.bufferLength = 1;
        memcpy (request.buffer, &byData, 1);
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_WRITEREGISTERS, (void *)&request);
        dwError = (Dword) request.error;
    } else {
        dwError = ERR_NOT_IMPLEMENTED;
    }

    return dwError;
}

Dword g_ITEAPI_TxReadRegLINK(
    IN  Dword dwRegAddr,
    OUT Byte* pbyData,
    IN  Byte  DevNo)
{
    Dword dwError = ERR_NO_ERROR;

    int result;
    TxReadRegistersRequest request;

    if (g_hDriver[DevNo] > 0) {
        request.chip = 0;
        request.processor = Processor_LINK;
        request.registerAddress = (__u32) dwRegAddr;
        request.bufferLength = 1;
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_READREGISTERS, (void *)&request);
		*pbyData = request.buffer[0];
        dwError = (Dword) request.error;
    }
    else {
        dwError = ERR_NOT_IMPLEMENTED;
    }

    return dwError;
}

Dword g_ITEAPI_TxWriteRegLINK(
    IN  Dword dwRegAddr,
    IN  Byte  byData,
    IN  Byte  DevNo)
{
    Dword dwError = ERR_NO_ERROR;
    int result;
    TxWriteRegistersRequest request;

    if (g_hDriver[DevNo] > 0) {
        request.chip = 0;
        request.processor = Processor_LINK;
        request.registerAddress = (__u32) dwRegAddr;
        request.bufferLength = 1;
        memcpy (request.buffer, &byData, 1);
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_WRITEREGISTERS, (void *)&request);
        dwError = (Dword) request.error;
    }
    else {
        dwError = ERR_NOT_IMPLEMENTED;
    }

    return dwError;
}

Dword g_ITEAPI_TxWriteEEPROM(
    IN  Word  wRegAddr,
    OUT Byte  byData,
    IN  Byte  DevNo)
{
    Dword dwError = ERR_NO_ERROR;
    int result;
    TxWriteEepromValuesRequest request;

    if (g_hDriver[DevNo] > 0) {
        request.chip = 0;
        request.registerAddress = wRegAddr;
        request.bufferLength = 1;
        memcpy (request.buffer, &byData, 1);
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_WRITEEEPROMVALUES, (void *)&request);
        dwError = (Dword) request.error;
    }
    else {
        dwError = ERR_NOT_IMPLEMENTED;
    }

    return dwError;
}

Dword g_ITEAPI_TxReadEEPROM(
    IN  Word  wRegAddr,
    OUT Byte* pbyData,
    IN  Byte  DevNo)
{
    Dword dwError = ERR_NO_ERROR;
    int result;
    TxReadEepromValuesRequest request;

    if (g_hDriver[DevNo] > 0) {
        request.chip = 0;
        request.registerAddress = wRegAddr;
        request.bufferLength = 1;
        result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_READEEPROMVALUES, (void *)&request);
		*pbyData = request.buffer[0];
        dwError = (Dword) request.error;
    }
    else {
        dwError = ERR_NOT_IMPLEMENTED;
    }

    return dwError;
}

Dword g_ITEAPI_TxGetGainRange(
	IN Dword frequency,
	IN Word bandwidth,
	OUT int *MaxGain,
	OUT int *MinGain,
	IN  Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxGetGainRangeRequest request;

	if (g_hDriver[DevNo] > 0) {
		if(frequency >= 173000 && frequency <= 9000000)
			request.frequency = frequency;
		else {
			printf("\nSet Frequency Out of Range!\n");
			dwError = ERR_FREQ_OUT_OF_RANGE;
		}
		if(bandwidth >= 2000 && bandwidth <= 8000)
			request.bandwidth = bandwidth;
		else {
			printf("\nSet Bandwidth Out of Range!\n");
			dwError = ERR_INVALID_BW;
		}
		if(!dwError) {
			result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_GETGAINRANGE, (void *)&request);
			*MaxGain = (int) request.maxGain;
			*MinGain = (int) request.minGain;
			dwError = (Dword) request.error;
		}
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}

    return (dwError);
}

Dword g_ITEAPI_TxGetTPS(
	OUT TPS *tps,
	IN Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxGetTPSRequest request; 

	if (g_hDriver[DevNo] > 0) {
		result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_GETTPS, (void *)&request);
		*tps = (TPS) request.tps;
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}

    return (dwError);
}

Dword g_ITEAPI_TxSetTPS(
	IN TPS tps,
	IN Byte DevNo)	
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxSetTPSRequest request; 
      
	if (g_hDriver[DevNo] > 0) {
		request.tps = tps;
		request.actualInfo = True;
		result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_SETTPS, (void *)&request);
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}

    return (dwError);
}

Dword g_ITEAPI_TxGetOutputGain(
	OUT int *gain,
	IN  Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxGetOutputGainRequest request; 
      
	if (g_hDriver[DevNo] > 0) {
		result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_GETOUTPUTGAIN, (void *)&request);
		*gain = request.gain;
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}

    return (dwError);
}

Dword g_ITEAPI_TxGetNumOfDevice(
	OUT Byte *NumOfDev)
{
	Dword dwError = ERR_NO_ERROR;
    Byte DevCount = 0;
    struct dirent *ptr;
    char *handle = "usb-it950x";
    char *existing;
    DIR *dir = opendir("/dev");
    
    while((ptr = readdir(dir)) != NULL) {
		existing = strndup(ptr->d_name, 10);
		if(!strcmp(existing, handle))
			DevCount++;
	}
	*NumOfDev = DevCount;
	closedir(dir);
    return (dwError);
}

Dword g_ITEAPI_TxSendCustomPacketOnce(
	IN int bufferSize,
	IN Byte *TableBuffer,
	IN Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxSendHwPSITableRequest request; 

	if(bufferSize != 188)
		return -1;

	if (g_hDriver[DevNo] > 0) {
		request.pbufferAddr = (__u32) TableBuffer;	
		result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_SENDHWPSITABLE, (void *)&request);
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}
    return (dwError);
}
 
Dword g_ITEAPI_TxSetPeridicCustomPacket(
	IN int bufferSize,
	IN Byte *TableBuffer,
	IN Byte index,
	IN Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxAccessFwPSITableRequest request; 

	if(bufferSize != 188)
		return -1;

	if (g_hDriver[DevNo] > 0) {
		request.psiTableIndex = index;
		request.pbufferAddr = (__u32) TableBuffer;
		result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_ACCESSFWPSITABLE, (void *)&request);
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}
    return (dwError);
}

Dword g_ITEAPI_TxSetPeridicCustomPacketTimer(
	IN Byte index,
	IN Word timer_interval,
	IN Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxSetFwPSITableTimerRequest request; 

	if (g_hDriver[DevNo] > 0) {
		request.psiTableIndex = index;
		request.timer = timer_interval;
		result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_SETFWPSITABLETIMER, (void *)&request);
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}
    return (dwError);
}

Dword g_ITEAPI_TxSetIQTable(
	IN Byte* ptrIQtable,
	IN Word	 IQtableSize,
	IN Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxSetIQTableRequest request; 

	if (g_hDriver[DevNo] > 0) {
		request.pIQtableAddr = (__u32) ptrIQtable;
		request.IQtableSize = IQtableSize;		
		result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_SETIQTABLE, (void *)&request);
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}
    return (dwError);
}

Dword g_ITEAPI_TxGetChipType(
	OUT Word* chipType,
	IN Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxGetChipTypeRequest request; 

	if (g_hDriver[DevNo] > 0) {
		result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_GETCHIPTYPE, (void *)&request);
		*chipType = request.chipType;
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}
    return (dwError);
}

Dword g_ITEAPI_TxSetDCCalibrationValue(
	IN int dc_i,
	IN int dc_q,
	IN Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxSetDCCalibrationValueRequest request; 

	if (g_hDriver[DevNo] > 0) {
		request.dc_i = dc_i;
		request.dc_q = dc_q;
		result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_SETDCCALIBRATIONVALUE, (void *)&request);
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}
    return (dwError);
}

Dword g_ITEAPI_TxSetLowBitRateTransferRequest(
	IN Bool isUseLowBrate,
	IN Byte DevNo)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxSetLowBitRateTransferRequest request; 

	if (g_hDriver[DevNo] > 0) {
		request.isUseLowBrate = isUseLowBrate;
		result = ioctl(g_hDriver[DevNo], IOCTL_ITE_MOD_SETLOWBRATETRANS, (void *)&request);
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}
    return (dwError);
}

Dword g_ITEAPI_TxWriteCmd(
	IN Word			len,
    IN Byte*			cmd
)
{
	Dword dwError = ERR_NO_ERROR;
    int result;
    TxCmdRequest request; 
	
	if (g_hDriver[0] > 0) {
		request.len = len;
		request.cmdAddr = (__u32) cmd;

		result = ioctl(g_hDriver[0], IOCTL_ITE_MOD_WRITECMD, (void *)&request);
        dwError = (Dword) request.error;
	}
	else {
        dwError = ERR_NOT_IMPLEMENTED;
	}
    return (dwError);
}
