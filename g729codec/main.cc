//
//  main.c
//  g729codec
//
//  Created by JFChen on 2019/1/2.
//  Copyright © 2019 JFChen. All rights reserved.
//

#include <stdio.h>
#include "utils/waveoperator.h"
#include "PESQ/pesq_calculate.h"

extern "C"{
    extern int encoder_main(int argc, char *argv[] );
    extern int decoder_main(int argc, char *argv[] );
}

int main(int argc, const char * argv[]) {
//    g729_main();
    
    //    char *sourceFilePath = "Ch_f1_8k.pcm";
    //    char *targetFilePath = "g729OutPut";
    /*
     argument 0 : ./g729codec
     argument 1 : Ch_f1_8k.pcm
     argument 2 : g729codecout
     argument 3 : 0
     */
    char *argv2[] = {"./g729codec","Ch_f1_8k.pcm","g729codecout","0","1"};
    encoder_main(5, argv2);
    
    /*
     ./g729codec g729codecout decodec.pcm
     */
    char *argv3[] = {"./g729codec","g729codecout","decodec.pcm"};
    decoder_main(3, argv3);
    
    
    CWaveOperator wavOpeEn;
    FILE *f_speech;
    f_speech = fopen("Ch_f1_8k.pcm", "rb");
    std::string pcmData;
    char pcmDataChar[80];
    while(fread(pcmDataChar, 1, 80, f_speech) == 80){
        pcmData.append(pcmDataChar,80);
    }
    wavOpeEn.WriteTotalWavData("sourcePCM.wav", pcmData, 8000, 1);
    
    CWaveOperator wavOpeDe;
    f_speech = fopen("decodec.pcm", "rb");
    std::string dePCMData;
    while(fread(pcmDataChar, 1, 80, f_speech) == 80){
        dePCMData.append(pcmDataChar,80);
    }
    wavOpeDe.WriteTotalWavData("targetPCM.wav", dePCMData, 8000, 1);
    
    float mos = PesqCalculate("sourcePCM.wav", "targetPCM.wav", 16000);
    
    // insert code here...
    printf("Hello, World! mos:%f \n", mos);
    return 0;
}

