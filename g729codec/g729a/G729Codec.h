//
//  G729Codec.hpp
//  g729codec
//
//  Created by JFChen on 2019/1/18.
//  Copyright © 2019 JFChen. All rights reserved.
//

#ifndef G729Codec_hpp
#define G729Codec_hpp

#include <stdio.h>
#include <string>

extern "C" {
#include "typedef.h"
#include "basic_op.h"
#include "ld8a.h"
    
extern Word16 *new_speech;     /* Pointer to new speech data            */
}


class G729Codec {
public:
    G729Codec();
    virtual ~G729Codec();
    virtual void Destroy();
    virtual void Uninit();
    virtual int Encode(Word16 *inData,int dataLen, std::string& outData);
    virtual int Decode(unsigned char *inData, int dataLen, std::string& outPCM);
    
private:
    /*---decoder---*/
    Word16  m_synth_buf[L_FRAME+M], *m_synth; /* Synthesis                   */
    Word16  m_parm[PRM_SIZE+1];             /* Synthesis parameters        */
    Word16  m_Az_dec[MP1*2];                /* Decoded Az for post-filter  */
    Word16  m_T2[2];                        /* Pitch lag for 2 subframes   */
    
    void randomDiscardParam();
};

#endif /* G729Codec_hpp */


/* frame form
 Table 1 – Bit allocation of the 8 kbit/s CS-ACELP algorithm (10 ms frame)
 Parameter               Codeword            Subframe 1          Subframe 2          Total per frame
 Line spectrum pairs   L0, L1, L2, L3                                                        18
 Adaptive-codebook delay   P1, P2                8                   5                       13
 Pitch-delay parity          P0                  1                                            1
 Fixed-codebook index      C1, C2                13                  13                      26
 Fixed-codebook sign       S1, S2                4                   4                       8
 Codebook gains (stage 1)  GA1, GA2              3                   3                       6
 Codebook gains (stage 2)  GB1, GB2              4                   4                       8
 Total                                                                                       80
 */
