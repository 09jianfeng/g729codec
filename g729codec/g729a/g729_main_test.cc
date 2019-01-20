//
//  main.c
//  g729codec
//
//  Created by JFChen on 2019/1/2.
//  Copyright © 2019 JFChen. All rights reserved.
//

#include <stdio.h>
#include "G729Codec.h"

extern "C"{
	extern Word16 *new_speech;     /* Pointer to new speech data            */
}

int g729_main() {
    G729Codec *codec = new G729Codec();
    int encodeFrame = 0;
    int decodeFrame = 0;
    {
//        const char *argv2[] = { "g729codec", "..\\..\\testfile\\ITU\\Ch_f1_8k.pcm", "g729codecout" };
        const char *argv2[] = { "g729codec", "Ch_f1_8k.pcm", "g729codecout" };
        FILE *f_speech;               /* File of speech data                   */
        FILE *f_serial;               /* File of serial bits for transmission  */
        if ( (f_speech = fopen(argv2[1], "rb")) == NULL) {
            printf("%s - Error opening file  %s !!\n", argv2[0], argv2[1]);
            exit(0);
        }
        printf(" Input speech file    :  %s\n", argv2[1]);
        
        if ( (f_serial = fopen(argv2[2], "wb")) == NULL) {
            printf("%s - Error opening file  %s !!\n", argv2[0], argv2[2]);
            exit(0);
        }
        
        while( fread(new_speech, sizeof(Word16), L_FRAME, f_speech) == L_FRAME)
        {
            encodeFrame++;
            printf("Frame:%d \n",encodeFrame);
            std::string outData;
            codec->Encode(new_speech, L_FRAME, outData);
            fwrite(outData.c_str(), 1, SERIAL_SIZE, f_serial);
        }
        fclose(f_serial);
    }
    
    {
        const char *argv3[] = {"g729codec","g729codecout","decodec.pcm"};
        FILE   *f_syn, *f_serial;
        unsigned char serial[SERIAL_SIZE];          /* Serial stream               */
        
        if( (f_serial = fopen(argv3[1],"rb") ) == NULL )
        {
            printf("%s - Error opening file  %s !!\n", argv3[0], argv3[1]);
            exit(0);
        }
        
        if( (f_syn = fopen(argv3[2], "wb") ) == NULL )
        {
            printf("%s - Error opening file  %s !!\n", argv3[0], argv3[2]);
            exit(0);
        }
        
        printf("Input bitstream file  :   %s\n",argv3[1]);
        printf("Synthesis speech file :   %s\n",argv3[2]);
        
        
        unsigned long readSize = fread(serial, 1, SERIAL_SIZE, f_serial);
        while( readSize == SERIAL_SIZE)
        {
            decodeFrame++;
            printf("dFrame:%d \n",decodeFrame);
            std::string outData;
            codec->Decode(serial, SERIAL_SIZE, outData);
            fwrite(outData.c_str(), sizeof(short), L_FRAME, f_syn);
            readSize = fread(serial, 1, SERIAL_SIZE, f_serial);
        }
        
        fclose(f_syn);
    }
    
    printf("Hello, World!\n");
    return 0;
}
