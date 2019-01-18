//
//  G729Codec.cpp
//  g729codec
//
//  Created by JFChen on 2019/1/18.
//  Copyright © 2019 JFChen. All rights reserved.
//

#include "G729Codec.h"

G729Codec::G729Codec(){
    /*--------------------------------------------------------------------------*
     * Initialization of the coder.                                             *
     *--------------------------------------------------------------------------*/
    
    // encoder
    Init_Pre_Process();
    Init_Coder_ld8a();
    
    //decoder
    Init_Decod_ld8a();
    Init_Post_Filter();
    Init_Post_Process();
}

G729Codec::~G729Codec(){
    
}

void G729Codec::Destroy(){
    
}

void G729Codec::Uninit(){
    
}

// framesize must be 80
int G729Codec::Encode(Word16 *inData,int dataLen, std::string& outData){
    if (dataLen != 80) {
        printf("<error> dataLen must be 80");
        return -1;
    }
    
    Word16 prm[PRM_SIZE];          /* Analysis parameters.                  */
    unsigned char serial[SERIAL_SIZE];    /* Output bitstream buffer               */
    
    Set_zero(prm, PRM_SIZE);
    Pre_Process(inData, L_FRAME);
    Coder_ld8a(prm);
    prm2bits_ld8k( prm, serial);
    outData.append((char *)serial,SERIAL_SIZE);
    
    return 0;
}

// inData must be 10
int G729Codec::Decode(char *inData, int dataLen, std::string& outPCM){
    if (dataLen != 10) {
        printf("<error> decode error, datalen must to be 10");
        return -1;
    }
    
    
    return 0;
}
