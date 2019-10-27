#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "simheader.h"

void diskFinish(queue *disk, event *e){
    event f = {e->time, e->jobNum, CPUBEGIN};
    event *fin = &f;
    push(&cpu, fin);
}

void processExit(event *e){
    event *f;
    event fin = {e->time, e->jobNum, EXIT};
    f = &fin;
    //push_EQ(f);
}

//keep track of cpu status, process cpu
void process(event *e, event *next){
    int t = e->time;
    if(e->type == ARRIVAL){  
        //generates next job, sends current job to CPU queue
        fprintf (file, "Job %d arrives at time %d\n", e->jobNum, t);
        event *j;
        int r = randGen(CPU_MIN,CPU_MAX);
        event job = {(e->time+r), e->jobNum, CPUBEGIN};
        j = &job;
        push(&cpu, j);
        cpurt += r;
        int num = e->jobNum;
        event n = {(e->time+randGen(ARRIVE_MIN, ARRIVE_MAX)), num+1, ARRIVAL};
        next = &n;
        push_EQ(next);
    }else if(e->type == FIN){
        cpujobs++;
        //cpu is now idle
        cpu.status = 0;      
        //check if job is done or needs disk access
        float x = (float)randGen(0,100);
        float y = QUIT_PROB * 100;
        if(x < y){
            //job is finished
            event f = {e->time, e->jobNum, EXIT};
            event *fin = &f;
            push_EQ(fin);
        }else{
            //send job to disk queue
            event n = {e->time, e->jobNum, DISKARRIVAL};
            event *next = &n;
            push_EQ(next);
            if(disk1.curSize == disk2.curSize){
                int d = randGen(0, 100);
                if(d <= 49){
                    push(&disk1, next);
                }else {
                    push(&disk2, next);
                }
            }else if(disk1.curSize < disk2.curSize){
                push(&disk1, next);
            }else{
                push(&disk2, next);
            }
        }
        fprintf (file, "job %d finishes at CPU at time %d\n", e->jobNum, e->time);
        event *c;
        //checks if CPU is idle, processes next job if so
        if(!(isEmpty(&cpu)) && (cpu.status == 0)){
            c = pop(&cpu);
            event *f;
            c->time = e->time;
            int r = randGen(CPU_MIN, CPU_MAX);
            event fin = {(c->time + r), (c->jobNum), FIN};
            cpurt += r;
            f = &fin;
            push_EQ(f);
            cpu.status = 1;
            fprintf (file, "Job %d begins at CPU at time %d\n", (c->jobNum), c->time);
        }
    }else if(e->type == DISK1FINISH){
        disk1.status = 0;
        d1jobs++;
        diskFinish(&disk1,e);
        fprintf (file, "Job %d finishes at Disk 1 at time %d\n", e->jobNum, e->time);
        event *d1;
        if(!(isEmpty(&disk1)) && (disk1.status == 0)){
        d1 = pop(&disk1);
        disk1.status = 1;
        d1->time = e->time;
        event *f;
        int r = randGen(DISK1_MIN, DISK1_MAX);
        event fin = {(d1->time + r), d1->jobNum, DISK1FINISH};
        d1rt += r;
        f = &fin;
        push_EQ(f);
        fprintf (file, "Job %d begins at Disk 1 at time %d\n", d1->jobNum, d1->time);
    }
    }else if(e->type == DISK2FINISH){
        disk2.status = 0;
        d2jobs++;
        diskFinish(&disk2,e);
        fprintf (file, "Job %d finishes at Disk 2 at time %d\n", e->jobNum, e->time);
        event *d2;
        if(!(isEmpty(&disk2)) && (disk2.status == 0)){
        d2 = pop(&disk2);
        disk2.status = 1;
        d2->time = e->time;
        event *f;
        int r = randGen(DISK2_MIN, DISK2_MAX);
        event fin = {(d2->time + r), d2->jobNum, DISK2FINISH};
        d2rt += r;
        f = &fin;
        push_EQ(f);
        fprintf (file, "Job %d begins at Disk 2 at time %d\n", d2->jobNum, d2->time);
    }
    }else if(e->type == EXIT){
        processExit(e);
        fprintf (file, "Job %d exits at time %d\n", e->jobNum, e->time);
    }else if(e->type == SIMULATION_END){
        destroy(EQ);
    }
    event *c, *d1, *d2;
    if(!(isEmpty(&cpu)) && (cpu.status == 0)){
        c = pop(&cpu);
        event *f;
        int r = randGen(CPU_MIN, CPU_MAX);
        event fin = {(c->time + r), (c->jobNum), FIN};
        f = &fin;
        cpurt += r;
        push_EQ(f);
        cpu.status = 1;
        fprintf (file, "Job %d begins at CPU at time %d\n", (c->jobNum), c->time);
    }if(!(isEmpty(&disk1)) && disk1.status == 0){
        d1 = pop(&disk1);
        disk1.status = 1;
        event *f;
        int r = randGen(DISK1_MIN, DISK1_MAX);
        event fin = {(d1->time + r), d1->jobNum, DISK1FINISH};
        f = &fin;
        d1rt += r;
        push_EQ(f);
        fprintf (file, "Job %d begins at Disk 1 at time %d\n", d1->jobNum, d1->time);
    }if(!(isEmpty(&disk2)) && disk2.status == 0){
        d2 = pop(&disk2);
        disk2.status = 1;
        event *f;
        int r = randGen(DISK2_MIN, DISK2_MAX);
        event fin = {(d2->time + r), d2->jobNum, DISK2FINISH};
        f = &fin;
        d2rt += r;
        push_EQ(f);
        fprintf (file, "Job %d begins at Disk 2 at time %d\n", d2->jobNum, d2->time);
    }

}

