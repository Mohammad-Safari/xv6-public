#include "types.h"
#include "stat.h"
#include "user.h"

// number of children created
#define NUM_CHILDREN 30

int main(int argc, char *argv[])
{
    int result = set_sched_policy(4);
    if(result == 4){
        printf(1,"scheduling policy changed to lottery mode !\n");
    } else {
        printf(1,"could not change the policy !!!\n");
    }

    int main_pid = getpid();
    int child_num = -1;
    int tickets = -1;

    //make process
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        //sleep(100);
        if (fork() == 0) // Child
        {
            child_num = i + 1;
            tickets = 15 + (i / 5) * 5;
            set_tickets(tickets);
            break;
        }
    }

    if (getpid() != main_pid)
    {
        for (int i = 1; i <= 25; i++)
        {
            printf(1, "childNumber = /%d/ : I = /%d/\n", child_num, i);
            // sleep(2000 + 10 * getpid()); // to print in order
        }
    } else {

        int tickets[NUM_CHILDREN] = {0};  // priorites for each child
        int turnarounds[NUM_CHILDREN] = {0}; // turnaround times for each child
        int waitings[NUM_CHILDREN] = {0};    // waiting times for each child
        int CBTs[NUM_CHILDREN] = {0};        // CBTs for each child



        int turnaroundsPerClass[6] = {0}; // turnaround times for each class
        int waitingsPerClass[6] = {0}; // waiting times for each class
        int CBTsPerClass[6] = {0}; // CBTs for each class



        int turnaroundsSum = 0; //sum for turnaround
        int waitingsSum = 0; //sum for waiting
        int CBTsSum = 0; //sum for cbt


        int i = 0;
        int turnAroundtime, waitingtime,  cbttime , pario;

        while (wait_and_fill_statistics(&turnAroundtime, &waitingtime,  &cbttime , &pario) > 0)
        {
            int childTickets = pario;
            int childTurnaround = turnAroundtime;
            int childWaiting = waitingtime;
            int childCBT = cbttime;

            tickets[i] = childTickets;
            turnarounds[i] = childTurnaround;
            waitings[i] = childWaiting;
            CBTs[i] = childCBT;
            i++;
        }

        printf(1, "\n--------Times for each child--------\n");
        for (int j = 0; j < NUM_CHILDREN; j++)
        {
            printf(1, "Child tickets %d | Turnaround : %d, Waiting : %d, CBT : %d\n",
                   tickets[j], turnarounds[j], waitings[j], CBTs[j]);
        }


        printf(1, "\n--------Average Times for each tickets class--------\n");

        for (int j = 0; j < NUM_CHILDREN; j++)
        {
            ///
            int ticketClass = 0;
            if(tickets[j]==15)
            ticketClass = 1;
            if(tickets[j]==20)
            ticketClass = 2;
            if(tickets[j]==25)
            ticketClass = 3;
            if(tickets[j]==30)
            ticketClass = 4;
            if(tickets[j]==35)
            ticketClass = 5;
            if(tickets[j]==40)
            ticketClass = 6;
            ///
            turnaroundsPerClass[ticketClass - 1] += turnarounds[j];
            waitingsPerClass[ticketClass - 1] += waitings[j];
            CBTsPerClass[ticketClass - 1] += CBTs[j];
        }


        for (int j = 0; j < 6; j++)
        {
            ////
            int tickets = -1;
            if (j == 0)
                tickets = 15;
            if (j == 1)
                tickets = 20;
            if (j == 2)
                tickets = 25;
            if (j == 3)
                tickets = 30;
            if (j == 4)
                tickets = 35;
            if (j == 5)
                tickets = 40;
            ///
            printf(1, "tickets Class: %d | Average Turnaround : %d, Average Waiting : %d, Average CBT : %d\n",
                   tickets,
                   turnaroundsPerClass[j] / (NUM_CHILDREN / 6),
                   waitingsPerClass[j] / (NUM_CHILDREN / 6),
                   CBTsPerClass[j] / (NUM_CHILDREN / 6));
        }


        printf(1, "\n-------- Average Times in total--------\n");


        for (int j = 0; j < NUM_CHILDREN; j++)
        {
            turnaroundsSum += turnarounds[j];
            waitingsSum += waitings[j];
            CBTsSum += CBTs[j];
        }
        printf(1, "Average Turnaround: %d\nAverage Waiting: %d\nAverage CBT: %d\n",
               turnaroundsSum / NUM_CHILDREN,
               waitingsSum / NUM_CHILDREN,
               CBTsSum / NUM_CHILDREN);
    }

    while (wait() != -1);

    exit();
}