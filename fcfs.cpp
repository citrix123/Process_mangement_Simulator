#include "fcfs.h"
#include "ui_fcfs.h"
#include <Sched.h>
#include <stdio.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <os_sim.h>
#include <QMessageBox>

FCFS *fcfs,*sjf;
bool bFCFSOpen;
ProcessDef ProcessInfo[MAXIMUM_PROC],ProcStructInUse[MAXIMUM_PROC];
DispProc DisplayRQ[MAXIMUM_PROC];
void FCFS::DisableWidgetsBeforeProcessCreation()
{
    gUi->StartSim->setEnabled(false);
    gUi->PauseSim->setEnabled(false);
    gUi->StepSim->setEnabled(false);
    gUi->ResetSim->setEnabled(false);
}
void FCFS::EnableWidgetAfterProcessCreation()
{
    gUi->StartSim->setEnabled(true);
    gUi->ResetSim->setEnabled(true);
}
void FCFS::ClearProcessLabels()
{
    gUi->Process1->setText("");
    gUi->Process1_2->setText("");
    gUi->Process1_3->setText("");
    gUi->Process1_4->setText("");
    gUi->Process1_5->setText("");
    gUi->Process1_6->setText("");
    gUi->Process1_7->setText("");
    gUi->Process1_8->setText("");
    gUi->Process1_9->setText("");
    gUi->Process1_10->setText("");
    gUi->Process1_11->setText("");
    gUi->Process1_12->setText("");
    gUi->Process1_13->setText("");
    gUi->Process1_14->setText("");
}
void FCFS::ClearBTLabels()
{
    gUi->BurstTime1->setText("");
    gUi->BurstTime1_2->setText("");
    gUi->BurstTime1_3->setText("");
    gUi->BurstTime1_4->setText("");
    gUi->BurstTime1_5->setText("");
    gUi->BurstTime1_6->setText("");
    gUi->BurstTime1_7->setText("");
    gUi->BurstTime1_8->setText("");
    gUi->BurstTime1_9->setText("");
    gUi->BurstTime1_10->setText("");
    gUi->BurstTime1_11->setText("");
    gUi->BurstTime1_12->setText("");
    gUi->BurstTime1_13->setText("");
    gUi->BurstTime1_14->setText("");

}
void FCFS::ClearATLabels()
{
    gUi->ArrivalTime1->setText("");
    gUi->ArrivalTime1_2->setText("");
    gUi->ArrivalTime1_3->setText("");
    gUi->ArrivalTime1_4->setText("");
    gUi->ArrivalTime1_5->setText("");
    gUi->ArrivalTime1_6->setText("");
    gUi->ArrivalTime1_7->setText("");
    gUi->ArrivalTime1_8->setText("");
    gUi->ArrivalTime1_9->setText("");
    gUi->ArrivalTime1_10->setText("");
    gUi->ArrivalTime1_11->setText("");
    gUi->ArrivalTime1_12->setText("");
    gUi->ArrivalTime1_13->setText("");
    gUi->ArrivalTime1_14->setText("");
}
void FCFS::ClearRQLabels()
{
    gUi->ReadyQ1->setText("");
    gUi->ReadyQ1_2->setText("");
    gUi->ReadyQ1_3->setText("");
    gUi->ReadyQ1_4->setText("");
    gUi->ReadyQ1_5->setText("");
    gUi->ReadyQ1_6->setText("");
    gUi->ReadyQ1_7->setText("");
    gUi->ReadyQ1_8->setText("");
    gUi->ReadyQ1_9->setText("");
    gUi->ReadyQ1_10->setText("");
    gUi->ReadyQ1_11->setText("");
    gUi->ReadyQ1_12->setText("");
    gUi->ReadyQ1_13->setText("");
    gUi->ReadyQ1_14->setText("");
}
void FCFS::ClearCPULabels()
{
    gUi->CPU->setText("");
}
void FCFS::ClearBTTimeLabel()
{
    gUi->BTTimerLeft->setText("");
    gUi->CPU->setStyleSheet("background-color: rgb(0, 0, 0);");
}
void FCFS::ClearAllStructure()
{
    int i;
    for(i=0;i<MAXIMUM_PROC;i++)
    {
        memset(&ProcessInfo[i],0,sizeof(ProcessDef));
        memset(&DisplayRQ[i],0,sizeof(DispProc));
        memset(ProcStructInUse,0,sizeof(ProcessDef));
    }
    bCPUBusy = false;
    iProcInRQ = 0;
    iProcessInSystem = 0;
    bFcfsStart = false;
    bFcfsPause = false;
    bFcfsReset = false;
    gUi->ChangeAT->clear();
    gUi->ChangeBT->clear();
    gUi->ChangeAT->addItem("Change Arrival Time");
    gUi->ChangeBT->addItem("Change Burst Time");
}
void FCFS::ClearStructureForStop()
{
    int i;
    for(i=0;i<MAXIMUM_PROC;i++)
    {
        memset(&ProcessInfo[i],0,sizeof(ProcessDef));
        memset(&DisplayRQ[i],0,sizeof(DispProc));
    }
    bCPUBusy = false;
    iProcInRQ = 0;
}

