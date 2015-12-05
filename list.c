//
//  list.c
//  cmpsc473_proj3
//
//  Created by Varun Patel on 5/1/15.
//  Copyright (c) 2015 Varun Patel. All rights reserved.
//

#include "list.h"

void append(word_list *list, char *_word, int count){
    pthread_mutex_lock(&list->mutex);
    word_node *isInList;
    boolean isFound = false;
    if (list->size == 0){
        word_node *_temp = (word_node *)calloc(1,sizeof(word_node));
        _temp->word=_word;
        _temp->wordCount=count;
        list->head=_temp;
        list->tail=_temp;
        _temp->next=NULL;
    } else {
        isInList=list->head;
        while (isInList->next != NULL){
            if(strcmp(isInList->word, _word) == 0){
                isInList->wordCount+=count;
                isFound=true;
                break;
            }
            isInList=isInList->next;
        }
        if(!isFound){
            word_node *temp_ = (word_node *)calloc(1,sizeof(word_node));
            temp_->word=_word;
            temp_->wordCount=count;
            if (list->head == list->tail) {
                list->head->next=temp_;
                temp_->next=NULL;
                list->tail=temp_;
            } else {
                word_node *right=list->tail;
                right->next=temp_;
                temp_->next=NULL;
                list->tail=temp_;
            }
            // list->size++;
        }
    }
    pthread_mutex_unlock(&list->mutex);
}

void add(word_list *list, char *word) {
    pthread_mutex_lock(&list->mutex);
    word_node *temp=(word_node *)calloc(1,sizeof(word_node));
    temp->word=word;
    temp->wordCount=1;
    if (list==NULL) {
        list->head=temp;
        list->tail=temp;
        temp->next=NULL;
    } else {
        temp->next=list->head;
        list->head=temp;
    }
    pthread_mutex_unlock(&list->mutex);
}

void addAfter(word_list *list, char* num, int loc){
    int i;
    word_node *temp,*left,*right;
    temp=(word_node *)malloc(sizeof(word_node));
    left=right=list->head;
    for(i=1;i<loc;i++){
        left=right;
        right=right->next;
    }
    temp->word=num;
    temp->wordCount=1;
    left->next=temp;
    left=temp;
    left->next=right;
}

void initList(word_list *list){
    list = (word_list *) malloc(sizeof(word_list));
    list->size=0;
    pthread_mutex_init(&list->mutex, NULL);
}

void insert(word_list *_list, char *word, int _count){
    pthread_mutex_lock(&_list->mutex);
    if (_list->head==NULL) {
        _list->size=0;
    }
    append(_list, word,_count);
    _list->size++;
    pthread_mutex_unlock(&_list->mutex);
}

/* MARK: Deprecated
void map_reduce(word_list *shortList, word_list *longList){
    boolean isAllReduced = false;
    word_node *temp,*left,*right;
    if (shortList == NULL){
        isAllReduced=true;
    } else {
        temp = shortList->head;
    }
    while (!isAllReduced) {
        if (shortList->head == NULL && shortList->size == 0) {
            isAllReduced = true;
            shortList->tail = NULL;
            shortList->size = 0;
            free(shortList);
        } else {
            temp = shortList->head;
            insert(longList, temp->word,1);
            shortList->head=temp->next;
            temp = temp->next;
            shortList->size=shortList->size-1;
        }
    }
    isAllReduced = false;
    pthread_mutex_lock(&longList->mutex);
    temp=longList->head;
    left=longList->head;
    right=longList->head->next;
    if (longList->size > 3){
        while (right->next != NULL) {
            if (strcmp(left->word, right->word) == 0) {
                right->wordCount+=left->wordCount;
                longList->size--;
                if(left == longList->head){
                    free(left);
                    longList->head=right;
                    temp=longList->head;
                    left = right;
                    right = left->next;
            } else {
                    free(left);
                    temp->next=right;
                    left= right;
                    right = right->next;
                }
            } else {
                temp = left;
                left = right;
                right = right->next;
            }
        }
    }
}

void reducer(word_list *lista , word_list *listb){
    if (lista->size > listb->size) {
        map_reduce(listb, lista);
    } else{
        map_reduce(lista , listb);
    }
}
*/
