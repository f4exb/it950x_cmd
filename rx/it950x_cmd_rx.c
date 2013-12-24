//
// This is a sample testkit for demodulator device.
//

#include "api.h"
#include "options.h"

#include <signal.h>


#define ERROR(x...)												\
	do {														\
		fprintf(stderr, "ERROR: ");								\
		fprintf(stderr, x);										\
		fprintf (stderr, "\n");									\
	} while (0)

#define PERROR(x...)											\
	do {														\
		fprintf(stderr, "ERROR: ");								\
		fprintf(stderr, x);										\
		fprintf (stderr, " (%s)\n", strerror(errno));			\
	} while (0)


typedef struct {
	char *name;
	int value;
} Param;

typedef struct {
	uint32_t count;
	uint32_t ulErrCount;
	uint32_t ulLostCount;
	uint16_t pid;
	uint8_t  sequence1;
	uint8_t  sequence2;
	uint8_t  dup_flag;
} PIDINFO, *PPIDINFO;

static int gDoExit = 0;
int gChoseBand;
int gFreq;
uint32_t gTransferInterval = 0;

//==================================================================================================
static void sighandler(int signum)
{
    fprintf(stderr, "Signal %d caught, exiting!\n", signum);
    gDoExit = 1;
}

//==================================================================================================
static int GetDriverInfo()
{
    int res = 0;
    DemodDriverInfo driverInfo;
    Dword dwError = ERR_NO_ERROR;

    dwError = DTV_GetVersion(&driverInfo);
    
    if (dwError) 
    {
        fprintf(stderr, "\nGet Driver Info failed 0x%X!\n", (unsigned int)dwError);
        res = -1;
    }
    else 
    {
        fprintf(stderr, "DriverVerion  = %s\n", driverInfo.DriverVerion);
        fprintf(stderr, "APIVerion     = %s\n", driverInfo.APIVerion);
        fprintf(stderr, "FWVerionLink  = %s\n", driverInfo.FWVerionLink);
        fprintf(stderr, "FWVerionOFDM  = %s\n", driverInfo.FWVerionOFDM);
        fprintf(stderr, "Company       = %s\n", driverInfo.Company);
        fprintf(stderr, "SupportHWInfo = %s\n", driverInfo.SupportHWInfo);
    }

    return res;
}

//==================================================================================================
static int ChannelLock(options_t *options)
{
    Dword dwFrequency = 0;
    Word  wBandwidth = 0;
    Dword dwStatus = 0;
    Bool boolIsLock = 0;
    int res;

    dwFrequency = options->frequency;
    gFreq = options->frequency;
    gChoseBand = options->bandwidth;

    fprintf(stderr, "\n");

    if(gChoseBand>=2000 && gChoseBand<3000) wBandwidth = 2;
    else if(gChoseBand>=3000 && gChoseBand<4000) wBandwidth = 3;
    else if(gChoseBand>=4000 && gChoseBand<5000) wBandwidth = 4;
    else if(gChoseBand>=5000 && gChoseBand<6000) wBandwidth = 5;
    else if(gChoseBand>=6000 && gChoseBand<7000) wBandwidth = 6;
    else if(gChoseBand>=7000 && gChoseBand<8000) wBandwidth = 7;
    else if(gChoseBand>=8000 && gChoseBand<9000) wBandwidth = 8;
    else 
    {
        fprintf(stderr, "No Supported Bandwidth!\n"); 
        return ERR_INVALID_BW;
    }

    fprintf(stderr, "Trying to lock:\n");
    fprintf(stderr, "Frequency ...: %lu KHz\n", dwFrequency);
    fprintf(stderr, "Bandwidth ...: %i MHz\n\n", wBandwidth);

    dwStatus = DTV_AcquireChannel(dwFrequency, wBandwidth);
    
    if (dwStatus)
    {
        fprintf(stderr, "DTV_AcquireChannel() returned error %s (%lx)\n", DTV_Error(dwStatus), dwStatus);
    }

    dwStatus = DTV_DisablePIDTbl();
    
    if (dwStatus)
    {
        fprintf(stderr, "DTV_DisablePIDTbl() returned error %s (%lx)\n", DTV_Error(dwStatus), dwStatus);
    }

    dwStatus = DTV_IsLocked((Bool *)&boolIsLock);
    
    if (dwStatus)
    {
        fprintf(stderr, "DTV_IsLocked() returned error %s (%lx)\n", DTV_Error(dwStatus), dwStatus);
    }

    if (boolIsLock)
    {
        fprintf(stderr, "*** Channel Locked ***\n");
    }
    else
    {
        fprintf(stderr, "*** Channel Unlocked ***\n");
    }

    return 0;
}

