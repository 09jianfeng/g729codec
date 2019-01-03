//
//  main.c
//  g729codec
//
//  Created by JFChen on 2019/1/2.
//  Copyright © 2019 JFChen. All rights reserved.
//

#include <stdio.h>

extern int encoder_main(int argc, char *argv[] );
extern int decoder_main(int argc, char *argv[] );

int main(int argc, const char * argv[]) {
    
//    char *sourceFilePath = "Ch_f1_8k.pcm";
//    char *targetFilePath = "g729OutPut";
    /*
     argument 0 : ./g729codec
     argument 1 : Ch_f1_8k.pcm
     argument 2 : g729codecout
     argument 3 : 0
     */
    char *argv2[] = {"./g729codec","Ch_f1_8k.pcm","g729codecout"};
    encoder_main(3, argv2);
    
    /*
     ./g729codec g729codecout decodec.pcm
     */
    char *argv3[] = {"./g729codec","g729codecout","decodec.pcm"};
    decoder_main(3, argv3);
    
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
