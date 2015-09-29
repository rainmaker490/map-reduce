//
//  main.c
//  cmpsc473_proj3
//
//  Created by Varun Patel on 5/1/15.
//  Copyright (c) 2015 Varun Patel. All rights reserved.
//

#include <stdio.h>
#include "list.h"
#include "readFile.h"
#define MAX_WORD_LENGTH 128

typedef struct threadArg_struct{
    word_list *listOfWords;
    int threadID;
    const char *nameOfFile;
    long long *startPosition;
    long long *endPosition;
    int threadBufSize;
} threadArg_struct;

void tostring(char str[], int num){
    int i, rem, len = 0, n;
    n = num;
    while (n != 0){
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++){
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

void mapperAndReducer(void *arguments){
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    threadArg_struct *arg = (threadArg_struct *) arguments;
    int i = 0, j = 0;
    int bufSize = arg->threadBufSize;
    boolean firstTry=false;
    char buffer[bufSize][MAX_WORD_LENGTH];
    // char temp[MAX_WORD_LENGTH];
    FILE *fp = fopen(arg->nameOfFile, "r");
    fseek(fp, *arg->startPosition, SEEK_SET);
    while (ftell(fp) < *arg->endPosition) {
        pthread_mutex_lock(&mutex);
        i=j%(bufSize);
        if (i == 0 && firstTry){
            for(int x=0;x < bufSize; x++){
                char *temp_storage=malloc(sizeof(char *)*MAX_WORD_LENGTH);
                strcpy(temp_storage, buffer[x]);
                insert(arg->listOfWords, temp_storage,1);
                // map_reduce(NULL, arg->listOfWords);
            }
        }
        fscanf(fp,"%s", buffer[i]);
        j++;
        if (j==2){
            firstTry=true;
        }
        pthread_mutex_unlock(&mutex);
    }
    // map_reduce(NULL, arg->listOfWords);
    fclose(fp);
    /*
    char temp_write[MAX_WORD_LENGTH];
    int temp_int_write;
    word_node *tempp = arg->listOfWords->head;
    while (tempp->next != NULL) {
        strcpy(temp_write, tempp->word);
        temp_int_write=tempp->wordCount;
        insert(finalList, temp_write, temp_int_write);
        tempp = tempp->next;
    }
     */
    // Write the OutPut of each thread to a Threadx.txt file
    pthread_mutex_lock(&mutex);
    char temp_write[MAX_WORD_LENGTH];
    int temp_int_write;
    char *x = "Thread";
    char *y = ".txt";
    char *myFile = malloc(1+strlen(x)+strlen(y)+strlen(temp_write));
    tostring(temp_write, (arg->threadID+1));
    strcpy(myFile, x);
    strcat(myFile, temp_write);
    strcat(myFile, y);
    FILE *output = fopen(myFile, "w");
    fseek(output, 0, SEEK_END);
    word_node *tempp = arg->listOfWords->head;
    while (tempp->next != NULL) {
        strcpy(temp_write, tempp->word);
        temp_int_write=tempp->wordCount;
        fprintf(output, "%s %i\n", temp_write, temp_int_write);
        tempp = tempp->next;
    }
    fclose(output);
    tempp = arg->listOfWords->head;
    // word_node *nextHead = tempp;
    /*
    while (tempp) {
        nextHead = tempp->next;
        free(tempp);
        tempp=nextHead;
    }
     */
    pthread_mutex_unlock(&mutex);
}

int main(int argc, const char * argv[]) {
    int space = 32;
    // char *nameOfFile = argv[3];
    char *nameOfFile = (char *)argv[1];
    long long x = getFileSize(nameOfFile);
    printf("%lld \n", x);
    // int bufferSize = 4;
    // int numberOfThreads=atoi(argv[2]);
    int numberOfThreads=atoi(argv[1]);
    int bufferSize =  atoi(argv[2]);
    word_list *listOfWords = calloc(1,sizeof(word_list)*numberOfThreads);
    pthread_t threads[numberOfThreads];
    long long *positions= calloc(1,sizeof(long long *)*(bufferSize +1));;
    threadArg_struct *threadArgs;
    FILE *fp = fopen(nameOfFile, "r");
    positions[0]=0;
    int i = 1;
    for (i=1; i <= numberOfThreads-1; i++){
        positions[i]=(i)*(x/numberOfThreads);
        fseek(fp, positions[i], SEEK_SET);
        while (fgetc(fp) != space) {
            positions[i]++;
        }
    }
    fclose(fp);
    threadArgs = (threadArg_struct *) malloc(sizeof(threadArg_struct *)*numberOfThreads);
    positions[numberOfThreads]=x;
    /*
    finalList = malloc(sizeof(word_list *));
    pthread_mutex_init(&finalList->mutex, NULL);
    finalList->size=0;
     */
    for (i =0; i < numberOfThreads; i++) {
        initList(&listOfWords[i]);
        threadArgs[i].threadID=i;
    }
    
    for(i=0; i<numberOfThreads; i++){
        threadArgs[i].startPosition=&positions[i];
        threadArgs[i].endPosition=&positions[i+1];
        threadArgs[i].listOfWords=&listOfWords[i];
        threadArgs[i].threadBufSize=bufferSize;
        threadArgs[i].nameOfFile=nameOfFile;
        pthread_create(&threads[i], NULL, (void *)mapperAndReducer, &threadArgs[i]);
    }
    
    for (i=0; i < numberOfThreads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    for (i=0; i < numberOfThreads; i++){
        word_node *new_temp;
        word_node *xx_temp = listOfWords[i].head;
        if (i != 0){
            while (xx_temp) {
                insert(&listOfWords[0], xx_temp->word, xx_temp->wordCount);
                xx_temp=xx_temp->next;
            }
            while (xx_temp) {
                new_temp=xx_temp->next;
                free(xx_temp);
                xx_temp=new_temp;
            }
        }
    }
    
    char temp_write[MAX_WORD_LENGTH];
    int temp_int_write;
    FILE *f = fopen("varun.txt", "w");
    word_node *printME = listOfWords[0].head;
    while (printME->next != NULL) {
        strcpy(temp_write, printME->word);
        temp_int_write=printME->wordCount;
        fprintf(f, "%s %i\n", temp_write, temp_int_write);
        printME = printME->next;
    }
    fclose(f);
    
    // word_node *nextHead = tempp;
    /*
     while (tempp) {
     nextHead = tempp->next;
     free(tempp);
     tempp=nextHead;
     }
     */
    
    /*
    word_list finalList;
    initList(&finalList);
    for (int i=1; i==numberOfThreads; i++) {
        pthread_mutex_lock(&finalList.mutex);
        boolean _success = true;
        char temp_write[1];
        int temp_int_write;
        char *x = "/Users/admin/Desktop/";
        char *y = ".txt";
        char *myFile = malloc(1+strlen(x)+strlen(y)+strlen(temp_write));
        tostring(temp_write, (numberOfThreads));
        strcpy(myFile, x);
        strcat(myFile, temp_write);
        strcat(myFile, y);
        FILE *output = fopen(myFile, "r");
        rewind(output);
        char *_temp_write=_temp_write = malloc(sizeof(char)*MAX_WORD_LENGTH);
        while (!_success) {
            if (!_success){
                fscanf(output, "%s %d", _temp_write,&temp_int_write);
                pthread_mutex_lock(&finalList.mutex);
                int _temp_int_write;
                _temp_int_write=temp_int_write;
                reduceFinalList(&finalList, _temp_write, _temp_int_write);
                pthread_mutex_unlock(&finalList.mutex);
                finalList.size++;
                _success=false;
            } else if (feof(output)) {
                _success = true;
            }
        }
        pthread_mutex_unlock(&finalList.mutex);
        
    }
     */
    
/*
    word_list *list = initList();
    word_list *list2 = initList();
    // insert code here...

    insert(list2, "foo");
    insert(list, "bar");
    insert(list,"foo");
    insert(list2,"foo");
    insert(list2,"bar");
    insert(list2,"foo");
    insert(list,"bar");
    insert(list, "x");
    insert(list2,"x");
    insert(list2, "y");
    insert(list2, "x");

    word_node *temp = list2->head;
    reducer(list,list2);
    while(temp->next != NULL){
        printf("%s (%i) \n",temp->word, temp->wordCount);
        temp=temp->next;
    }
    printf("%s (%i) \n",temp->word, temp->wordCount);
    printf("%i (%i) \n",list->size, list2->size);
*/
    // number of threads
    
/*
    char *numThreads=argv[1];
    int bufferSize = atoi(argv[2]);
    char *inputFile = argv[3];
    
    splitFile(inputFile, numThreads);
    char *buffer[atoi(numThreads)][bufferSize];
    
 */
    
    // printf("Hello, World!\n");
    return 0;
}