//==================================================================================================
static int MultiChannelTest()
{
	Dword dwFrequency = 0;
    Word  wBandwidth = 0;
	Dword dwStatus = 0;
	Bool boolIsLock = 0;
	Dword dwpPostErrCnt;    // ErrorBitCount
	Dword dwpPostBitCnt;    // TotalBitCount
    Word  wpAbortCnt;       // number of abort RSD packet
    Word  wpQuality;        // signal quality (0 - 100)
    Word  wpStrength;		// signal strength (0 - 100)
	DTVStatistic StatisicEx;
	int startFreq, stopFreq;
	int channels = 0, res;
	FILE* pFile = NULL;

	pFile = fopen("scanlog.txt", "w");

	printf("\n=> Please Input Start Frequency (KHz): ");
	res = scanf("%d", &startFreq);
	
	printf("\n=> Please Input Stop Frequency (KHz): ");
	res = scanf("%d", &stopFreq);
	
	printf("\n=> Please Choose Bandwidth (0:8MHz  1:7MHz  2:6MHz): ");
	res = scanf("%d",&gChoseBand);

	if (gChoseBand == 0)
		wBandwidth = 8;
	else if (gChoseBand == 1)
		wBandwidth = 7;
	else
		wBandwidth = 6;

	gFreq = startFreq;

	while (gFreq <= stopFreq)
	{
		dwFrequency = gFreq;

		dwStatus = DTV_AcquireChannel(dwFrequency, wBandwidth);
		if (dwStatus)
			printf("DTV_AcquireChannel() return error!\n");

		usleep(2500000);

		dwStatus =  DTV_GetStatistic(10000,
										(Dword *)&dwpPostErrCnt,
										(Dword *)&dwpPostBitCnt,
										(Word *)&wpAbortCnt,
										(Word *)&wpQuality,
										(Word *)&wpStrength);

		if (dwStatus)
			printf("DTV_GetStatistic() return error!\n");

		dwStatus =  DTV_GetStatisticEx(&StatisicEx);
		if (dwStatus)
			printf("DTV_GetStatisticEx() return error!\n");

		dwStatus = DTV_IsLocked((Bool *)&boolIsLock);
		if (dwStatus)
			printf("DTV_IsLocked() return error!\n");

		if (boolIsLock) {
			channels ++;
			printf("\n======  Statistics   ======\n");
			printf("Frequency = %i KHz\n", gFreq);
			printf("Demod locked: %d\n", StatisicEx.signalLocked);
			printf("TPS locked :%d\n", StatisicEx.signalPresented);
			printf("Quality: %d\n", wpQuality);
			printf("Strength: %d\n", wpStrength);
			if (dwpPostBitCnt != 0)
				printf("BER: %3.3e\n", ((float)dwpPostErrCnt)/dwpPostBitCnt);
			else
				printf("BER: %3.3e\n", (double) dwpPostBitCnt);
			printf("Abort Count: %d\n", wpAbortCnt);

			printf("===============================\n");

			if (pFile) {
				fprintf(pFile, "\n======  Statistics   ======\n");
				fprintf(pFile, "Frequency = %i KHz\n", gFreq);
				fprintf(pFile, "Demod locked: %d\n", StatisicEx.signalLocked);
				fprintf(pFile, "TPS locked :%d\n", StatisicEx.signalPresented);
				fprintf(pFile, "Quality: %d\n", wpQuality);
				fprintf(pFile, "Strength: %d\n", wpStrength);
				if (dwpPostBitCnt != 0)
					fprintf(pFile, "BER: %3.3e\n", ((float)dwpPostErrCnt)/dwpPostBitCnt);
				else
					fprintf(pFile, "BER: %3.3e\n", (double) dwpPostBitCnt);
				fprintf(pFile, "Abort Count: %d\n", wpAbortCnt);

				fprintf(pFile, "===============================\n");
			}
		}

		if (gChoseBand == 0)
			gFreq = gFreq + 8000;
		else if (gChoseBand == 1)
			gFreq = gFreq + 7000;
		else
			gFreq = gFreq + 6000;

	}

	printf("\n============ Analysis ============");
	printf("\n Frequency %d(KHz) ~  %d(KHz)", startFreq, stopFreq);
	printf("\n Scan Channel : %d channels Locked  \n", channels);

	if (pFile) {
		fprintf(pFile, "\n============ Analysis ============");
		fprintf(pFile, "\n Frequency %d(KHz) ~  %d(KHz)", startFreq, stopFreq);
		fprintf(pFile, "\n Scan Channel : %d channels Locked  \n", channels);
		fclose(pFile);
		printf("\n Save log to file scanlog.txt\n\n");
	}

	return 0;
}

