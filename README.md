
Giorgio’s Discrete Event Simulator
	This program stimulates an operating system’s process scheduler, representing the flow of tasks through the CPU and I/O devices. 
  A major role and challenge faced by the operating system is to make the most use out of its hardware, especially the CPU, as it 
  is very small and very fast, and every running process needs to access it.
 
The CPU, and both disks in this simulation all are implemented as structs which store status variables and most importantly a
first-in-first-out(FIFO) queue of “events”, implemented as an array. Whenever a device processes a new job, it checks its queue 
for the job that has been waiting the longest to be serviced. 

This program handles the jobs processed by the simulation through a struct called “event”. Events contain three int values for time, 
jobNum, and type. Time holds the time that the event occurs, jobNum is the job that is causing the event, and type refers to the stage 
in the cycle that the job is in. The types of events used are:

ARRIVAL		arrival of job at CPU queue
FIN			job is finished at CPU
DISK1ARRIVAL	arrival of job at disk queue
DISK1FINISH	job is finished at disk
DISK2ARRIVAL
DISK2FINISH
EXIT			job is done with CPU and exits the simulation
CPUBEGIN		job begins processing at CPU
DISKBEGIN		job begins processing at disk
SIMULATION_END	end of simulation

The way in which the jobs are processed and organized throughout the program is through a struct named “eventQ”. The Event Queue 
contains variables for capacity and current size, and an array-based Priority Queue of events. It stores the events with a priority 
sorted by time that the event takes place. When a future event is created, the Event Queue sorts it into its proper index in the
queue. This allows for a realistic program that simulates this flow of events in real time, without actually relying on any time 
measurements. 

When the programming is running, the majority of it takes place in a while loop, where events are continuously popped out of the 
Event Queue until the time of the current event reaches the end time, which is defined by FIN_TIME. The program calls a function 
called “process” that covers each type of event and processes it accordingly. An important characteristic of this function is that 
when it processes each event, it takes the proper measures to maintain the flow of the simulation, just as an operating system has 
to do. Whenever a job arrives, the next sequentially-numbered job is created, with a randomly generated arrival time. Whenever a job 
is finished at a device, the program immediately checks the device’s queue for the next serviceable job, thus maximizing the utilization
of each device. Events are also logged to a file named “simresults.txt”.
After processing all of the jobs in the time limit, the program makes some final analytical calculations and prints them to simresults.txt.  
To test that this program is functioning properly, as well as producing correct results, it keeps track of variables that measure average 
and maximum queue sizes, utilization, response time, throughput, and total jobs. These can be examined in further detail in runs.docx and simulator_test_runs.xls.
This program runs by compiling main.c and functs.c, which take in information from simheader.h. 