void FCFS::DeleteTimer()
{
    if(NULL != Timer)
    {
        printf("Delete timer\n");
        Timer->stop();
        delete Timer;
        Timer = NULL;
    }
}

FCFS::FCFS(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FCFS)
{
    ui->setupUi(this);
     int i;
     bFCFSOpen = false;
     bCPUBusy = false;
     iProcInRQ = 0;
     iProcessInSystem = 0;
     for(i=0;i<MAXIMUM_PROC;i++)
     {
         memset(&ProcessInfo[i],0,sizeof(ProcessInfo));
         memset(&DisplayRQ[i],0,sizeof(DisplayRQ));
         memset(&ProcStructInUse[i],0,sizeof(ProcessDef));
     }
     gUi = ui;
     Timer = NULL;
    this->setWindowTitle("First Come First Serve Scheduling");
    DisableWidgetsBeforeProcessCreation();
}
void FCFS::closeEvent(QCloseEvent *event)
{
    event->ignore();
    event->accept();
    ClearFCFSAll();
    DeleteTimer();
    printf("Dleted object is %d\n",this->iTypeOFScheduling);
        delete fcfs;
}
void FCFS::ClearFCFSAll()
{
    bFCFSOpen = false;
}
FCFS::~FCFS()
{
    delete ui;
}

