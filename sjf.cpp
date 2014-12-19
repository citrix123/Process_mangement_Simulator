#include "sjf.h"
#include "ui_sjf.h"
#include <Sched.h>
#include <stdio.h>
#include <QTimer>
#include <QCloseEvent>
#include <os_sim.h>
#include <QMessageBox>
sjf *sjff;
bool bSJFOpen;
ProcessDef SjfProcessInfo[MAXIMUM_PROC],SjfProcStructInUse[MAXIMUM_PROC];
DispProc SjfDisplayRQ[MAXIMUM_PROC],SjfCpu;

void sjf::DisableWidgetsBeforeProcessCreation()
{
    gUi->StartSim->setEnabled(false);
    gUi->PauseSim->setEnabled(false);
    gUi->StepSim->setEnabled(false);
    gUi->ResetSim->setEnabled(false);
}
void sjf::EnableWidgetAfterProcessCreation()
{
    gUi->StartSim->setEnabled(true);
    gUi->ResetSim->setEnabled(true);
}

void sjf::ClearProcessLabels()
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
void sjf::ClearBTLabels()
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
void sjf::ClearATLabels()
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
void sjf::ClearRQLabels()
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
void sjf::ClearCPULabels()
{
    gUi->CPU->setText("");
    gUi->CPU->setStyleSheet("background-color: rgb(0, 0, 0);");
}
void sjf::ClearBTTimeLabel()
{
    gUi->BTTimerLeft->setText("");
}
void sjf::ClearAllStructure()
{
    int i;
    for(i=0;i<MAXIMUM_PROC;i++)
    {
        memset(&SjfProcessInfo[i],0,sizeof(ProcessDef));
        memset(&SjfDisplayRQ[i],0,sizeof(DispProc));
        memset(SjfProcStructInUse,0,sizeof(ProcessDef));
    }
    bCPUBusy = false;
    iProcInRQ = 0;
    iProcessInSystem = 0;
    bSjfStart = false;
    bSjfPause = false;
    bSjfReset = false;
    gUi->ChangeAT->clear();
    gUi->ChangeBT->clear();
    gUi->ChangeAT->addItem("Change Arrival Time");
    gUi->ChangeBT->addItem("Change Burst Time");
}
void OpenSjfWindow()
{
    if(!bSJFOpen)
    {
        sjff = new sjf();
        sjff->show();
        bSJFOpen = true;
    }
}

void sjf::ClearStructureForStop()
{
    int i;
    for(i=0;i<MAXIMUM_PROC;i++)
    {
        memset(&SjfProcessInfo[i],0,sizeof(ProcessDef));
        memset(&SjfDisplayRQ[i],0,sizeof(DispProc));
    }
    bCPUBusy = false;
    iProcInRQ = 0;
}

void sjf::DeleteTimer()
{
    if(NULL != Timer)
    {
        printf("Delete timer\n");
        Timer->stop();
        delete Timer;
        Timer = NULL;
    }
}
void sjf::ClearFCFSAll()
{
    bSJFOpen = false;
    bSjfPause = false;
}

void sjf::closeEvent(QCloseEvent *event)
{
    event->ignore();
    event->accept();
    ClearFCFSAll();
    DeleteTimer();
    delete sjff;

}

sjf::sjf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sjf)
{
    ui->setupUi(this);
    int i;
    bSJFOpen = false;
    bCPUBusy = false;
    bSjfPause = false;
    iProcInRQ = 0;
    iProcessInSystem = 0;
    iPreemptiveness = NONPREEMPTIVE;

    for(i=0;i<MAXIMUM_PROC;i++)
    {
        memset(&SjfProcessInfo[i],0,sizeof(ProcessDef));
        memset(&SjfDisplayRQ[i],0,sizeof(DispProc));
        memset(&SjfProcStructInUse[i],0,sizeof(ProcessDef));
    }
    gUi = ui;
    Timer = NULL;
    this->setWindowTitle("Shortest Job First Scheduling");
    DisableWidgetsBeforeProcessCreation();
}

