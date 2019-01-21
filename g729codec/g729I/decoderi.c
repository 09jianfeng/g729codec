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
 File : DECODERI.C
 */
/* from decodere.c G.729 Annex E Version 1.2  Last modified: May 1998 */
/* from decoderd.c G.729 Annex D Version 1.2  Last modified: May 1998 */
/* from decoder.c G.729 Annex B Version 1.3  Last modified: August 1997 */
/* from decoder.c G.729 Version 3.3  */

/*--------------------------------------------------------------------------------------*
 * Main program of the ITU-T G.729 annex I   11.8/8/6.4 kbit/s encoder.
 *
 *    Usage : decoderi bitstream_file  output_file
 *--------------------------------------------------------------------------------------*/

/* ------------------------------------------------------------------------ */
/*                            MAIN PROGRAM                                  */
/* ------------------------------------------------------------------------ */

#include <stdlib.h>
#include <stdio.h>

#include "typedef.h"
#include "basic_op.h"
#include "ld8k.h"
#include "ld8cp.h"
#include "dtx.h"
#include "octet.h"

#define POSTPROC

/*-----------------------------------------------------------------*
 *            Main decoder routine                                 *
 *-----------------------------------------------------------------*/

int decoder_main(int argc, const char *argv[] )
{
    Word16 Vad;
    Word16  synth_buf[L_ANA_BWD], *synth; /* Synthesis                   */
    Word16  parm[PRM_SIZE_E+3];             /* Synthesis parameters        */
    unsigned char  serial[SERIAL_SIZE_E];            /* Serial stream               */
    Word16  Az_dec[M_BWDP1*2], *ptr_Az;       /* Decoded Az for post-filter  */
    Word16  T0_first;                         /* Pitch lag in 1st subframe   */
    Word16  pst_out[L_FRAME];                 /* Postfilter output           */
    
    Word16  voicing;                          /* voicing from previous frame */
    Word16  sf_voic;                          /* voicing for subframe        */
    
    Word16  i;
    Word32 frame;
    Word16  ga1_post, ga2_post, ga_harm;
    Word16  long_h_st, m_pst;
    Word16  serial_size;
    Word16  bwd_dominant;
    FILE    *f_syn, *f_serial;
    
    /* Open file for synthesis and packed serial stream */
    if( (f_serial = fopen(argv[1],"rb") ) == NULL ) {
        printf("%s - Error opening file  %s !!\n", argv[0], argv[1]);
        exit(0);
    }
    if( (f_syn = fopen(argv[2], "wb") ) == NULL ) {
        printf("%s - Error opening file  %s !!\n", argv[0], argv[2]);
        exit(0);
    }
    
    /*-----------------------------------------------------------------*
    *           Initialization of decoder                             *
    *-----------------------------------------------------------------*/
    for (i=0; i<L_ANA_BWD; i++) synth_buf[i] = 0;
    synth = synth_buf + MEM_SYN_BWD;
    
    Init_Decod_ld8c();
    Init_Post_Filter();
    Init_Post_Process();
    
    voicing = 60;

    ga1_post = GAMMA1_PST_E;
    ga2_post = GAMMA2_PST_E;
    ga_harm = GAMMA_HARM_E;
    /* for G.729b */
    Init_Dec_cng();
    
    frame = 0L;
    /*-----------------------------------------------------------------*
    *            Loop for each "L_FRAME" speech data                  *
    *-----------------------------------------------------------------*/
    serial_size = 10;
    while( fread(serial, sizeof(char), 10, f_serial) == 10) {
//        fread(&serial[2], sizeof(Word16), serial_size, f_serial);
        
        frame++;
        printf(" Frame: %d\r", frame);
        bits2prm_ld8c(serial, parm+2);

        parm[5] = Check_Parity_Pitch(parm[4], parm[5]);
        parm[0] = 0;           /* No frame erasure */
        parm[1] = 3; //bitrate
        
        /* ---------- */
        /*  Decoding  */
        /* ---------- */
        Decod_ld8c(parm, voicing, synth_buf, Az_dec, &T0_first, &bwd_dominant,
            &m_pst, &Vad);
        
        /* ---------- */
        /* Postfilter */
        /* ---------- */
        ptr_Az = Az_dec;
        
        /* Adaptive parameters for postfiltering */
        /* ------------------------------------- */
        long_h_st = LONG_H_ST;
        ga1_post = GAMMA1_PST;
        ga2_post = GAMMA2_PST;
        ga_harm = GAMMA_HARM;

        for(i=0; i<L_FRAME; i++) pst_out[i] = synth[i];
        
        voicing = 0;  /* XXX */
        for(i=0; i<L_FRAME; i+=L_SUBFR) {
            Poste(T0_first, &synth[i], ptr_Az, &pst_out[i], &sf_voic,
                ga1_post, ga2_post, ga_harm, long_h_st, m_pst, Vad);
            if (sf_voic != 0) voicing = sf_voic;
            ptr_Az += m_pst+1;
        }
        
        
        Post_Process(pst_out, L_FRAME);

        fwrite(pst_out, sizeof(Word16), L_FRAME, f_syn);
    }
    printf("\n");
    if(f_serial) fclose(f_serial);
    if(f_syn) fclose(f_syn);

    return(0);
}