void OpenFCFSWindow()
{
    if(!bFCFSOpen)
    {
        fcfs = new FCFS();
        fcfs->show();
        bFCFSOpen = true;
    }
}
void FCFS::ProcessDisplayOnHMi(ProcessDef *info)
{
    if(info[0].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[0].iBurstTime);
        gUi->Process1->setText(info[0].cProcessName);
        gUi->BurstTime1->setText(cBurst);
        sprintf(cArrival,"%d",info[0].iArrivalTime);
        gUi->ArrivalTime1->setText(cArrival);
    }
    if(info[1].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[1].iBurstTime);
        gUi->Process1_2->setText(info[1].cProcessName);
        gUi->BurstTime1_2->setText(cBurst);
        sprintf(cArrival,"%d",info[1].iArrivalTime);
        gUi->ArrivalTime1_2->setText(cArrival);
    }
    if(info[2].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[2].iBurstTime);
        gUi->Process1_3->setText(info[2].cProcessName);
        gUi->BurstTime1_3->setText(cBurst);
        sprintf(cArrival,"%d",info[2].iArrivalTime);
        gUi->ArrivalTime1_3->setText(cArrival);
    }
    if(info[3].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[3].iBurstTime);
        gUi->Process1_4->setText(info[3].cProcessName);
        gUi->BurstTime1_4->setText(cBurst);
        sprintf(cArrival,"%d",info[3].iArrivalTime);
        gUi->ArrivalTime1_4->setText(cArrival);
    }
    if(info[4].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[4].iBurstTime);
        gUi->Process1_5->setText(info[4].cProcessName);
        gUi->BurstTime1_5->setText(cBurst);
        sprintf(cArrival,"%d",info[4].iArrivalTime);
        gUi->ArrivalTime1_5->setText(cArrival);
    }
    if(info[5].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[5].iBurstTime);
        gUi->Process1_6->setText(info[5].cProcessName);
        gUi->BurstTime1_6->setText(cBurst);
        sprintf(cArrival,"%d",info[5].iArrivalTime);
        gUi->ArrivalTime1_6->setText(cArrival);
    }
    if(info[6].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[6].iBurstTime);
        gUi->Process1_7->setText(info[6].cProcessName);
        gUi->BurstTime1_7->setText(cBurst);
        sprintf(cArrival,"%d",info[6].iArrivalTime);
        gUi->ArrivalTime1_7->setText(cArrival);
    }
    if(info[7].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[7].iBurstTime);
        gUi->Process1_8->setText(info[7].cProcessName);
        gUi->BurstTime1_8->setText(cBurst);
        sprintf(cArrival,"%d",info[7].iArrivalTime);
        gUi->ArrivalTime1_8->setText(cArrival);
    }
    if(info[8].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[8].iBurstTime);
        gUi->Process1_9->setText(info[8].cProcessName);
        gUi->BurstTime1_9->setText(cBurst);
        sprintf(cArrival,"%d",info[8].iArrivalTime);
        gUi->ArrivalTime1_9->setText(cArrival);
    }
    if(info[9].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[9].iBurstTime);
        gUi->Process1_10->setText(info[9].cProcessName);
        gUi->BurstTime1_10->setText(cBurst);
        sprintf(cArrival,"%d",info[9].iArrivalTime);
        gUi->ArrivalTime1_10->setText(cArrival);
    }
    if(info[10].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[10].iBurstTime);
        gUi->Process1_11->setText(info[10].cProcessName);
        gUi->BurstTime1_11->setText(cBurst);
        sprintf(cArrival,"%d",info[10].iArrivalTime);
        gUi->ArrivalTime1_11->setText(cArrival);
    }
    if(info[11].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[11].iBurstTime);
        gUi->Process1_12->setText(info[11].cProcessName);
        gUi->BurstTime1_12->setText(cBurst);
        sprintf(cArrival,"%d",info[11].iArrivalTime);
        gUi->ArrivalTime1_12->setText(cArrival);
    }
    if(info[12].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[12].iBurstTime);
        gUi->Process1_13->setText(info[12].cProcessName);
        gUi->BurstTime1_13->setText(cBurst);
        sprintf(cArrival,"%d",info[12].iArrivalTime);
        gUi->ArrivalTime1_13->setText(cArrival);
    }
    if(info[13].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[13].iBurstTime);
        gUi->Process1_14->setText(info[13].cProcessName);
        gUi->BurstTime1_14->setText(cBurst);
        sprintf(cArrival,"%d",info[13].iArrivalTime);
        gUi->ArrivalTime1_14->setText(cArrival);
    }
}

