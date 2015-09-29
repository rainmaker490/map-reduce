//
//  list.h
//  cmpsc473_proj3
//
//  Created by Varun Patel on 5/1/15.
//  Copyright (c) 2015 Varun Patel. All rights reserved.
//

#ifndef __cmpsc473_proj3__list__
#define __cmpsc473_proj3__list__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int boolean;
#define true 1
#define false 0

typedef struct word_node {
    char *word;
    int wordCount;
    struct word_node *next;
} word_node;

typedef struct word_list {
    word_node *head;
    word_node *tail;
    int size;
    pthread_mutex_t mutex;
} word_list;

void initList(word_list *list);
void insert(word_list *list, char *word,int count);
void removeWordNode(word_list *list, char *word);
void append(word_list *list, char *_word, int count);
void add(word_list *list, char *word);
void addAfter(word_list *list, char *word, int loc);
void reducer(word_list *lista , word_list *listb);
void map_reduce(word_list *shortList, word_list *longList);

#endif /* defined(__cmpsc473_proj3__list__) */