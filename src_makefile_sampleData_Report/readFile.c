//
//  readFile.c
//  cmpsc473_proj3
//
//  Created by Varun Patel on 5/2/15.
//  Copyright (c) 2015 Varun Patel. All rights reserved.
//

#include "readFile.h"

void removePunctuation(char *word){
    char *src = word, *dst = word;
    while (*src) {
        if (ispunct((unsigned char)*src)) {
            src++;
        } else if (isupper((unsigned char)*src)){
            *dst++ = tolower((unsigned char)*src);
            src++;
        } else if (src == dst) {
            src++;
            dst++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = 0;
}

off_t getFileSize(char *file){
    struct stat buf;
    stat(file, &buf);
    off_t size = buf.st_size;
    return size;
}