/*void FCFS::FillProcessInfo()
{
    int i;
    for(i = 0;i<MAXIMUM_PROC;i++)
    {
        if(false == ProcessInfo[i].bFill)
        {
            ProcessInfo[i].bFill = true;
            ProcessInfo[i].iBurstTime = GenRendTime();
            ProcessInfo[i].iArrivalTime = GenRendTime();
            sprintf(ProcessInfo[i].cProcessName,"Process %d",(i+1));
            ProcessInfo[i].iProcessID = i;
            ProcessInfo[i].bInJobQ = true;
            break;
        }
        else
        {
            printf("Burst time and Arrival time is %d %d\n",ProcessInfo[i].iBurstTime,ProcessInfo[i].iBurstTime);
        }
    }
}*/
void FCFS::UpdateColourWhenInRQ()
{
    if(ProcessInfo[0].bInReadyQ)
    {
        gUi->Process1->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProcessInfo[1].bInReadyQ)
    {
        gUi->Process1_2->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProcessInfo[2].bInReadyQ)
    {
        gUi->Process1_3->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProcessInfo[3].bInReadyQ)
    {
        gUi->Process1_4->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProcessInfo[4].bInReadyQ)
    {
        gUi->Process1_5->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProcessInfo[5].bInReadyQ)
    {
        gUi->Process1_6->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProcessInfo[6].bInReadyQ)
    {
        gUi->Process1_7->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProcessInfo[7].bInReadyQ)
    {
        gUi->Process1_8->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProcessInfo[8].bInReadyQ)
    {
        gUi->Process1_9->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProcessInfo[9].bInReadyQ)
    {
        gUi->Process1_10->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProcessInfo[10].bInReadyQ)
    {
        gUi->Process1_11->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProcessInfo[11].bInReadyQ)
    {
        gUi->Process1_12->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProcessInfo[12].bInReadyQ)
    {
        gUi->Process1_13->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProcessInfo[13].bInReadyQ)
    {
        gUi->Process1_14->setStyleSheet("background-color: rgb(85, 255, 127);");
    }

}
void FCFS::UpdateColourWhenInCPU()
{
    if(ProcessInfo[0].bInCpu)
    {
        gUi->Process1->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProcessInfo[1].bInCpu)
    {
        gUi->Process1_2->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProcessInfo[2].bInCpu)
    {
        gUi->Process1_3->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProcessInfo[3].bInCpu)
    {
        gUi->Process1_4->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProcessInfo[4].bInCpu)
    {
        gUi->Process1_5->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProcessInfo[5].bInCpu)
    {
        gUi->Process1_6->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProcessInfo[6].bInCpu)
    {
        gUi->Process1_7->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProcessInfo[7].bInCpu)
    {
        gUi->Process1_8->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProcessInfo[8].bInCpu)
    {
        gUi->Process1_9->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProcessInfo[9].bInCpu)
    {
        gUi->Process1_10->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProcessInfo[10].bInCpu)
    {
        gUi->Process1_11->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProcessInfo[11].bInCpu)
    {
        gUi->Process1_12->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProcessInfo[12].bInCpu)
    {
        gUi->Process1_13->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProcessInfo[13].bInCpu)
    {
        gUi->Process1_14->setStyleSheet("background-color: rgb(255, 255, 127);");
    }

}
void FCFS::UpdateColourWhenInNormal()
{
    if(ProcessInfo[0].bInJobQ)
    {
        gUi->Process1->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
        //gUi->Process1->setStyleSheet("color: rgb(255, 255, 255);");
    }
    if(ProcessInfo[1].bInJobQ)
    {
        gUi->Process1_2->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");

    }
    if(ProcessInfo[2].bInJobQ)
    {
        gUi->Process1_3->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProcessInfo[3].bInJobQ)
    {
        gUi->Process1_4->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProcessInfo[4].bInJobQ)
    {
        gUi->Process1_5->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProcessInfo[5].bInJobQ)
    {
        gUi->Process1_6->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProcessInfo[6].bInJobQ)
    {
        gUi->Process1_7->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProcessInfo[7].bInJobQ)
    {
        gUi->Process1_8->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProcessInfo[8].bInJobQ)
    {
        gUi->Process1_9->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProcessInfo[9].bInJobQ)
    {
        gUi->Process1_10->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProcessInfo[10].bInJobQ)
    {
        gUi->Process1_11->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProcessInfo[11].bInJobQ)
    {
        gUi->Process1_12->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProcessInfo[12].bInJobQ)
    {
        gUi->Process1_13->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProcessInfo[13].bInJobQ)
    {
        gUi->Process1_14->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }

}
void FCFS::ClearLabelColour()
{
    gUi->Process1->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    gUi->Process1_2->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    gUi->Process1_3->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    gUi->Process1_4->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    gUi->Process1_5->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    gUi->Process1_6->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    gUi->Process1_7->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    gUi->Process1_8->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    gUi->Process1_9->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    gUi->Process1_10->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    gUi->Process1_11->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    gUi->Process1_12->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    gUi->Process1_13->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    gUi->Process1_14->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
}

void FCFS::UpdateDisplayRQ()
{
    int i;
    for(i = 0;i<MAXIMUM_PROC-1;i++)
    {
        strcpy(DisplayRQ[i].cProc,DisplayRQ[i+1].cProc);
        DisplayRQ[i].bFill = DisplayRQ[i+1].bFill;
        DisplayRQ[i].iProcessID = DisplayRQ[i+1].iProcessID;
    }
}

