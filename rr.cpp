#include "rr.h"
#include "ui_rr.h"
#include <Sched.h>
#include <stdio.h>
#include <QTimer>
#include <QCloseEvent>
#include <QMessageBox>
#include <os_sim.h>
rr *pRr;
bool bRROpen;
ProcessDef RrProcessInfo[MAXIMUM_PROC],RrProcStructInUse[MAXIMUM_PROC];
DispProc RrDisplayRQ[MAXIMUM_PROC],RrCpu;

void rr::DisableWidgetsBeforeProcessCreation()
{
    gUi->StartSim->setEnabled(false);
    gUi->PauseSim->setEnabled(false);
    gUi->StepSim->setEnabled(false);
    gUi->ResetSim->setEnabled(false);
}
void rr::EnableWidgetAfterProcessCreation()
{
    gUi->StartSim->setEnabled(true);
    gUi->ResetSim->setEnabled(true);
}
rr::rr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rr)
{
    ui->setupUi(this);
    int i;
    bRROpen = false;
    bCPUBusy = false;
    bRRPause = false;
    bRRStart = false;
    bRRReset = false;
    iBurstIndex=iArrivalIndex=iProcInRQ=iProcessInSystem=iTempTimeQuantum=iTimeQuantum = 0;
    for(i=0;i<MAXIMUM_PROC;i++)
    {
        memset(&RrProcessInfo[i],0,sizeof(ProcessDef));
        memset(&RrDisplayRQ[i],0,sizeof(DispProc));
        memset(&RrProcStructInUse[i],0,sizeof(ProcessDef));
    }
    gUi = ui;
    Timer = NULL;
    this->setWindowTitle("Round Robin Scheduling");
    DisableWidgetsBeforeProcessCreation();

}

rr::~rr()
{
    delete ui;
}
void rr::ClearProcessLabels()
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
void rr::ClearBTLabels()
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
void rr::ClearATLabels()
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
void rr::ClearRQLabels()
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
void rr::ClearCPULabels()
{
    gUi->CPU->setText("");
    gUi->CPU->setStyleSheet("background-color: rgb(0, 0, 0);");
}
void rr::ClearBTTimeLabel()
{
    gUi->BTTimerLeft->setText("");
}
void rr::ClearAllStructure()
{
    int i;
    for(i=0;i<MAXIMUM_PROC;i++)
    {
        memset(&RrProcessInfo[i],0,sizeof(ProcessDef));
        memset(&RrDisplayRQ[i],0,sizeof(DispProc));
        memset(RrProcStructInUse,0,sizeof(ProcessDef));
    }
    bCPUBusy = false;
    bRRPause = false;
    bRRStart = false;
    bRRReset = false;
    iBurstIndex=iArrivalIndex=iProcInRQ=iProcessInSystem=iTempTimeQuantum=iTimeQuantum = 0;
    gUi->ChangeAT->clear();
    gUi->ChangeBT->clear();
    gUi->ChangeAT->addItem("Change Arrival Time");
    gUi->ChangeBT->addItem("Change Burst Time");

}

void OpenRRWindow()
{
    if(!bRROpen)
    {
        pRr = new rr();
        pRr->show();
        bRROpen = true;
    }
}
void rr::ClearStructureForStop()
{
    int i;
    for(i=0;i<MAXIMUM_PROC;i++)
    {
        memset(&RrProcessInfo[i],0,sizeof(ProcessDef));
        memset(&RrDisplayRQ[i],0,sizeof(DispProc));
    }
    bCPUBusy = false;
    iProcInRQ = 0;
}

void rr::DeleteTimer()
{
    if(NULL != Timer)
    {
        Timer->stop();
        delete Timer;
        Timer = NULL;
    }
}
void rr::ClearFCFSAll()
{
    bRROpen = false;
    bRRPause = false;
}

