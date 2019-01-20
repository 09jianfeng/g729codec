//
//  G729Codec.cpp
//  g729codec
//
//  Created by JFChen on 2019/1/18.
//  Copyright © 2019 JFChen. All rights reserved.
//

#include "G729Codec.h"

bool shouldDiscard(int rate){
    int randomValue = rand();
    if (randomValue%100 < rate) {
        return true;
    }
    return false;
}

G729Codec::G729Codec(){
    /*--------------------------------------------------------------------------*
     * Initialization of the coder.                                             *
     *--------------------------------------------------------------------------*/
    
    // encoder
    Init_Pre_Process();
    Init_Coder_ld8a();
    
    extern Word16 bad_lsf;
    bad_lsf = 0;
    //decoder
    Init_Decod_ld8a();
    Init_Post_Filter();
    Init_Post_Process();
    for (Word16 i=0; i<M; i++) m_synth_buf[i] = 0;
    m_synth = m_synth_buf + M;
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
    Pre_Process(new_speech, L_FRAME);
    Coder_ld8a(prm);
    prm2bits_ld8k( prm, serial);
    outData.append((char *)serial,SERIAL_SIZE);
    
    return 0;
}

// inData must be 10
int G729Codec::Decode(unsigned char *inData, int dataLen, std::string& outPCM){
    if (dataLen != 10) {
        printf("<error> decode error, datalen must to be 10");
        return -1;
    }
    
    bits2prm_ld8k(inData, &m_parm[1]);
    m_parm[0] = 0;
    m_parm[4] = 0;
    
    //discard parm
    randomDiscardParam();
    
    Decod_ld8a(m_parm, m_synth, m_Az_dec, m_T2);
    Post_Filter(m_synth, m_Az_dec, m_T2);
    Post_Process(m_synth, L_FRAME);
    outPCM.append((char *)m_synth,L_FRAME*2);
    return 0;
}

void G729Codec::randomDiscardParam(){
    //LSP
    if (shouldDiscard(0.0))
    {
        m_parm[1] = 0;
        m_parm[2] = 0;
    }
    
    // Adaptive-codebook delay
    if (shouldDiscard(0.0))
    {
        m_parm[3] = 0;
        m_parm[8] = 0;
    }
    
    // gains
    if (shouldDiscard(0.0))
    {
        m_parm[7] = 0; //Fitst frame
        m_parm[11] = 0; //second frame
    }
    
    // codebook
    if (shouldDiscard(0.0))
    {   //first subframe
        m_parm[5] = 0; // Fixed-codebook index
        m_parm[6] = 0; // Fixed-codebook sign
        //second subframe
        m_parm[9] = 0;
        m_parm[10] = 0;
    }
    
    // second frame set to zero
    /*
     for (int i = PRM_SIZE - 4; i < PRM_SIZE; i++) {
     m_parm[i] = 0;
     }*/
}