void FCFS::UpdateReadyQueue()
{
    gUi->ReadyQ1->setText(DisplayRQ[0].cProc);
    gUi->ReadyQ1_2->setText(DisplayRQ[1].cProc);
    gUi->ReadyQ1_3->setText(DisplayRQ[2].cProc);
    gUi->ReadyQ1_4->setText(DisplayRQ[3].cProc);
    gUi->ReadyQ1_5->setText(DisplayRQ[4].cProc);
    gUi->ReadyQ1_6->setText(DisplayRQ[5].cProc);
    gUi->ReadyQ1_7->setText(DisplayRQ[6].cProc);
    gUi->ReadyQ1_8->setText(DisplayRQ[7].cProc);
    gUi->ReadyQ1_9->setText(DisplayRQ[8].cProc);
    gUi->ReadyQ1_10->setText(DisplayRQ[9].cProc);
    gUi->ReadyQ1_11->setText(DisplayRQ[10].cProc);
    gUi->ReadyQ1_12->setText(DisplayRQ[11].cProc);
    gUi->ReadyQ1_13->setText(DisplayRQ[12].cProc);
    gUi->ReadyQ1_14->setText(DisplayRQ[13].cProc);
}

void FCFS::PutIntoTheCPU()
{
    printf("Putinto the CPU\n");
    bCPUBusy = true;
    gUi->CPU->setText(DisplayRQ[0].cProc);
    gUi->CPU->setStyleSheet("background-color: rgb(255, 0, 0);");
    //DisplayRQ[0].bFill = false;
    ProcessInfo[DisplayRQ[0].iProcessID].bInCpu = true;
    ProcessInfo[DisplayRQ[0].iProcessID].bInReadyQ = false;
    iProcInRQ--;
    UpdateColourWhenInCPU();
    UpdateDisplayRQ();
    UpdateReadyQueue();
}

void FCFS::FillDisplayRQ(char *proc,int iProcID)
{
    int i;
    for(i = 0;i<MAXIMUM_PROC;i++)
    {
        if(false == DisplayRQ[i].bFill)
        {
            DisplayRQ[i].bFill = true;
            strcpy(DisplayRQ[i].cProc,proc);
            DisplayRQ[i].iProcessID = iProcID;
            break;
        }
    }
}
void FCFS::Dispatcher()
{
    int i;
    for(i = 0;i<iProcessInSystem;i++)
    {
        ProcessInfo[i].iArrivalTime--;
        if(0 == ProcessInfo[i].iArrivalTime)
        {
            ProcessInfo[i].bInReadyQ = true;
            iProcInRQ++;
            ProcessInfo[i].bInJobQ = false;
            FillDisplayRQ(ProcessInfo[i].cProcessName,ProcessInfo[i].iProcessID);
            UpdateReadyQueue();
            UpdateColourWhenInRQ();
        }
    }
    if(!bCPUBusy && (0 < iProcInRQ))
    {
        PutIntoTheCPU();
    }
    if(bCPUBusy)
    {
        int i;
        for(i=0;i<iProcessInSystem;i++)
        {
            if(ProcessInfo[i].bInCpu)
            {
                char cBurst[4];
                sprintf(cBurst,"%d",ProcessInfo[i].iBurstTime);
                gUi->BTTimerLeft->setText(cBurst);
                if(0 == (ProcessInfo[i].iBurstTime--))
                {
                    bCPUBusy = false;
                    ProcessInfo[i].bInCpu = false;
                    ProcessInfo[i].bInJobQ = true;
                    UpdateColourWhenInNormal();
                }
            }
        }
    }
}
void FCFS::DisplayMessage(int iMessageType)
{
    QMessageBox msgBox;
    switch(iMessageType)
    {
         case BURST_MSG:
         msgBox.setText("Burst time less then One is not allowed");
         msgBox.exec();
         break;
         case ARRIVAL_MSG:
         msgBox.setText("Arrival time less then One is not allowed");
         msgBox.exec();
         break;
    }
}
void FCFS::CallDispatcher()
{
    Dispatcher();
}

void FCFS::on_CreateProcess_clicked()
{
    EnableWidgetAfterProcessCreation();
    FillProcessInfo(ProcStructInUse);
    gUi->ChangeBT->addItem(ProcStructInUse[iProcessInSystem].cProcessName);
    gUi->ChangeAT->addItem(ProcStructInUse[iProcessInSystem].cProcessName);
    memcpy(&ProcessInfo[iProcessInSystem],&ProcStructInUse[iProcessInSystem],sizeof(ProcessDef));
    iProcessInSystem++;
    ProcessDisplayOnHMi(ProcStructInUse);
}