void rr::closeEvent(QCloseEvent *event)
{
    event->ignore();
    event->accept();
    ClearFCFSAll();
    DeleteTimer();
    delete pRr;

}
void rr::ProcessDisplayOnHMi(ProcessDef *info)
{
    if(info[0].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[0].iBurstTime);
        gUi->Process1->setText(info[0].cProcessName);
        gUi->BurstTime1->setText(cBurst);
        if(info[0].iArrivalTime>0)
        {
            sprintf(cArrival,"%d",info[0].iArrivalTime);
            gUi->ArrivalTime1->setText(cArrival);
        }
    }
    if(info[1].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[1].iBurstTime);
        gUi->Process1_2->setText(info[1].cProcessName);
        gUi->BurstTime1_2->setText(cBurst);
        if(info[1].iArrivalTime>0)
        {
        sprintf(cArrival,"%d",info[1].iArrivalTime);
        gUi->ArrivalTime1_2->setText(cArrival);
        }
    }
    if(info[2].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[2].iBurstTime);
        gUi->Process1_3->setText(info[2].cProcessName);
        gUi->BurstTime1_3->setText(cBurst);
        if(info[2].iArrivalTime>0)
        {
        sprintf(cArrival,"%d",info[2].iArrivalTime);
        gUi->ArrivalTime1_3->setText(cArrival);
        }
    }
    if(info[3].bFill)
    {
        char cBurst[4],cArrival[4];
        sprintf(cBurst,"%d",info[3].iBurstTime);
        gUi->Process1_4->setText(info[3].cProcessName);
        gUi->BurstTime1_4->setText(cBurst);
        if(info[3].iArrivalTime>0)
        {
        sprintf(cArrival,"%d",info[3].iArrivalTime);
        gUi->ArrivalTime1_4->setText(cArrival);
        }
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


void rr::UpdateColourWhenInRQ()
{
    if(RrProcessInfo[0].bInReadyQ)
    {
        gUi->Process1->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(RrProcessInfo[1].bInReadyQ)
    {
        gUi->Process1_2->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(RrProcessInfo[2].bInReadyQ)
    {
        gUi->Process1_3->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(RrProcessInfo[3].bInReadyQ)
    {
        gUi->Process1_4->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(RrProcessInfo[4].bInReadyQ)
    {
        gUi->Process1_5->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(RrProcessInfo[5].bInReadyQ)
    {
        gUi->Process1_6->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(RrProcessInfo[6].bInReadyQ)
    {
        gUi->Process1_7->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(RrProcessInfo[7].bInReadyQ)
    {
        gUi->Process1_8->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(RrProcessInfo[8].bInReadyQ)
    {
        gUi->Process1_9->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(RrProcessInfo[9].bInReadyQ)
    {
        gUi->Process1_10->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(RrProcessInfo[10].bInReadyQ)
    {
        gUi->Process1_11->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(RrProcessInfo[11].bInReadyQ)
    {
        gUi->Process1_12->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(RrProcessInfo[12].bInReadyQ)
    {
        gUi->Process1_13->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(RrProcessInfo[13].bInReadyQ)
    {
        gUi->Process1_14->setStyleSheet("background-color: rgb(85, 255, 127);");
    }

}
void rr::UpdateColourWhenInCPU()
{
    if(RrProcessInfo[0].bInCpu)
    {
        gUi->Process1->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(RrProcessInfo[1].bInCpu)
    {
        gUi->Process1_2->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(RrProcessInfo[2].bInCpu)
    {
        gUi->Process1_3->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(RrProcessInfo[3].bInCpu)
    {
        gUi->Process1_4->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(RrProcessInfo[4].bInCpu)
    {
        gUi->Process1_5->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(RrProcessInfo[5].bInCpu)
    {
        gUi->Process1_6->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(RrProcessInfo[6].bInCpu)
    {
        gUi->Process1_7->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(RrProcessInfo[7].bInCpu)
    {
        gUi->Process1_8->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(RrProcessInfo[8].bInCpu)
    {
        gUi->Process1_9->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(RrProcessInfo[9].bInCpu)
    {
        gUi->Process1_10->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(RrProcessInfo[10].bInCpu)
    {
        gUi->Process1_11->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(RrProcessInfo[11].bInCpu)
    {
        gUi->Process1_12->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(RrProcessInfo[12].bInCpu)
    {
        gUi->Process1_13->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(RrProcessInfo[13].bInCpu)
    {
        gUi->Process1_14->setStyleSheet("background-color: rgb(255, 255, 127);");
    }

}
void rr::UpdateColourWhenInNormal()
{
    if(RrProcessInfo[0].bInJobQ)
    {
        gUi->Process1->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
        //gUi->Process1->setStyleSheet("color: rgb(255, 255, 255);");
    }
    if(RrProcessInfo[1].bInJobQ)
    {
        gUi->Process1_2->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");

    }
    if(RrProcessInfo[2].bInJobQ)
    {
        gUi->Process1_3->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(RrProcessInfo[3].bInJobQ)
    {
        gUi->Process1_4->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(RrProcessInfo[4].bInJobQ)
    {
        gUi->Process1_5->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(RrProcessInfo[5].bInJobQ)
    {
        gUi->Process1_6->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(RrProcessInfo[6].bInJobQ)
    {
        gUi->Process1_7->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(RrProcessInfo[7].bInJobQ)
    {
        gUi->Process1_8->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(RrProcessInfo[8].bInJobQ)
    {
        gUi->Process1_9->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(RrProcessInfo[9].bInJobQ)
    {
        gUi->Process1_10->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(RrProcessInfo[10].bInJobQ)
    {
        gUi->Process1_11->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(RrProcessInfo[11].bInJobQ)
    {
        gUi->Process1_12->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(RrProcessInfo[12].bInJobQ)
    {
        gUi->Process1_13->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(RrProcessInfo[13].bInJobQ)
    {
        gUi->Process1_14->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }

}
void rr::ClearLabelColour()
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

void rr::UpdateDisplayRQ()
{
    int i;
    for(i = 0;i<MAXIMUM_PROC-1;i++)
    {
        strcpy(RrDisplayRQ[i].cProc,RrDisplayRQ[i+1].cProc);
        RrDisplayRQ[i].bFill = RrDisplayRQ[i+1].bFill;
        RrDisplayRQ[i].iProcessID = RrDisplayRQ[i+1].iProcessID;
        RrDisplayRQ[i].iBurst = RrDisplayRQ[i+1].iBurst;
    }
}

void rr::UpdateReadyQueue()
{
    gUi->ReadyQ1->setText(RrDisplayRQ[0].cProc);
    gUi->ReadyQ1_2->setText(RrDisplayRQ[1].cProc);
    gUi->ReadyQ1_3->setText(RrDisplayRQ[2].cProc);
    gUi->ReadyQ1_4->setText(RrDisplayRQ[3].cProc);
    gUi->ReadyQ1_5->setText(RrDisplayRQ[4].cProc);
    gUi->ReadyQ1_6->setText(RrDisplayRQ[5].cProc);
    gUi->ReadyQ1_7->setText(RrDisplayRQ[6].cProc);
    gUi->ReadyQ1_8->setText(RrDisplayRQ[7].cProc);
    gUi->ReadyQ1_9->setText(RrDisplayRQ[8].cProc);
    gUi->ReadyQ1_10->setText(RrDisplayRQ[9].cProc);
    gUi->ReadyQ1_11->setText(RrDisplayRQ[10].cProc);
    gUi->ReadyQ1_12->setText(RrDisplayRQ[11].cProc);
    gUi->ReadyQ1_13->setText(RrDisplayRQ[12].cProc);
    gUi->ReadyQ1_14->setText(RrDisplayRQ[13].cProc);
}

void rr::PutIntoTheCPU()
{
    bCPUBusy = true;
    gUi->CPU->setText(RrDisplayRQ[0].cProc);
    gUi->CPU->setStyleSheet("background-color: rgb(255, 0, 0);");
    RrProcessInfo[(RrDisplayRQ[0].iProcessID)-1].bInCpu = true;
    RrProcessInfo[(RrDisplayRQ[0].iProcessID)-1].bInReadyQ = false;
    memcpy(&RrCpu,&RrDisplayRQ[0],sizeof(DispProc));
    iProcInRQ--;
    UpdateColourWhenInCPU();
    UpdateDisplayRQ();
    UpdateReadyQueue();
}

void rr::FillDisplayRQ(ProcessDef proc)
{
    int i;
    for(i = 0;i<MAXIMUM_PROC;i++)
    {
        if(false == RrDisplayRQ[i].bFill)
        {
            RrDisplayRQ[i].bFill = true;
            strcpy(RrDisplayRQ[i].cProc,proc.cProcessName);
            RrDisplayRQ[i].iProcessID = proc.iProcessID;
            RrDisplayRQ[i].iBurst = proc.iBurstTime;
            iProcInRQ++;
            printf("Process ID in Fill Display is %d\n",RrDisplayRQ[i].iProcessID);
            break;
        }
    }
 }
void rr::BurstTimeDisplay(int i)
{
        char cBurst[4];
        RrProcessInfo[i].iBurstTime--;
        sprintf(cBurst,"%d",RrProcessInfo[i].iBurstTime);
        gUi->BTTimerLeft->setText(cBurst);
}

void rr::Dispatcher()
{
    int i;
    for(i = 0;i<iProcessInSystem;i++)
    {
        RrProcessInfo[i].iArrivalTime--;
        if((0 == RrProcessInfo[i].iArrivalTime))
        {
            RrProcessInfo[i].bInReadyQ = true;
            RrProcessInfo[i].bInJobQ = false;
            FillDisplayRQ(RrProcessInfo[i]);
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
            if(RrProcessInfo[i].bInCpu)
            {
                BurstTimeDisplay(i);
                if(iTempTimeQuantum>0)
                {
                    iTempTimeQuantum--;
                    printf("iTempTimeQuantum is 1 %d %d %d\n",RrProcessInfo[i].iProcessID,
                           RrProcessInfo[i].iBurstTime,iTempTimeQuantum);
                }
                //if(iPreemptiveness == PREEMPTIVE)
                {
                    if(0 == RrProcessInfo[i].iBurstTime)
                    {
                        bCPUBusy = false;
                        RrProcessInfo[i].bInCpu = false;
                        RrProcessInfo[i].bInJobQ = true;
                        RrProcessInfo[i].bInReadyQ = false;
                        UpdateColourWhenInNormal();
                        iTempTimeQuantum = 0;
                    }
                    else if(iTempTimeQuantum == 0)
                    {
                        iTempTimeQuantum = iTimeQuantum;
                        RrProcessInfo[i].bInCpu = false;
                        RrProcessInfo[i].bInReadyQ = true;
                        FillDisplayRQ(RrProcessInfo[i]);
                        UpdateColourWhenInRQ();
                        printf("Process in ready q 33333 is%d  %d\n",RrProcessInfo[i].iProcessID,RrProcessInfo[i].iBurstTime);
                        UpdateDisplayRQAtEveryBeat(RrDisplayRQ,RrProcessInfo[i]);
                        ProcessDisplayOnHMi(RrProcessInfo);
                        bCPUBusy = false;
                        gUi->BTTimerLeft->setText("");
                        //PutIntoTheCPU();
                        break;
                    }
                    else
                    {
                        UpdateDisplayRQAtEveryBeat(RrDisplayRQ,RrProcessInfo[i]);
                        ProcessDisplayOnHMi(RrProcessInfo);
                    }
                }
             }
        }
    }
}
bool rr::CheckQValueBeforeStart()
{
    if(!l.empty())
    {
        qSort(l.begin(),l.end());
        if(iTimeQuantum == 0)
        {
            DisplayMessage(QVALUE_ZERO);
            return false;
        }
        else if(iTimeQuantum > l.first())
        {
            DisplayMessage(QVALUE_OUT_RANGE);
            return false;
        }
        return true;
    }
}

void rr::CallDispatcher()
{
    Dispatcher();
}
void rr::DisplayMessage(int iMessageType)
{
    QMessageBox msgBox;
    switch(iMessageType)
    {
         case QVALUE_ZERO:
         msgBox.setText("Quantum time is NULL. Please set Quantum time greater then 0.");
         msgBox.exec();
         break;
         case QVALUE_OUT_RANGE:
         msgBox.setText("Quantum time is Out of range.Please give less then or equal to smallest burst time");
         msgBox.exec();
         break;
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
void rr::on_CreateProcess_clicked()
{
    EnableWidgetAfterProcessCreation();
    FillProcessInfo(RrProcStructInUse);
    gUi->ChangeBT->addItem(RrProcStructInUse[iProcessInSystem].cProcessName);
    gUi->ChangeAT->addItem(RrProcStructInUse[iProcessInSystem].cProcessName);
    l.push_back(RrProcStructInUse[iProcessInSystem].iBurstTime);
    memcpy(&RrProcessInfo[iProcessInSystem],&RrProcStructInUse[iProcessInSystem],sizeof(ProcessDef));
    iProcessInSystem++;
    ProcessDisplayOnHMi(RrProcStructInUse);
}

void rr::on_BurstTime_textChanged(QString )
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
        RrProcessInfo[iBurstIndex-1].iBurstTime = atoi(cBurst);
        RrProcStructInUse[iBurstIndex-1].iBurstTime = atoi(cBurst);
        l.push_back(atoi(cBurst));
        ProcessDisplayOnHMi(RrProcessInfo);
    }
}

void rr::on_ChangeBT_highlighted(int index)
{
    iBurstIndex = index;
}

void rr::on_ChangeAT_highlighted(int index)
{
    iArrivalIndex = index;
}

void rr::on_ArrivalTime_textChanged(QString )
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
        RrProcessInfo[iArrivalIndex-1].iArrivalTime = atoi(cArrival);
        RrProcStructInUse[iArrivalIndex-1].iArrivalTime = atoi(cArrival);
        ProcessDisplayOnHMi(RrProcessInfo);
    }
}

void rr::CreateTimer()
{
    Timer = new QTimer(pRr);
    connect(Timer,SIGNAL(timeout()),pRr,SLOT(CallDispatcher()));
    Timer->start(1000);
}
void rr::DisableWidgetsWhenStart()
{
    gUi->CreateProcess->setEnabled(false);
    gUi->ChangeBT->setEnabled(false);
    gUi->BurstTime->setEnabled(false);
    gUi->ChangeAT->setEnabled(false);
    gUi->ArrivalTime->setEnabled(false);
    gUi->TimeQuantum->setEnabled(false);
}
void rr::EnableWidgetWhenStop()
{
    gUi->CreateProcess->setEnabled(true);
    gUi->ChangeBT->setEnabled(true);
    gUi->BurstTime->setEnabled(true);
    gUi->ChangeAT->setEnabled(true);
    gUi->ArrivalTime->setEnabled(true);
    gUi->TimeQuantum->setEnabled(true);
}
void rr::on_StartSim_clicked()
{
    if(CheckQValueBeforeStart())
    {
        if(!bRRStart)
        {
            DisableWidgetsWhenStart();
            int i = 0;
            for(i=0;i<MAXIMUM_PROC;i++)
            {
                memcpy(&RrProcessInfo[i],&RrProcStructInUse[i],sizeof(ProcessDef));
            }
            CreateTimer();
            gUi->StartSim->setText("Stop");
            bRRStart= true;
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
            bRRStart = false;
            gUi->StartSim->setText("Start");
            ProcessDisplayOnHMi(RrProcStructInUse);
            gUi->PauseSim->setEnabled(false);

        }
    }
    else
    {
        return;
    }
}
void rr::ResetSimulator()
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

void rr::on_StepSim_clicked()
{
    if(CheckQValueBeforeStart())
    Dispatcher();

}

void rr::on_ResetSim_clicked()
{
    DisableWidgetsBeforeProcessCreation();
    EnableWidgetWhenStop();
    DeleteTimer();
    ResetSimulator();
}

void rr::on_PauseSim_clicked()
{
    if(!bRRPause)
    {
        Timer->stop();
        gUi->PauseSim->setText("Resume");
        gUi->StepSim->setEnabled(true);
        gUi->StartSim->setEnabled(false);
        bRRPause = true;
    }
    else
    {
        gUi->PauseSim->setText("Pause");
        Timer->start(1000);
        bRRPause = false;
        gUi->StepSim->setEnabled(false);
        gUi->StartSim->setEnabled(true);
    }
}

void rr::on_TimeQuantum_textChanged(QString )
{
    char cQuantum[4];
    strcpy(cQuantum,ui->TimeQuantum->text().toStdString().c_str());
    qSort(l.begin(),l.end());
    if(atoi(cQuantum) > l.first())
    {
        DisplayMessage(QVALUE_OUT_RANGE);
        return;
    }
    iTimeQuantum = iTempTimeQuantum = atoi(cQuantum);
}
