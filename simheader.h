#define INIT_TIME 0
#define FIN_TIME 20000
#define QUIT_PROB 0.2      //probability that job finishes after CPU rather than go to disk queue
#define SEED
#define ARRIVE_MIN 10 
#define ARRIVE_MAX 30
#define CPU_MIN 20   
#define CPU_MAX 50
#define DISK1_MIN 100 
#define DISK1_MAX 150
#define DISK2_MIN 200
#define DISK2_MAX 300
#define QUEUE_CAP 2000
#define EQCAP 2000
#define ARRIVAL 1
#define FIN 2
#define DISKARRIVAL 3
#define DISK1FINISH 4
#define DISK2FINISH 8
#define EXIT 5
#define CPUBEGIN 6
#define DISKBEGIN 7
#define SIMULATION_END 9


static int cpuStatus = 0;           //0 when idle, 1 when busy
static int disk1Status = 0;         //0 when idle, 1 when busy
static int disk2Status = 0;         //0 when idle, 1 when busy
static FILE *file;                  //file to be written to
static float cpujobs = 0, d1jobs = 0, d2jobs = 0;       //tracks total num of jobs per device for response time calculation
static float cpuar = 0, cpubeg = 0;
static float d1ar = 0, d1beg = 0;
static float d2ar = 0, d2beg = 0;                       //a job's response time = time of finish - time of arrival(at device queue)
static float cpurt = 0, d1rt = 0, d2rt = 0;             //avg response time of device to be calculated after simulation
static int cpumaxrt = 0, d1maxrt = 0, d2maxrt = 0;      //maximum response times
//avg response time = sum of response times / num of jobs

/*
    events log and keep track of flow of the instructions through the system. 
    An event happens when a job enters, passes through one of the devices, or exits. 
*/
typedef struct{
        int time;       //time of event
        int jobNum;     //sequential, starting at 1
        int type;   //1=CPUarrival, 2=CPUfinish, 3=DiskArrival, 4=DiskFinish, 5=exit, 6=uninitialized
}event;

/*
    the Event Queue is the means in which this program manages the data processed. It stores every
    event that is generated, and sorts them by time. This is done by implementing an array-based   
    priority queue.
*/
typedef struct{
        //priority queue for events, to be written to file after FIN_TIME
        int capacity;
        int curSize;
        event q[EQCAP * sizeof(event)];
}eventQ;

/*  
    There are three queues in this program, one for Disk 1, Disk 2, and the CPU. They are array-based,
    FIFO queues(first-in-first-out). Whenever a device processes a new job, it takes the job that has 
    been waiting the longest.
*/
typedef struct{
        //FIFO queue, implemented by CPU and disks
        int head;                  
        int tail;                    
        int capacity;
        int curSize;
        int status;
        event q[sizeof(event) * QUEUE_CAP];
}queue;

//initialize the device queues and Event Queue
static queue cpu = {0, 0, QUEUE_CAP, 0, 0, };
static queue disk1 = {0, 0, QUEUE_CAP, 0, 0, };
static queue disk2= {0, 0, QUEUE_CAP, 0, 0, };
static eventQ EQ = {EQCAP, 0, };

void push(queue *eq, event *e);
int isEmpty(queue *eq);
event *pop(queue *eq);
int isFull(queue *eq);
int isEmpty_EQ();
int isFull_EQ();
int randGen(int min, int max);
void destroy(eventQ eq);
void push_EQ(event *e);
event *pop_EQ();
void diskFinish(queue *disk, event *e);
void processExit(event *e);
void process(event *e, event *next);