sjf::~sjf()
{
    delete ui;
}
void sjf::ProcessDisplayOnHMi(ProcessDef *info)
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


void sjf::UpdateColourWhenInRQ()
{
    if(SjfProcessInfo[0].bInReadyQ)
    {
        gUi->Process1->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(SjfProcessInfo[1].bInReadyQ)
    {
        gUi->Process1_2->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(SjfProcessInfo[2].bInReadyQ)
    {
        gUi->Process1_3->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(SjfProcessInfo[3].bInReadyQ)
    {
        gUi->Process1_4->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(SjfProcessInfo[4].bInReadyQ)
    {
        gUi->Process1_5->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(SjfProcessInfo[5].bInReadyQ)
    {
        gUi->Process1_6->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(SjfProcessInfo[6].bInReadyQ)
    {
        gUi->Process1_7->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(SjfProcessInfo[7].bInReadyQ)
    {
        gUi->Process1_8->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(SjfProcessInfo[8].bInReadyQ)
    {
        gUi->Process1_9->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(SjfProcessInfo[9].bInReadyQ)
    {
        gUi->Process1_10->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(SjfProcessInfo[10].bInReadyQ)
    {
        gUi->Process1_11->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(SjfProcessInfo[11].bInReadyQ)
    {
        gUi->Process1_12->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(SjfProcessInfo[12].bInReadyQ)
    {
        gUi->Process1_13->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(SjfProcessInfo[13].bInReadyQ)
    {
        gUi->Process1_14->setStyleSheet("background-color: rgb(85, 255, 127);");
    }

}
void sjf::UpdateColourWhenInCPU()
{
    if(SjfProcessInfo[0].bInCpu)
    {
        gUi->Process1->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(SjfProcessInfo[1].bInCpu)
    {
        gUi->Process1_2->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(SjfProcessInfo[2].bInCpu)
    {
        gUi->Process1_3->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(SjfProcessInfo[3].bInCpu)
    {
        gUi->Process1_4->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(SjfProcessInfo[4].bInCpu)
    {
        gUi->Process1_5->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(SjfProcessInfo[5].bInCpu)
    {
        gUi->Process1_6->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(SjfProcessInfo[6].bInCpu)
    {
        gUi->Process1_7->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(SjfProcessInfo[7].bInCpu)
    {
        gUi->Process1_8->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(SjfProcessInfo[8].bInCpu)
    {
        gUi->Process1_9->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(SjfProcessInfo[9].bInCpu)
    {
        gUi->Process1_10->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(SjfProcessInfo[10].bInCpu)
    {
        gUi->Process1_11->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(SjfProcessInfo[11].bInCpu)
    {
        gUi->Process1_12->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(SjfProcessInfo[12].bInCpu)
    {
        gUi->Process1_13->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(SjfProcessInfo[13].bInCpu)
    {
        gUi->Process1_14->setStyleSheet("background-color: rgb(255, 255, 127);");
    }

}
void sjf::UpdateColourWhenInNormal()
{
    if(SjfProcessInfo[0].bInJobQ)
    {
        gUi->Process1->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
        //gUi->Process1->setStyleSheet("color: rgb(255, 255, 255);");
    }
    if(SjfProcessInfo[1].bInJobQ)
    {
        gUi->Process1_2->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");

    }
    if(SjfProcessInfo[2].bInJobQ)
    {
        gUi->Process1_3->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(SjfProcessInfo[3].bInJobQ)
    {
        gUi->Process1_4->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(SjfProcessInfo[4].bInJobQ)
    {
        gUi->Process1_5->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(SjfProcessInfo[5].bInJobQ)
    {
        gUi->Process1_6->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(SjfProcessInfo[6].bInJobQ)
    {
        gUi->Process1_7->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(SjfProcessInfo[7].bInJobQ)
    {
        gUi->Process1_8->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(SjfProcessInfo[8].bInJobQ)
    {
        gUi->Process1_9->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(SjfProcessInfo[9].bInJobQ)
    {
        gUi->Process1_10->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(SjfProcessInfo[10].bInJobQ)
    {
        gUi->Process1_11->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(SjfProcessInfo[11].bInJobQ)
    {
        gUi->Process1_12->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(SjfProcessInfo[12].bInJobQ)
    {
        gUi->Process1_13->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(SjfProcessInfo[13].bInJobQ)
    {
        gUi->Process1_14->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }

}
void sjf::ClearLabelColour()
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

void sjf::UpdateDisplayRQ()
{
    int i;
    for(i = 0;i<MAXIMUM_PROC-1;i++)
    {
        strcpy(SjfDisplayRQ[i].cProc,SjfDisplayRQ[i+1].cProc);
        SjfDisplayRQ[i].bFill = SjfDisplayRQ[i+1].bFill;
        SjfDisplayRQ[i].iProcessID = SjfDisplayRQ[i+1].iProcessID;
        SjfDisplayRQ[i].iBurst = SjfDisplayRQ[i+1].iBurst;
    }
}

void sjf::UpdateReadyQueue()
{
    gUi->ReadyQ1->setText(SjfDisplayRQ[0].cProc);
    gUi->ReadyQ1_2->setText(SjfDisplayRQ[1].cProc);
    gUi->ReadyQ1_3->setText(SjfDisplayRQ[2].cProc);
    gUi->ReadyQ1_4->setText(SjfDisplayRQ[3].cProc);
    gUi->ReadyQ1_5->setText(SjfDisplayRQ[4].cProc);
    gUi->ReadyQ1_6->setText(SjfDisplayRQ[5].cProc);
    gUi->ReadyQ1_7->setText(SjfDisplayRQ[6].cProc);
    gUi->ReadyQ1_8->setText(SjfDisplayRQ[7].cProc);
    gUi->ReadyQ1_9->setText(SjfDisplayRQ[8].cProc);
    gUi->ReadyQ1_10->setText(SjfDisplayRQ[9].cProc);
    gUi->ReadyQ1_11->setText(SjfDisplayRQ[10].cProc);
    gUi->ReadyQ1_12->setText(SjfDisplayRQ[11].cProc);
    gUi->ReadyQ1_13->setText(SjfDisplayRQ[12].cProc);
    gUi->ReadyQ1_14->setText(SjfDisplayRQ[13].cProc);
}

void sjf::PutIntoTheCPU()
{
    bCPUBusy = true;
    gUi->CPU->setText(SjfDisplayRQ[0].cProc);
    gUi->CPU->setStyleSheet("background-color: rgb(255, 0, 0);");
    SjfProcessInfo[(SjfDisplayRQ[0].iProcessID)-1].bInCpu = true;
    SjfProcessInfo[(SjfDisplayRQ[0].iProcessID)-1].bInReadyQ = false;
    memcpy(&SjfCpu,&SjfDisplayRQ[0],sizeof(DispProc));
    iProcInRQ--;
    printf("Process in ready q 222222 is %d\n",iProcInRQ);
    UpdateColourWhenInCPU();
    //if(iPreemptiveness != PREEMPTIVE)
    {
        UpdateDisplayRQ();
    }
    UpdateReadyQueue();
}

void sjf::FillDisplayRQ(ProcessDef proc)
{
    int i;
    for(i = 0;i<MAXIMUM_PROC;i++)
    {
        if(false == SjfDisplayRQ[i].bFill)
        {
            SjfDisplayRQ[i].bFill = true;
            strcpy(SjfDisplayRQ[i].cProc,proc.cProcessName);
            SjfDisplayRQ[i].iProcessID = proc.iProcessID;
            SjfDisplayRQ[i].iBurst = proc.iBurstTime;
            iProcInRQ++;
            printf("Process ID in Fill Display is %d\n",SjfDisplayRQ[i].iProcessID);
            printf("Process in ready q 11111 is %d\n",iProcInRQ);
            break;
        }
        else
        {
            if(SjfDisplayRQ[i].iProcessID == proc.iProcessID)
            {
                SjfDisplayRQ[i].bFill = true;
                strcpy(SjfDisplayRQ[i].cProc,proc.cProcessName);
                SjfDisplayRQ[i].iProcessID = proc.iProcessID;
                SjfDisplayRQ[i].iBurst = proc.iBurstTime;
                iProcInRQ++;
                break;
            }
            printf("Process ID in Fill Display is %d\n",SjfDisplayRQ[i].iProcessID);
        }
    }
    Sorting(SjfDisplayRQ,iProcInRQ,SJF);
 }


void sjf::Dispatcher()
{
    int i;
    for(i = 0;i<iProcessInSystem;i++)
    {
        SjfProcessInfo[i].iArrivalTime--;
        if((0 == SjfProcessInfo[i].iArrivalTime)/* && (false == SjfProcessInfo[i].bInReadyQ)*/)
        {
            SjfProcessInfo[i].bInReadyQ = true;
            SjfProcessInfo[i].bInJobQ = false;
            FillDisplayRQ(SjfProcessInfo[i]);
            UpdateReadyQueue();
            UpdateColourWhenInRQ();
        }
    }
    if((!bCPUBusy) && (0 < iProcInRQ))
    {
        PutIntoTheCPU();
    }
    if(bCPUBusy)
    {
        int i;
        for(i=0;i<iProcessInSystem;i++)
        {
            if(SjfProcessInfo[i].bInCpu)
            {
                char cBurst[4];
                sprintf(cBurst,"%d",SjfProcessInfo[i].iBurstTime);
                gUi->BTTimerLeft->setText(cBurst);

                if(iPreemptiveness == PREEMPTIVE)
                {
                    if(0 == SjfProcessInfo[i].iBurstTime)
                    {
                        bCPUBusy = false;
                        SjfProcessInfo[i].bInCpu = false;
                        SjfProcessInfo[i].bInJobQ = true;
                        SjfProcessInfo[i].bInReadyQ = false;
                        UpdateColourWhenInNormal();
                    }
                    else if(SjfProcessInfo[i].iBurstTime > SjfDisplayRQ[0].iBurst)
                    {
                        SjfProcessInfo[i].bInCpu = false;
                        SjfProcessInfo[i].bInReadyQ = true;
                        FillDisplayRQ(SjfProcessInfo[i]);
                        Sorting(SjfDisplayRQ,iProcInRQ,SJF);
                        UpdateColourWhenInRQ();
                        printf("Process in ready q 33333 is %d\n",iProcInRQ);
                        UpdateDisplayRQAtEveryBeat(SjfDisplayRQ,SjfProcessInfo[i]);
                        PutIntoTheCPU();
                    }
                    else
                    {
                        UpdateDisplayRQAtEveryBeat(SjfDisplayRQ,SjfProcessInfo[i]);
                        ProcessDisplayOnHMi(SjfProcessInfo);
                    }
                }
                else if(0 == (SjfProcessInfo[i].iBurstTime))
                {
                    bCPUBusy = false;
                    SjfProcessInfo[i].bInCpu = false;
                    SjfProcessInfo[i].bInJobQ = true;
                    UpdateColourWhenInNormal();
                }
                SjfProcessInfo[i].iBurstTime--;
            }
        }
    }
}

void sjf::CallDispatcher()
{
    Dispatcher();
}
void sjf::DisplayMessage(int iMessageType)
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
void sjf::on_CreateProcess_clicked()
{
    EnableWidgetAfterProcessCreation();
    FillProcessInfo(SjfProcStructInUse);
    gUi->ChangeBT->addItem(SjfProcStructInUse[iProcessInSystem].cProcessName);
    gUi->ChangeAT->addItem(SjfProcStructInUse[iProcessInSystem].cProcessName);
    memcpy(&SjfProcessInfo[iProcessInSystem],&SjfProcStructInUse[iProcessInSystem],sizeof(ProcessDef));
    iProcessInSystem++;
    ProcessDisplayOnHMi(SjfProcStructInUse);
}

void sjf::on_BurstTime_textChanged(QString )
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
        SjfProcessInfo[iBurstIndex-1].iBurstTime = atoi(cBurst);
        SjfProcStructInUse[iBurstIndex-1].iBurstTime = atoi(cBurst);
        ProcessDisplayOnHMi(SjfProcessInfo);
    }
}

void sjf::on_ChangeBT_highlighted(int index)
{
    iBurstIndex = index;
}

void sjf::on_ChangeAT_highlighted(int index)
{
    iArrivalIndex = index;
}

void sjf::on_ArrivalTime_textChanged(QString )
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
        SjfProcessInfo[iArrivalIndex-1].iArrivalTime = atoi(cArrival);
        SjfProcStructInUse[iArrivalIndex-1].iArrivalTime = atoi(cArrival);
        ProcessDisplayOnHMi(SjfProcessInfo);
    }
}

void sjf::CreateTimer()
{
    Timer = new QTimer(sjff);
    connect(Timer,SIGNAL(timeout()),sjff,SLOT(CallDispatcher()));
    Timer->start(1000);
}
void sjf::DisableWidgetsWhenStart()
{
    gUi->CreateProcess->setEnabled(false);
    gUi->ChangeBT->setEnabled(false);
    gUi->BurstTime->setEnabled(false);
    gUi->ChangeAT->setEnabled(false);
    gUi->ArrivalTime->setEnabled(false);
    gUi->Preemptiveness->setEnabled(false);
}
void sjf::EnableWidgetWhenStop()
{
    gUi->CreateProcess->setEnabled(true);
    gUi->ChangeBT->setEnabled(true);
    gUi->BurstTime->setEnabled(true);
    gUi->ChangeAT->setEnabled(true);
    gUi->ArrivalTime->setEnabled(true);
    gUi->Preemptiveness->setEnabled(false);
}
void sjf::on_StartSim_clicked()
{
    if(!bSjfStart)
    {
        DisableWidgetsWhenStart();
        int i = 0;
        for(i=0;i<MAXIMUM_PROC;i++)
        {
            memcpy(&SjfProcessInfo[i],&SjfProcStructInUse[i],sizeof(ProcessDef));
        }
        CreateTimer();
        gUi->StartSim->setText("Stop");
        bSjfStart= true;
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
        bSjfStart = false;
        gUi->StartSim->setText("Start");
        ProcessDisplayOnHMi(SjfProcStructInUse);
        gUi->PauseSim->setEnabled(false);

    }
}
void sjf::ResetSimulator()
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

void sjf::on_StopSim_clicked()
{
}

void sjf::on_StepSim_clicked()
{
    Dispatcher();

}

void sjf::on_ResetSim_clicked()
{
    DisableWidgetsBeforeProcessCreation();
    EnableWidgetWhenStop();
    DeleteTimer();
    ResetSimulator();
}

void sjf::on_PauseSim_clicked()
{
    if(!bSjfPause)
    {
        Timer->stop();
        gUi->PauseSim->setText("Resume");
        gUi->StepSim->setEnabled(true);
        gUi->StartSim->setEnabled(false);
        bSjfPause = true;
    }
    else
    {
        gUi->PauseSim->setText("Pause");
        Timer->start(1000);
        bSjfPause = false;
        gUi->StepSim->setEnabled(false);
        gUi->StartSim->setEnabled(true);
    }
}

void sjf::on_Preemptiveness_highlighted(int index)
{
    if(index == 1)
    {
        iPreemptiveness = PREEMPTIVE;
    }
    else
    {
        iPreemptiveness = NONPREEMPTIVE;
    }
}
