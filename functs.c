#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "simheader.h"

//add an event to a FIFO queue
void push(queue *eq, event *e){
    int tail = eq->tail;
    int next = (tail + 1) % eq->capacity;
    eq->q[tail] = *e;
    eq->tail = next;
    (eq->curSize)++;
}

//checks if queue is empty, returns 1 if so, or 0.
int isEmpty(queue *eq){
    if((eq->curSize) == 0){
        return 1;
    } else return 0;
}

//removes the next event from queue and returns it
event *pop(queue *eq){
    if(eq == &cpu){
        if(isEmpty(&cpu)){
            ;
        }else{
        int head = cpu.head;
        event *e;
        e = &(cpu.q[head]);
        cpu.head = (head + 1) % QUEUE_CAP;
        (cpu.curSize)--;
        return e;
    }
    }else if(eq == &disk1){
        if(isEmpty(&disk1)){
            ;
        }else{
        int head = disk1.head;
        event *e;
        e = &(disk1.q[head]);
        disk1.head = (head + 1) % QUEUE_CAP;
        (disk1.curSize)--;
        return e;
        }
    }else if(eq == &disk2){
        if(isEmpty(&disk2)){
            ;
        }else{
        int head = disk2.head;
        event *e;
        e = &(disk2.q[head]);
        disk2.head = (head + 1) % QUEUE_CAP;
        (disk2.curSize)--;
        return e;
        }
    }else{
        printf("error...");
        exit(1);
    }
    
}

//returns 1 if the queue is at capacity, or 0
int isFull(queue *eq){
    event tail = eq->q[eq->tail];
    if(tail.type !=  0){
        return 1;
    }
    else return 0;
}

//checks if the Event Queue is empty
int isEmpty_EQ(){
    if(EQ.curSize == 0){
        return 1;
    }
    else return 0;
}

//checks if the Event Queue is full
int isFull_EQ(){
    if(EQ.curSize == EQ.capacity-1){
        return 1;
    }
    else return 0;
}

//destroys excess memory that was requested by malloc()
void destroy(eventQ eq){
    //free(EQ.q);
}

//adds an event to the Event Queue, then linearly finds the proper index for the event based on time
void push_EQ(event *e){
    if(!isFull_EQ()){
        EQ.q[EQ.curSize] = *e;
        int i = EQ.curSize;
        EQ.curSize++;
        event temp = EQ.q[i-1];
            if(EQ.curSize > 1){
            while(i>0 && ((EQ.q[i]).time > (EQ.q[i-1]).time)){
            temp = EQ.q[i-1];
            EQ.q[i-1] = EQ.q[i];
            EQ.q[i] = temp;
            i--;
            }
        }
    }
}

//removes the next event from the Event Queue and returns it
event *pop_EQ(){
    if(isEmpty_EQ()){
        return NULL;
    }else{
        (EQ.curSize)--;
        event *e = &(EQ.q[EQ.curSize]);
        return e;
    }
    
}

//uses rand() library function(with time() function for seed) to generate rand num between min and max
int randGen(int min, int max){
    if(min > max){
        printf("ERROR: Invalid input.\n");
        exit(1);
    }
    int r = max - min;
    return rand() % r + min;
}
