/*   ITU-T G.729 Annex I Software Package Release 2 (November 2006) */
/*
   ITU-T G.729 Annex I  - Reference C code for fixed point
                         implementation of G.729 Annex I
   Version 1.2    Last modified: October 2006 
*/
/*
----------------------------------------------------------------------
                    COPYRIGHT NOTICE
----------------------------------------------------------------------
   ITU-T G.729 Annex I fixed point ANSI C source code
   Copyright (C) 1999, AT&T, France Telecom, NTT, University of
   Sherbrooke, Conexant, Ericsson. All rights reserved.
----------------------------------------------------------------------
*/

/*
 File : CODERI.C
 */
/* from codere.c G.729 Annex E Version 1.2  Last modified: May 1998 */
/* from coderd.c G.729 Annex D Version 1.2  Last modified: May 1998 */
/* from coder.c G.729 Annex B Version 1.3  Last modified: August 1997 */
/* from coder.c G.729 Version 3.3  */

/*--------------------------------------------------------------------------------------*
 * Main program of the ITU-T G.729 annex I   11.8/8/6.4 kbit/s encoder.
 *
 *    Usage : coderi speech_file  bitstream_file  DTX_flag [bit_rate or file_bit_rate]
 *--------------------------------------------------------------------------------------*/

/* ------------------------------------------------------------------------ */
/*                            MAIN PROGRAM                                  */
/* ------------------------------------------------------------------------ */

/*
 8K采样率 一帧80bit 10ms
 */

#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "basic_op.h"
#include "ld8k.h"
#include "ld8cp.h"
#include "dtx.h"
#include "octet.h"
#if defined(__BORLANDC__)
extern unsigned _stklen = 48000U;
#endif

/*#define SYNC*/
#define PREPROC

int encoder_main(int argc, const char *argv[] )
{
    FILE *f_speech;               /* File of speech data                   */
    FILE *f_serial;               /* File of serial bits for transmission  */
    FILE  *f_rate;
    Word16 rate;
    
    extern Word16 *new_speech;     /* Pointer to new speech data            */
    
    Word16 prm[PRM_SIZE_D+2];          /* Analysis parameters.                  */
    unsigned char serial[SERIAL_SIZE_E];    /* Output bitstream buffer               */
    
    Word16 i, frame;               /* frame counter */
    Word32 count_frame;
    
    Word16 nb_words, dtx_enable;
    
    if ( (f_speech = fopen(argv[1], "rb")) == NULL) {
        printf("%s - Error opening file  %s !!\n", argv[0], argv[1]);
        exit(0);
    }
    
    if ( (f_serial = fopen(argv[2], "wb")) == NULL) {
        printf("%s - Error opening file  %s !!\n", argv[0], argv[2]);
        exit(0);
    }
    
    dtx_enable = (Word16)atoi(argv[3]);
    if (dtx_enable == 1)
        printf(" DTX enabled\n");
    else
        printf(" DTX disabled\n");
    f_rate = NULL; /* to avoid  visual warning */
    rate = G729;  /* to avoid  visual warning */
    
    Init_Pre_Process();
    Init_Coder_ld8c(dtx_enable);
    
    for(i=0; i<PRM_SIZE_D; i++) prm[i] = (Word16)0;
    
    /* Loop for each "L_FRAME" speech data. */
    frame=0;
    count_frame = 0L;
    
    while( fread(new_speech, sizeof(Word16), L_FRAME, f_speech) == L_FRAME) {
        if (frame == 32767) frame = 256;
        else frame++;
        
        Pre_Process(new_speech, L_FRAME);
        
        count_frame++;
        printf(" Frame: %d\r", count_frame);
        Coder_ld8c(prm, frame, dtx_enable, rate);
        
        prm2bits_ld8c( prm+1, serial);
        
//        nb_words = (Word16)serial[1] +  (Word16)2;
        
        if (fwrite(serial, sizeof(char), 10, f_serial) != (size_t)10)
            printf("Write Error for frame %d\n", count_frame);
        
    }
    printf("\n");
    printf("%d frames processed\n", count_frame);
    
    if(f_serial) fclose(f_serial);
    if(f_speech) fclose(f_speech);
    if(f_rate) fclose(f_rate);

    return(0);
    
} /* end of main() */