//==================================================================================================
static int ChannelStatisticTest(options_t *options)
{
    Dword dwpPostErrCnt;    // ErrorBitCount
    Dword dwpPostBitCnt;    // TotalBitCount
    Word  wpAbortCnt;       // number of abort RSD packet
    Word  wpQuality;        // signal quality (0 - 100)
    Word  wpStrength;       // signal strength (0 - 100)
    Dword dwStatus;
    DTVStatistic StatisicEx;
    int res;
    int i = 0;
    FILE *pFile = NULL;

    do 
    {
        dwStatus =  DTV_GetStatistic(10000,
                                        (Dword *)&dwpPostErrCnt,
                                        (Dword *)&dwpPostBitCnt,
                                        (Word *)&wpAbortCnt,
                                        (Word *)&wpQuality,
                                        (Word *)&wpStrength);

        if (dwStatus)
        {
            fprintf(stderr, "DTV_GetStatistic() return error!\n");
        }

        dwStatus =  DTV_GetStatisticEx(&StatisicEx);
        
        if (dwStatus)
        {
            fprintf(stderr, "DTV_GetStatisticEx() return error!\n");
        }
        
        fprintf(stderr, "\n*******************************\n*** Channel Statistics: ***\n");

        fprintf(stderr, "Demod locked: %d\n", StatisicEx.signalLocked);
        fprintf(stderr, "TPS locked :%d\n", StatisicEx.signalPresented);
        fprintf(stderr, "Quality: %d\n", wpQuality);
        fprintf(stderr, "Strength: %d\n", wpStrength);
        
        if (dwpPostBitCnt != 0)
        {
            fprintf(stderr, "BER: %3.3e\n", ((float)dwpPostErrCnt)/dwpPostBitCnt);
        }
        else
        {
            fprintf(stderr, "BER: %3.3e\n", (double) dwpPostBitCnt);
        }
        
        fprintf(stderr, "Abort Count: %d\n", wpAbortCnt);

        if (options->stat_filename)
        {
            pFile = fopen(options->stat_filename, "w");
            
            fprintf(pFile, "\n*******************************\n*** Channel Statistics: ***\n");

            fprintf(pFile, "Demod locked: %d\n", StatisicEx.signalLocked);
            fprintf(pFile, "TPS locked :%d\n", StatisicEx.signalPresented);
            fprintf(pFile, "Quality: %d\n", wpQuality);
            fprintf(pFile, "Strength: %d\n", wpStrength);
            
            if (dwpPostBitCnt != 0)
            {
                fprintf(pFile, "BER: %3.3e\n", ((float)dwpPostErrCnt)/dwpPostBitCnt);
            }
            else
            {
                fprintf(pFile, "BER: %3.3e\n", (double) dwpPostBitCnt);
            }
            
            fprintf(pFile, "Abort Count: %d\n", wpAbortCnt);

            fprintf(pFile, "*******************************\n");
            fprintf(pFile, "\n");
        }

        usleep(1000000);
        i++;
    } while (((i < options->stat_count) || (options->stat_count == 0)) && !gDoExit);

    if (pFile)
    {
        fclose(pFile);
    }

    return 0;
}


