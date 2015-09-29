//
//  readFile.h
//  cmpsc473_proj3
//
//  Created by Varun Patel on 5/2/15.
//  Copyright (c) 2015 Varun Patel. All rights reserved.
//

#ifndef __cmpsc473_proj3__readFile__
#define __cmpsc473_proj3__readFile__

#include <stdio.h>
#include <sys/types.h>
#include  <sys/stat.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>


void removePunctuation(char *p);
off_t getFileSize(char *file);
void splitFile(char *fileName, char *numberOfThreads);
#endif /* defined(__cmpsc473_proj3__readFile__) */
