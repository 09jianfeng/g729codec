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
}


class G729Codec {
public:
    G729Codec();
    virtual ~G729Codec();
    virtual void Destroy();
    virtual void Uninit();
    virtual int Encode(Word16 *inData,int dataLen, std::string& outData);
    virtual int Decode(char *inData, int dataLen, std::string& outPCM);
    
};

#endif /* G729Codec_hpp */