#define BSIZE 188
#define BBLOCK 64

PIDINFO pidt[0x2001];

//==================================================================================================
int dvbtraffic(options_t *options)
{
    int tempCount = 0;
    int packets = 0;
    int infinite_loop = 0;
    struct timeval startt, startt2;
    char *search;
    unsigned char buffer[BSIZE];
    int i = 0, j = 0;
    uint8_t seq;
    uint8_t adaptationField;
    uint8_t checkBit;
    FILE *pFile = NULL;

    if (options->ts_filename) 
    {
        if (strcmp("-", options->ts_filename) == 0)
        {
            pFile = stdout;
        }
        else
        {
            pFile = fopen(options->ts_filename, "wb");
        }
    }

    for (j = 0; j < 0x2001; j++) 
    {
        pidt[j].count = 0;
        pidt[j].ulLostCount = 0;
        pidt[j].pid = 0;
        pidt[j].sequence1 = 0xFF;
        pidt[j].sequence2 = 0xFF;
    }

    DTV_StartCapture();

    //usleep(2000000);

    gettimeofday(&startt, 0);
    startt2 = startt;

    //if (argc > 1)
    //	search = argv[1];
    //else
        search = 0;

    j = 0;
    
    if (options->packet_limit == 0)
    {
        infinite_loop = 1; // for Burn in test
    }

    while (!gDoExit && (infinite_loop || (i < options->packet_limit))) 
    {
        int pid, ok;
        Dword r;
        r = 188;
        DTV_GetData(buffer, &r);
        //printf("DTV_GetData - %d\n", r);
        if (r <= 0) 
        {
            //printf("read%d", j);
            usleep(100000);
            j++;
            
            if (j>100) // 10 sec
            {
                break;
            }
            
            continue;
        }
        
        j = 0;
        
        if (r != 188) 
        {
            fprintf(stderr, "only read %lu\n", r);
            break;
        }
        
        if (buffer[0] != 0x47) 
        {
            //continue;
            fprintf(stderr, "desync (%x, %x, %x, %x) - %lu\n", buffer[0], buffer[1], buffer[2], buffer[3], r);
            
            while (buffer[0] != 0x47) 
            {
                r = 1;
                DTV_GetData(buffer, &r);
            }
            
            r = 187;
            DTV_GetData(buffer, &r);
            continue;
            tempCount++;
        }
            //printf("tempCount %d\n", tempCount);
        ok = 1;
        pid = ((((unsigned) buffer[1]) << 8) |
               ((unsigned) buffer[2])) & 0x1FFF;

        adaptationField = (buffer[3]&0x30) >> 4;
        seq = buffer[3] & 0xf;

        if (search) 
        {
            int i, sl = strlen(search);
            ok = 0;
            
            if (pid != 0x1fff) 
            {
                for (i = 0; i < (188 - sl); ++i)
                {
                    if (!memcmp(buffer + i, search, sl))
                    {
                        ok = 1;
                    }
                }
            }
        }

        if (pid == 0x1FFF) 
        {
            //printf("\nSave %d KB...\n", packets*188/1024);
            continue;
        }

        if (ok) 
        {
            pidt[pid].count++;

            if (pidt[pid].sequence1 == 0xFF) 
            {
                //the first packet arrives, nothing to do
                pidt[pid].sequence1 = seq;
            }
            else 
            {
                if (adaptationField==0 || adaptationField==2) 
                {
                    //No increment(lost) check for type 0 & 2 
                }
                else 
                {
                    //Increment(lost) check for type 1 & 3
                    if ((pidt[pid].sequence1==seq) && (pidt[pid].dup_flag==0)) 
                    {
                        // Duplicate packets may be sent as two, and only two, consecutive Transport Stream packets of the same PID
                        pidt[pid].dup_flag = 1; // No more duplicate allowed
                    }
                    else 
                    {
                        if (pidt[pid].sequence1+1 == seq) 
                        {
                            //no packet loss, expected packet arrives 
                            if (pidt[pid].count == 100000)
                            {
                                fprintf(stderr, "%d", pidt[pid].ulLostCount);
                            }
                        }
                        else 
                        {
                            if (pidt[pid].sequence1+1 < seq) 
                            {
                                pidt[pid].ulLostCount += seq - (pidt[pid].sequence1+1);
                            }
                            else 
                            {
                                pidt[pid].ulLostCount += (seq+0x10) - (pidt[pid].sequence1+1);
                            }
                        }
                        
                        pidt[pid].dup_flag = 0; //next duplicate allowed
                        pidt[pid].sequence1 = seq;
                    }
                }
            }
                    
            //Check if TEI error  
            if (checkBit != 0x00)
            {
                pidt[pid].ulErrCount++;
            }
        }

        packets++;

        if (!(packets & 0xFF)) 
        {
            struct timeval now;
            uint32_t diff, diff2;
            gettimeofday(&now, 0);
            diff2 = (now.tv_sec - startt2.tv_sec) * 1000 +
                (now.tv_usec - startt2.tv_usec) / 1000;
                
            if (diff2 > 1000) 
            {
                int n = 0;
                diff = (now.tv_sec - startt.tv_sec) * 1000 +
                    (now.tv_usec - startt.tv_usec) / 1000;
                    
                fprintf(stderr, "\n-PID-------Total----Lost----Error---Mbps-\n");
                
                for (n = 0; n < 0x2001; n++) 
                {
                    if (pidt[n].count) 
                    {
                        fprintf(stderr, "%04x %10d %7d %7d %8d\n",
                                n,
                                pidt[n].count,
                                pidt[n].ulLostCount,
                                pidt[n].ulErrCount,
                                pidt[n].count * 188 / 1000 * 8 * 1000 / diff);
                    }
                }
                
                startt2 = now;
                
                if(i >= 0) 
                {
                    i++;
                }

                if (options->ts_filename) 
                {
                    fprintf(stderr, "\nSave %d KB...\n", packets*188/1024);
                }
            }
        }

        if (pFile) 
        {
            i = (packets*188) / 1024 / 1024; // MB
            fwrite(buffer, 1, sizeof(buffer), pFile);
        }
    }

    DTV_StopCapture();

    if (pFile) 
    {
        fclose(pFile);
        fprintf(stderr, "\n*** Save File to %s (%dM) ***\n", options->ts_filename, i);
    }

    return 0;
}