void FCFS::on_BurstTime_textChanged(QString )
{
    if(iBurstIndex!=0)
    {
        char cBurst[4];
        strcpy(cBurst,ui->BurstTime->text().toStdString().c_str());
        if(atoi(cBurst) <= 0)
        {
            DisplayMessage(BURST_MSG);
            return;
        }
        ProcessInfo[iBurstIndex-1].iBurstTime = atoi(cBurst);
        ProcStructInUse[iBurstIndex-1].iBurstTime = atoi(cBurst);
        ProcessDisplayOnHMi(ProcessInfo);
    }
}

void FCFS::on_ChangeBT_highlighted(int index)
{
    iBurstIndex = index;
}

void FCFS::on_ChangeAT_highlighted(int index)
{
    iArrivalIndex = index;
}

void FCFS::on_ArrivalTime_textChanged(QString )
{
    if(iArrivalIndex != 0)
    {
        char cArrival[4];
        strcpy(cArrival,ui->ArrivalTime->text().toStdString().c_str());
        if(atoi(cArrival) <= 0)
        {
            DisplayMessage(ARRIVAL_MSG);
            return;
        }
        ProcessInfo[iArrivalIndex-1].iArrivalTime = atoi(cArrival);
        ProcStructInUse[iArrivalIndex-1].iArrivalTime = atoi(cArrival);
        ProcessDisplayOnHMi(ProcessInfo);
    }
}


void FCFS::DisableWidgetsWhenStart()
{
    gUi->CreateProcess->setEnabled(false);
    gUi->ChangeBT->setEnabled(false);
    gUi->BurstTime->setEnabled(false);
    gUi->ChangeAT->setEnabled(false);
    gUi->ArrivalTime->setEnabled(false);
}
void FCFS::EnableWidgetWhenStop()
{
    gUi->CreateProcess->setEnabled(true);
    gUi->ChangeBT->setEnabled(true);
    gUi->BurstTime->setEnabled(true);
    gUi->ChangeAT->setEnabled(true);
    gUi->ArrivalTime->setEnabled(true);
}
void FCFS::CreateTimer()
{
    Timer = new QTimer(fcfs);
    connect(Timer,SIGNAL(timeout()),fcfs,SLOT(CallDispatcher()));
    Timer->start(1000);
}
void FCFS::on_StartSim_clicked()
{
    if(!bFcfsStart)
    {
        DisableWidgetsWhenStart();
        int i = 0;
        for(i=0;i<MAXIMUM_PROC;i++)
        {
            memcpy(&ProcessInfo[i],&ProcStructInUse[i],sizeof(ProcessDef));
        }
        CreateTimer();
        gUi->StartSim->setText("Stop");
        bFcfsStart= true;
        gUi->PauseSim->setText("Pause");
        gUi->PauseSim->setEnabled(true);
    }
    else
    {
        EnableWidgetWhenStop();
        DeleteTimer();
        ClearLabelColour();
        ClearRQLabels();
        ClearCPULabels();
        ClearBTTimeLabel();
        ClearStructureForStop();
        bFcfsStart = false;
        gUi->StartSim->setText("Start");
        ProcessDisplayOnHMi(ProcStructInUse);
        gUi->PauseSim->setEnabled(false);

    }
}
void FCFS::ResetSimulator()
{
    ClearLabelColour();
    ClearProcessLabels();
    ClearBTLabels();
    ClearATLabels();
    ClearRQLabels();
    ClearCPULabels();
    ClearBTTimeLabel();
    ClearAllStructure();
    gUi->StartSim->setText("Start");
    gUi->PauseSim->setText("Pause");
}

void FCFS::on_StepSim_clicked()
{
    Dispatcher();
}

void FCFS::on_ResetSim_clicked()
{
    DeleteTimer();
    DisableWidgetsBeforeProcessCreation();
    EnableWidgetWhenStop();
    ResetSimulator();
}

void FCFS::on_PauseSim_clicked()
{
    if(!bFcfsPause)
    {
        Timer->stop();
        gUi->PauseSim->setText("Resume");
        gUi->StepSim->setEnabled(true);
        gUi->StartSim->setEnabled(false);
        bFcfsPause = true;
    }
    else
    {
        gUi->PauseSim->setText("Pause");
        Timer->start(1000);
        bFcfsPause = false;
        gUi->StepSim->setEnabled(false);
        gUi->StartSim->setEnabled(true);
    }
}
