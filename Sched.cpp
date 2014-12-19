#include <Sched.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <os_sim.h>

void Sorting(DispProc *proc,int iProcInRQ,int iSortOn)
{
    DispProc Temp;
    int i,j,k;
    for ( i = 1 ; i <= (iProcInRQ - 1) ; i++ )
     {
         for ( j = 0 ; j < i ; j++ )
         {
             if ( (iSortOn == SJF) && (proc[j].iBurst > proc[i].iBurst) )
             {
                 memcpy(&Temp,&proc[j],sizeof(DispProc));
                 memcpy(&proc[j],&proc[i],sizeof(DispProc));
                 for ( k = i ; k > j ; k-- )
                         memcpy(&proc[k],&proc[k-1],sizeof(DispProc));

                 memcpy(&proc[k+1],&Temp,sizeof(DispProc));
             }
             else if( (iSortOn == PRIORITY) && (proc[j].iPriority > proc[i].iPriority) )
             {
                 memcpy(&Temp,&proc[j],sizeof(DispProc));
                 memcpy(&proc[j],&proc[i],sizeof(DispProc));
                 for ( k = i ; k > j ; k-- )
                         memcpy(&proc[k],&proc[k-1],sizeof(DispProc));

                 memcpy(&proc[k+1],&Temp,sizeof(DispProc));
             }
         }
     }
    for ( i = 0 ; i <= (iProcInRQ-1) ; i++ )
    {
        printf("Burst size in sorting is %d %d\n",proc[i].iPriority,iProcInRQ);
    }
    printf("\n");
}

void UpdateDisplayRQAtEveryBeat(DispProc *proc,ProcessDef data)
{
    int i;
    for(i = 0;i<MAXIMUM_PROC;i++)
    {
        if(data.iProcessID == proc[i].iProcessID)
        {
            proc[i].iBurst = data.iBurstTime;
        }
    }
}
int GenRendTime()
{
    int iTime;
    do
    {
        iTime = rand();
        iTime = iTime/(1000*1000*100);
    }while(iTime == 0);
    return iTime;
}
void FillProcessInfo(ProcessDef *ProcessInfo)
{
    int i;
    for(i = 0;i<MAXIMUM_PROC;i++)
    {
        if(false == ProcessInfo[i].bFill)
        {
            ProcessInfo[i].bFill = true;
            ProcessInfo[i].iBurstTime = GenRendTime();
            ProcessInfo[i].iArrivalTime = GenRendTime();
            ProcessInfo[i].iPriority = GenRendTime();
            sprintf(ProcessInfo[i].cProcessName,"Process %d",(i+1));
            ProcessInfo[i].iProcessID = i+1;
            ProcessInfo[i].bInJobQ = true;
            break;
        }
        else
        {
        }
    }
}
