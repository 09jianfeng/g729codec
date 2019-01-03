/* ITU-T G.729 Software Package Release 2 (November 2006) */
/*
   ITU-T G.729A Speech Coder    ANSI-C Source Code
   Version 1.1    Last modified: September 1996

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke
   All rights reserved.
*/

/*-------------------------------------------------------------------*
 * Main program of the ITU-T G.729A  8 kbit/s encoder.               *
 *                                                                   *
 *    Usage : coder speech_file  bitstream_file                      *
 *-------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "basic_op.h"
#include "ld8a.h"

int encoder_main(int argc, char *argv[] )
{
  FILE *f_speech;               /* File of speech data                   */
  FILE *f_serial;               /* File of serial bits for transmission  */

  extern Word16 *new_speech;     /* Pointer to new speech data            */

  Word16 prm[PRM_SIZE];          /* Analysis parameters.                  */
  unsigned char serial[SERIAL_SIZE];    /* Output bitstream buffer               */

  Word16 frame;                  /* frame counter */


  printf("\n");
  printf("***********    ITU G.729A 8 KBIT/S SPEECH CODER    ***********\n");
  printf("\n");
  printf("------------------- Fixed point C simulation -----------------\n");
  printf("\n");
  printf("------------ Version 1.1 (Release 2, November 2006) --------\n");
  printf("\n");


/*--------------------------------------------------------------------------*
 * Open speech file and result file (output serial bit stream)              *
 *--------------------------------------------------------------------------*/

  if ( argc != 3 )
    {
       printf("Usage : coder speech_file  bitstream_file\n");
       printf("\n");
       printf("Format for speech_file:\n");
       printf("  Speech is read from a binary file of 16 bits PCM data.\n");
       printf("\n");
       printf("Format for bitstream_file:\n");
       printf("  One (2-byte) synchronization word \n");
       printf("  One (2-byte) size word,\n");
       printf("  80 words (2-byte) containing 80 bits.\n");
       printf("\n");
       exit(1);
    }

  if ( (f_speech = fopen(argv[1], "rb")) == NULL) {
     printf("%s - Error opening file  %s !!\n", argv[0], argv[1]);
     exit(0);
  }
  printf(" Input speech file    :  %s\n", argv[1]);

  if ( (f_serial = fopen(argv[2], "wb")) == NULL) {
     printf("%s - Error opening file  %s !!\n", argv[0], argv[2]);
     exit(0);
  }
  printf(" Output bitstream file:  %s\n", argv[2]);

/*--------------------------------------------------------------------------*
 * Initialization of the coder.                                             *
 *--------------------------------------------------------------------------*/

  Init_Pre_Process();
  Init_Coder_ld8a();
  Set_zero(prm, PRM_SIZE);

  /* Loop for each "L_FRAME" speech data. */

  frame =0;
//  while( fread(new_speech, sizeof(Word16), L_FRAME, f_speech) == L_FRAME)
//  {
//    printf("Frame =%d\r", frame++);
//    Pre_Process(new_speech, L_FRAME);
//    Coder_ld8a(prm);
//    prm2bits_ld8k( prm, serial);
//    fwrite(serial, sizeof(Word16), SERIAL_SIZE, f_serial);
//  }
    while( fread(new_speech, sizeof(Word16), L_FRAME, f_speech) == L_FRAME)
    {
        printf("Frame =%d\r", frame++);
        Pre_Process(new_speech, L_FRAME);
        Coder_ld8a(prm);
        prm2bits_ld8k( prm, serial);
        fwrite(serial, 1, SERIAL_SIZE, f_serial);
    }
    
    fclose(f_serial);
  return (0);
}