int main(){
    srand(time(0));
    file = fopen("simresults.txt", "w");
    if (file == NULL){
        printf("Error opening file...\n");
        exit(1);
    }
    float cpuavg = 0, eqavg = 0;
    float disk1avg = 0, disk2avg = 0;
    int cpumaxsize = 0, eqmaxsize = 0;
    int disk1maxsize = 0, disk2maxsize = 0;
    float cpubusy =0, cpuidle = 0;
    float d1busy = 0, d1idle = 0;
    float d2busy = 0, d2idle = 0;
    event *next;
    int tTime = FIN_TIME - INIT_TIME;
    static int curTime = 0;
    event f = {tTime, 0, SIMULATION_END};
    event n = {0, 1, ARRIVAL};
    event *new = &n;
    push_EQ(new);
    int c = 0;      //to keep track of total interations, and to calculate avg sizes after loop
    while((curTime <= tTime) && !(isEmpty_EQ())){
        //process events from EQ
        event *cur = pop_EQ();
        if(cpu.curSize > cpumaxsize){ 
            cpumaxsize = cpu.curSize;
        }if(disk1.curSize > disk1maxsize){
            disk1maxsize = disk1.curSize;
        }if(disk2.curSize > disk2maxsize){
            disk2maxsize = disk2.curSize;
        }if(EQ.curSize > eqmaxsize){
            eqmaxsize = EQ.curSize;
        }
        cpuavg += cpu.curSize;
        disk1avg += disk1.curSize;
        disk2avg += disk2.curSize;
        eqavg += EQ.curSize;
        curTime = cur->time;
        if(cpu.status == 1){
            cpubusy++;
        }else cpuidle++;
        if(disk1.status == 1){
            d1busy++;
        }else d1idle++;
        if(disk2.status == 1){
            d2busy++;
        }else d2idle++;
        process(cur, next);
        c++;
    }
    cpuavg = cpuavg/((float)c);
    disk1avg = disk1avg/((float)c);
    disk2avg = disk2avg/((float)c);
    eqavg = eqavg/((float)c);
    //calculate utilization:
    float cpuutil = cpubusy/(cpubusy+cpuidle);
    float d1util = d1busy/(d1busy+d1idle);
    float d2util = d2busy/(d2busy+d2idle);
    //calculate average response time
    float cpuart = cpurt/cpujobs;
    float d1art = d1rt/d1jobs;
    float d2art = d2rt/d2jobs;
    float cputhroughput = cpujobs/(float)tTime;
    float d1throughput = d1jobs/(float)tTime;
    float d2throughput = d2jobs/(float)tTime;
    fprintf(file, "\nAverage length of CPU queue: %f\n", cpuavg);
    fprintf(file, "Maximum length of CPU queue: %d\n", cpumaxsize);
    fprintf(file, "\nAverage length of Disk 1 queue: %f\n", disk1avg);
    fprintf(file, "Maximum length of Disk 1 queue: %d\n", disk1maxsize);
    fprintf(file, "\nAverage length of Disk 2 Queue: %f\n", disk2avg);
    fprintf(file, "Maximum length of Disk 2 queue: %d\n", disk2maxsize);
    fprintf(file, "\nAverage length of Event Queue: %f\n", eqavg);
    fprintf(file, "Maximum length of Event Queue: %d\n\n", eqmaxsize);
    fprintf(file, "CPU utilization: %f\n", cpuutil);
    fprintf(file, "Disk 1 utilization: %f\n", d1util);
    fprintf(file, "Disk 2 utilization: %f\n", d2util);
    fprintf(file, "\nAverage CPU response time: %f\n", cpuart);
    fprintf(file, "Average Disk 1 response time: %f\n", d1art);
    fprintf(file, "Average Disk 2 response time: %f\n", d2art);
    fprintf(file, "\nCPU throughput: %f jobs per unit of time.\n", cputhroughput);
    fprintf(file, "Disk 1 throughput: %f jobs per unit of time.\n", d1throughput);
    fprintf(file, "Disk 2 throughput: %f jobs per unit of time.\n", d2throughput);
    fprintf(file, "\nTotal jobs completed by CPU: %d\n", (int)cpujobs);
    fprintf(file, "Total jobs completed by Disk 1: %d\n", (int)d1jobs);
    fprintf(file, "Total jobs completed by Disk 2: %d\n", (int)d2jobs);
    printf("Results logged to file: simresults.txt\n");
    fclose(file);
}
