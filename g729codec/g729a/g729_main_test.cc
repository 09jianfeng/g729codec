//
//  main.c
//  g729codec
//
//  Created by JFChen on 2019/1/2.
//  Copyright © 2019 JFChen. All rights reserved.
//

#include <stdio.h>
#include "G729Codec.h"
#include "waveOperator.h"
#include "pesq_calculate.h"

extern "C"{
    extern Word16 *new_speech;     /* Pointer to new speech data            */
}

int g729_main() {
    G729Codec *codec = new G729Codec();
    int encodeFrame = 0;
    int decodeFrame = 0;
    std::string inString;
    std::string outString;
    
    {
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
            inString.append((char *)new_speech, L_FRAME * 2);
            
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
            outString.append(outData);
            fwrite(outData.c_str(), sizeof(short), L_FRAME, f_syn);
            readSize = fread(serial, 1, SERIAL_SIZE, f_serial);
        }
        
        fclose(f_syn);
    }
    
    std::string sourceFile = "729sourcePCM.wav";
    CWaveOperator wavOperaSource;
    wavOperaSource.WriteTotalWavData(sourceFile.c_str(),inString,8000,1);
    std::string targetFile = "729targetPCM.wav";
    CWaveOperator wavOperaTarget;
    wavOperaTarget.WriteTotalWavData(targetFile.c_str(), outString, 8000, 1);
    
    float mos = PesqCalculate(sourceFile.c_str(),targetFile.c_str(),8000);
    
    printf("Hello, g729 mos:%f!\n",mos);
    return 0;
}