//==================================================================================================
int dvbout(options_t *options)
{
    int tempCount = 0;
    int packets = 0;
    int infinite_loop = 0;
    Byte buffer[BSIZE*BBLOCK];
    int i = 0, j = 0;
    FILE *pFile = NULL;

    if (options->ts_filename) 
    {
        if (strcmp("-", options->ts_filename) == 0)
        {
            pFile = stdout;
        }
        else
        {
            pFile = fopen(options->ts_filename, "wb");
        }
    }

    DTV_StartCapture();

    usleep(2000000);

    j = 0;
    
    if (options->packet_limit == 0)
    {
        infinite_loop = 1; // for Burn in test
    }

    while (!gDoExit && (infinite_loop || (i < options->packet_limit))) 
    {
        int pid, ok;
        Dword r;
        r = 188*BBLOCK;

        DTV_GetData(buffer, &r);
        //printf("DTV_GetData - %d\n", r);
        
        if (r <= 0) 
        {
			if (options->verbose)
			{
				fprintf(stderr, "read %d (%lu, %d) \n", j, r, packets);
			}
            usleep(100000); // was 100 ms
            j++;
            
            if (j>10) // was 10 sec
            {
                fprintf(stderr, "Read timeout\n", r);
                
                if (options->relock)
                {
                    if (ChannelLock(options) < 0) 
                    {
                        fprintf(stderr, "Re-lock channel fail!\n");
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            
            continue;
        }
        
        j = 0;
        
        if (r != 188*BBLOCK) 
        {
            fprintf(stderr, "only read %lu\n", r);
            break;
        }
        
        packets++;

        if (pFile) 
        {
            i = (packets*188*2) / 1024 / 1024; // MB
            fwrite(buffer, 1, sizeof(buffer), pFile);
        }
    }

    DTV_StopCapture();

    if (pFile) 
    {
        fclose(pFile);
        fprintf(stderr, "\n*** Save File to %s (%dM) ***\n", options->ts_filename, i);
    }

    return 0;
}

//==================================================================================================
static int RWRegister()
{
	int res = 0;
	int rwFlag = 0, processor = 0;
	uint32_t regAddr = 0, vaule = 0;
	Dword dwStatus;

	printf("\n=> Please Choose Read/Write Register (0: Read, 1: Write): ");
	res = scanf("%d", &rwFlag);

	printf("\n=> Please Choose LINK or OFDM (0: LINK, 1: OFDM): ");
	res = scanf("%d", &processor);

	printf("\n=> Please Enter Read/Write Register Address (Hex): ");
	res = scanf("%x", &regAddr);

	if (rwFlag == 1) { // Write register
		printf("\n=> Please Enter Write Value (Hex): ");
		res = scanf("%x", &vaule);

		if (processor == 0) {
			dwStatus = DTV_WriteRegLINK((Word)regAddr, (Byte)vaule);
			if (dwStatus)
				printf("DTV_WriteRegLINK() return error!\n");
			else
				printf("\nWrite LINK Address 0x%X Value 0x%X OK.\n", regAddr, vaule);
		}
		else {
			dwStatus = DTV_WriteRegOFDM((Word)regAddr, (Byte)vaule);
			if (dwStatus)
				printf("DTV_WriteRegOFDM() return error!\n");
			else
				printf("\nWrite OFDM Address 0x%X Value 0x%X OK.\n", regAddr, vaule);
		}
	}
	else { // Read register
		if (processor == 0) {
			dwStatus = DTV_ReadRegLINK((Word)regAddr, (Byte*) &vaule);
			if (dwStatus)
				printf("DTV_ReadRegLINK() return error 0x%lX!\n", dwStatus);
			else
				printf("\nRead LINK Address 0x%X Value 0x%X OK.\n", regAddr, vaule);
		}
		else {
			dwStatus = DTV_ReadRegOFDM((Word)regAddr, (Byte*) &vaule);
			if (dwStatus)
				printf("DTV_ReadRegOFDM() return error!\n");
			else
				printf("\nRead OFDM Address 0x%X Value 0x%X OK.\n", regAddr, vaule);
		}
	}
	return res;
}

//==================================================================================================
int main(int argc, char **argv)
{
    struct sigaction sigact;
    int chose, res;
    int closeFlag = 0;
    struct timeval;
    Byte handleNum;
    options_t options;

    get_options(argc, argv, &options);
    print_options(&options);
    
    if (DTV_Initialize(options.dev_handle) == ERR_INVALID_DEV_TYPE)
    {
        return 0;
    }

    sigact.sa_handler = sighandler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGINT, &sigact, NULL);
    sigaction(SIGTERM, &sigact, NULL);
    sigaction(SIGQUIT, &sigact, NULL);
    sigaction(SIGPIPE, &sigact, NULL);
    
    fprintf(stderr, "\n========== ITEtech Linux DTV receiver control program ==========\n");
    GetDriverInfo();
    fprintf(stderr, "================================================================\n");

    
    if (ChannelLock(&options) < 0) 
    {
        fprintf(stderr, "Lock channel fail!\n");
    }
    else
    {
        if (options.stat_count >= 0)
        {
            ChannelStatisticTest(&options);
        }
        else 
        {
            if (options.ts_filename)
            {
                dvbout(&options);
            }
            else
            {
                dvbtraffic(&options);
            }
        }
    }

    DTV_Finalize();

    return 0;
}

