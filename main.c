/*****************************************************************************/
/*  Автор         		: Дракончик				                             */
/*  Приставка     		: Sony PlayStation 2					             */
/*  ОС			  		: OSDSYS											 */
/*  Язык	      		: GNU C                                              */
/*                                                                           */
/*  Содержимое файла	: Главный Cи файл			                         */
/*  Атрибут файла		: SOURCE                                             */
/*  Имя файла     		: main.c	                                         */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>
#include <malloc.h>
#include <gsKit.h>
#include <dmaKit.h>
#include <gsToolkit.h>
#include <iopcontrol.h> 
#include <sbv_patches.h>
#include <assert.h>

#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <string.h>
#include <stdbool.h>
#include <ps2_all_drivers.h>

#include <unistd.h>


#include "Data/include/introState.h"
#include "Data/include/menuState.h"
#include "Data/include/gameState.h"
#include "Data/include/overState.h"


#include "Data/include/pad.h"

#define IMPORT_BIN2C(_n) \
    extern unsigned char _n[];   \
    extern int size_##_n

IMPORT_BIN2C(padman_irx);
IMPORT_BIN2C(libsd_irx);
IMPORT_BIN2C(audsrv_irx);

char relativePath[];

StateMachine GameMachineState;

char *dirname(char *path)
{
    static char buffer[260];
    size_t len;
    if (path==NULL) {
        strcpy(buffer, ".");
        return buffer;
    }
    len = strlen(path);
    assert(len<sizeof(buffer));
    if (len!=0 && (path[len-1]=='/' || path[len-1]=='\\')) {
        --len;
    }
    while (len!=0 && path[len-1]!='/' && path[len-1]!='\\') {
        --len;
    }
    if (len==0) {
        strcpy(buffer, ".");
    } else if (len==1) {
        if (path[0]=='/' || path[0]=='\\') {
            strcpy(buffer, "/");
        } else {
            strcpy(buffer, ".");
        }
    } else {
        memcpy(buffer, path, len-1);
    }
    return buffer;
}

void reset_iop()
{
    SifInitRpc(0);
    // Reset IOP
    while (!SifIopReset("", 0x0))
        ;
    while (!SifIopSync())
        ;
    SifInitRpc(0);

    sbv_patch_enable_lmb();
}

static void prepare_drivers() 
{
    //init_fileXio_driver();
    //init_memcard_driver(true);
    init_usb_driver(true);
    //init_cdfs_driver();
   	//init_joystick_driver(true);
    init_audio_driver();
    //init_poweroff_driver();
    //init_hdd_driver(true, true);
}


int main(int argc, char* argv[])
{

	
	reset_iop();
	
	SifExecModuleBuffer(padman_irx, size_padman_irx, 0, NULL, NULL);
	SifExecModuleBuffer(libsd_irx, size_libsd_irx, 0, NULL, NULL);
	//SifExecModuleBuffer(audsrv_irx, size_audsrv_irx, 0, NULL, NULL);
	prepare_drivers();
	
	// Initialize GamePad
	PadInitialize();
	
	strcpy(relativePath ,dirname(argv[0]));
	
	u64 Black = GS_SETREG_RGBAQ(0x00,0x00,0x00,0x00,0x00);
	
	u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
	
	GSGLOBAL *gsGlobal = gsKit_init_global();
	
	// GS_MODE_PAL_I
	// GS_MODE_VGA_640_60
	
	// Check PAL or NTSC (Code by Daniel Santos)
	gsGlobal->PSM = GS_PSM_CT24;
	gsGlobal->PSMZ = GS_PSMZ_16S;
	
	gsGlobal->Mode = gsKit_check_rom();
	
	if(gsGlobal->Mode == GS_MODE_PAL)
	{
		gsGlobal->Height = 512;
	}
	else
	{ 
		gsGlobal->Height = 448;
	}
	
	dmaKit_init(D_CTRL_RELE_OFF,D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
		    D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

	// Initialize the DMAC
	dmaKit_chan_init(DMA_CHANNEL_GIF);

	gsKit_init_screen(gsGlobal);
	
	gsGlobal->PrimAlphaEnable = GS_SETTING_ON;

	// Sets up the mode for drawing.
	// GS_ONESHOT-> every draw clears the draw queue
	// GS_PERSISTENT-> gskit stores a draw queue and whenever you draw again, it just adds onto that queue
	gsKit_mode_switch(gsGlobal, GS_ONESHOT);

	gsKit_clear(gsGlobal, Black);

	StateMachineStart(&GameMachineState, &IntroState, gsGlobal);
	
	while(1)
	{		
		// Update GamePads
		UpdatePad();
		
		StateMachineUpdate(&GameMachineState, gsGlobal);
		
		StateMachineDraw(&GameMachineState, gsGlobal, TexCol);
		
		gsKit_queue_exec(gsGlobal);
		
		gsKit_sync_flip(gsGlobal);
			
		//gsKit_clear(gsGlobal, Black);
	}
	
	sleep(60); // sleep before boing bai bai
	
	return 0;
}

