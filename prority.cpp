#include "prority.h"
#include "ui_prority.h"
#include <Sched.h>
#include <QCloseEvent>
#include <stdio.h>
#include <QTimer>
#include <os_sim.h>
#include <QMessageBox>
prority *pro;
bool bProOpen = false;

ProcessDef ProProcessInfo[MAXIMUM_PROC],ProProcStructInUse[MAXIMUM_PROC];
DispProc ProDisplayRQ[MAXIMUM_PROC],ProCpu;
void prority::DisableWidgetsBeforeProcessCreation()
{
    gUi->StartSim->setEnabled(false);
    gUi->PauseSim->setEnabled(false);
    gUi->StepSim->setEnabled(false);
    gUi->ResetSim->setEnabled(false);
}
void prority::EnableWidgetAfterProcessCreation()
{
    gUi->StartSim->setEnabled(true);
    gUi->ResetSim->setEnabled(true);
}
void prority::ClearProcessLabels()
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
void prority::ClearBTLabels()
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
void prority::ClearProLabels()
{
    gUi->Priority1->setText("");
    gUi->Priority1_2->setText("");
    gUi->Priority1_3->setText("");
    gUi->Priority1_4->setText("");
    gUi->Priority1_5->setText("");
    gUi->Priority1_6->setText("");
    gUi->Priority1_7->setText("");
    gUi->Priority1_8->setText("");
    gUi->Priority1_9->setText("");
    gUi->Priority1_10->setText("");
    gUi->Priority1_11->setText("");
    gUi->Priority1_12->setText("");
    gUi->Priority1_13->setText("");
    gUi->Priority1_14->setText("");
}

void prority::ClearATLabels()
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
void prority::ClearRQLabels()
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
void prority::ClearCPULabels()
{
    gUi->CPU->setText("");
    gUi->CPU->setStyleSheet("background-color: rgb(0, 0, 0);");
}
void prority::ClearBTTimeLabel()
{
    gUi->BTTimerLeft->setText("");
}
void prority::ClearAllStructure()
{
    int i;
    for(i=0;i<MAXIMUM_PROC;i++)
    {
        memset(&ProProcessInfo[i],0,sizeof(ProcessDef));
        memset(&ProDisplayRQ[i],0,sizeof(DispProc));
        memset(&ProProcStructInUse,0,sizeof(ProcessDef));
    }
    bCPUBusy = false;
    iProcInRQ = 0;
    iProcessInSystem = 0;
    bProStart = false;
    bProPause = false;
    bProReset = false;
    gUi->ChangeAT->clear();
    gUi->ChangeBT->clear();
    gUi->ChangeAT->addItem("Change Arrival Time");
    gUi->ChangeBT->addItem("Change Burst Time");
}


void prority::ClearStructureForStop()
{
    int i;
    for(i=0;i<MAXIMUM_PROC;i++)
    {
        memset(&ProProcessInfo[i],0,sizeof(ProcessDef));
        memset(&ProDisplayRQ[i],0,sizeof(DispProc));
    }
    bCPUBusy = false;
    iProcInRQ = 0;
}

void prority::DeleteTimer()
{
    if(NULL != Timer)
    {
        printf("Delete timer\n");
        Timer->stop();
        delete Timer;
        Timer = NULL;
    }
}
void prority::ClearFCFSAll()
{
    bProOpen = false;
    bProPause = false;
}

void prority::closeEvent(QCloseEvent *event)
{
    event->ignore();
    event->accept();
    ClearFCFSAll();
    DeleteTimer();
    delete pro;

}


prority::prority(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::prority)
{
    ui->setupUi(this);
    bProOpen = false;
    int i;
    bCPUBusy = false;
    bProPause = false;
    iProcInRQ = 0;
    iProcessInSystem = 0;
    iPreemptiveness = NONPREEMPTIVE;
    for(i=0;i<MAXIMUM_PROC;i++)
    {
        memset(&ProProcessInfo[i],0,sizeof(ProcessDef));
        memset(&ProDisplayRQ[i],0,sizeof(DispProc));
        memset(&ProProcStructInUse[i],0,sizeof(ProcessDef));
    }
    gUi = ui;
    Timer = NULL;
    this->setWindowTitle("Priority Scheduling");
    DisableWidgetsBeforeProcessCreation();
}
void OpenPriorityWIndow()
{
    if(!bProOpen)
    {
        pro = new prority();
        pro->show();
        bProOpen = true;
    }
}

prority::~prority()
{
    delete ui;
}


void prority::ProcessDisplayOnHMi(ProcessDef *info)
{
    if(info[0].bFill)
    {
        char cBurst[4],cArrival[4],cPriority[4];
        sprintf(cBurst,"%d",info[0].iBurstTime);
        gUi->Process1->setText(info[0].cProcessName);
        gUi->BurstTime1->setText(cBurst);
        sprintf(cArrival,"%d",info[0].iArrivalTime);
        gUi->ArrivalTime1->setText(cArrival);
        sprintf(cPriority,"%d",info[0].iPriority);
        gUi->Priority1->setText(cPriority);
    }
    if(info[1].bFill)
    {
        char cBurst[4],cArrival[4],cPriority[4];
        sprintf(cBurst,"%d",info[1].iBurstTime);
        gUi->Process1_2->setText(info[1].cProcessName);
        gUi->BurstTime1_2->setText(cBurst);
        sprintf(cArrival,"%d",info[1].iArrivalTime);
        gUi->ArrivalTime1_2->setText(cArrival);
        sprintf(cPriority,"%d",info[1].iPriority);
        gUi->Priority1_2->setText(cPriority);
    }
    if(info[2].bFill)
    {
        char cBurst[4],cArrival[4],cPriority[4];
        sprintf(cBurst,"%d",info[2].iBurstTime);
        gUi->Process1_3->setText(info[2].cProcessName);
        gUi->BurstTime1_3->setText(cBurst);
        sprintf(cArrival,"%d",info[2].iArrivalTime);
        gUi->ArrivalTime1_3->setText(cArrival);
        sprintf(cPriority,"%d",info[2].iPriority);
        gUi->Priority1_3->setText(cPriority);
    }
    if(info[3].bFill)
    {
        char cBurst[4],cArrival[4],cPriority[4];
        sprintf(cBurst,"%d",info[3].iBurstTime);
        gUi->Process1_4->setText(info[3].cProcessName);
        gUi->BurstTime1_4->setText(cBurst);
        sprintf(cArrival,"%d",info[3].iArrivalTime);
        gUi->ArrivalTime1_4->setText(cArrival);
        sprintf(cPriority,"%d",info[3].iPriority);
        gUi->Priority1_4->setText(cPriority);
    }
    if(info[4].bFill)
    {
        char cBurst[4],cArrival[4],cPriority[4];
        sprintf(cBurst,"%d",info[4].iBurstTime);
        gUi->Process1_5->setText(info[4].cProcessName);
        gUi->BurstTime1_5->setText(cBurst);
        sprintf(cArrival,"%d",info[4].iArrivalTime);
        gUi->ArrivalTime1_5->setText(cArrival);
        sprintf(cPriority,"%d",info[4].iPriority);
        gUi->Priority1_5->setText(cPriority);
    }
    if(info[5].bFill)
    {
        char cBurst[4],cArrival[4],cPriority[4];
        sprintf(cBurst,"%d",info[5].iBurstTime);
        gUi->Process1_6->setText(info[5].cProcessName);
        gUi->BurstTime1_6->setText(cBurst);
        sprintf(cArrival,"%d",info[5].iArrivalTime);
        gUi->ArrivalTime1_6->setText(cArrival);
        sprintf(cPriority,"%d",info[5].iPriority);
        gUi->Priority1_6->setText(cPriority);
    }
    if(info[6].bFill)
    {
        char cBurst[4],cArrival[4],cPriority[4];
        sprintf(cBurst,"%d",info[6].iBurstTime);
        gUi->Process1_7->setText(info[6].cProcessName);
        gUi->BurstTime1_7->setText(cBurst);
        sprintf(cArrival,"%d",info[6].iArrivalTime);
        gUi->ArrivalTime1_7->setText(cArrival);
        sprintf(cPriority,"%d",info[6].iPriority);
        gUi->Priority1_7->setText(cPriority);
    }
    if(info[7].bFill)
    {
        char cBurst[4],cArrival[4],cPriority[4];
        sprintf(cBurst,"%d",info[7].iBurstTime);
        gUi->Process1_8->setText(info[7].cProcessName);
        gUi->BurstTime1_8->setText(cBurst);
        sprintf(cArrival,"%d",info[7].iArrivalTime);
        gUi->ArrivalTime1_8->setText(cArrival);
        sprintf(cPriority,"%d",info[7].iPriority);
        gUi->Priority1_8->setText(cPriority);
    }
    if(info[8].bFill)
    {
        char cBurst[4],cArrival[4],cPriority[4];
        sprintf(cBurst,"%d",info[8].iBurstTime);
        gUi->Process1_9->setText(info[8].cProcessName);
        gUi->BurstTime1_9->setText(cBurst);
        sprintf(cArrival,"%d",info[8].iArrivalTime);
        gUi->ArrivalTime1_9->setText(cArrival);
        sprintf(cPriority,"%d",info[8].iPriority);
        gUi->Priority1_9->setText(cPriority);
    }
    if(info[9].bFill)
    {
        char cBurst[4],cArrival[4],cPriority[4];
        sprintf(cBurst,"%d",info[9].iBurstTime);
        gUi->Process1_10->setText(info[9].cProcessName);
        gUi->BurstTime1_10->setText(cBurst);
        sprintf(cArrival,"%d",info[9].iArrivalTime);
        gUi->ArrivalTime1_10->setText(cArrival);
        sprintf(cPriority,"%d",info[9].iPriority);
        gUi->Priority1_10->setText(cPriority);
    }
    if(info[10].bFill)
    {
        char cBurst[4],cArrival[4],cPriority[4];
        sprintf(cBurst,"%d",info[10].iBurstTime);
        gUi->Process1_11->setText(info[10].cProcessName);
        gUi->BurstTime1_11->setText(cBurst);
        sprintf(cArrival,"%d",info[10].iArrivalTime);
        gUi->ArrivalTime1_11->setText(cArrival);
        sprintf(cPriority,"%d",info[10].iPriority);
        gUi->Priority1_11->setText(cPriority);
    }
    if(info[11].bFill)
    {
        char cBurst[4],cArrival[4],cPriority[4];
        sprintf(cBurst,"%d",info[11].iBurstTime);
        gUi->Process1_12->setText(info[11].cProcessName);
        gUi->BurstTime1_12->setText(cBurst);
        sprintf(cArrival,"%d",info[11].iArrivalTime);
        gUi->ArrivalTime1_12->setText(cArrival);
        sprintf(cPriority,"%d",info[11].iPriority);
        gUi->Priority1_12->setText(cPriority);
    }
    if(info[12].bFill)
    {
        char cBurst[4],cArrival[4],cPriority[4];
        sprintf(cBurst,"%d",info[12].iBurstTime);
        gUi->Process1_13->setText(info[12].cProcessName);
        gUi->BurstTime1_13->setText(cBurst);
        sprintf(cArrival,"%d",info[12].iArrivalTime);
        gUi->ArrivalTime1_13->setText(cArrival);
        sprintf(cPriority,"%d",info[12].iPriority);
        gUi->Priority1_13->setText(cPriority);
    }
    if(info[13].bFill)
    {
        char cBurst[4],cArrival[4],cPriority[4];
        sprintf(cBurst,"%d",info[13].iBurstTime);
        gUi->Process1_14->setText(info[13].cProcessName);
        gUi->BurstTime1_14->setText(cBurst);
        sprintf(cArrival,"%d",info[13].iArrivalTime);
        gUi->ArrivalTime1_14->setText(cArrival);
        sprintf(cPriority,"%d",info[13].iPriority);
        gUi->Priority1_14->setText(cPriority);
    }
}


void prority::UpdateColourWhenInRQ()
{
    if(ProProcessInfo[0].bInReadyQ)
    {
        gUi->Process1->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProProcessInfo[1].bInReadyQ)
    {
        gUi->Process1_2->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProProcessInfo[2].bInReadyQ)
    {
        gUi->Process1_3->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProProcessInfo[3].bInReadyQ)
    {
        gUi->Process1_4->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProProcessInfo[4].bInReadyQ)
    {
        gUi->Process1_5->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProProcessInfo[5].bInReadyQ)
    {
        gUi->Process1_6->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProProcessInfo[6].bInReadyQ)
    {
        gUi->Process1_7->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProProcessInfo[7].bInReadyQ)
    {
        gUi->Process1_8->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProProcessInfo[8].bInReadyQ)
    {
        gUi->Process1_9->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProProcessInfo[9].bInReadyQ)
    {
        gUi->Process1_10->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProProcessInfo[10].bInReadyQ)
    {
        gUi->Process1_11->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProProcessInfo[11].bInReadyQ)
    {
        gUi->Process1_12->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProProcessInfo[12].bInReadyQ)
    {
        gUi->Process1_13->setStyleSheet("background-color: rgb(85, 255, 127);");
    }
    if(ProProcessInfo[13].bInReadyQ)
    {
        gUi->Process1_14->setStyleSheet("background-color: rgb(85, 255, 127);");
    }

}
void prority::UpdateColourWhenInCPU()
{
    if(ProProcessInfo[0].bInCpu)
    {
        gUi->Process1->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProProcessInfo[1].bInCpu)
    {
        gUi->Process1_2->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProProcessInfo[2].bInCpu)
    {
        gUi->Process1_3->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProProcessInfo[3].bInCpu)
    {
        gUi->Process1_4->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProProcessInfo[4].bInCpu)
    {
        gUi->Process1_5->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProProcessInfo[5].bInCpu)
    {
        gUi->Process1_6->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProProcessInfo[6].bInCpu)
    {
        gUi->Process1_7->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProProcessInfo[7].bInCpu)
    {
        gUi->Process1_8->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProProcessInfo[8].bInCpu)
    {
        gUi->Process1_9->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProProcessInfo[9].bInCpu)
    {
        gUi->Process1_10->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProProcessInfo[10].bInCpu)
    {
        gUi->Process1_11->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProProcessInfo[11].bInCpu)
    {
        gUi->Process1_12->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProProcessInfo[12].bInCpu)
    {
        gUi->Process1_13->setStyleSheet("background-color: rgb(255, 255, 127);");
    }
    if(ProProcessInfo[13].bInCpu)
    {
        gUi->Process1_14->setStyleSheet("background-color: rgb(255, 255, 127);");
    }

}
void prority::UpdateColourWhenInNormal()
{
    if(ProProcessInfo[0].bInJobQ)
    {
        gUi->Process1->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
        //gUi->Process1->setStyleSheet("color: rgb(255, 255, 255);");
    }
    if(ProProcessInfo[1].bInJobQ)
    {
        gUi->Process1_2->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");

    }
    if(ProProcessInfo[2].bInJobQ)
    {
        gUi->Process1_3->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProProcessInfo[3].bInJobQ)
    {
        gUi->Process1_4->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProProcessInfo[4].bInJobQ)
    {
        gUi->Process1_5->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProProcessInfo[5].bInJobQ)
    {
        gUi->Process1_6->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProProcessInfo[6].bInJobQ)
    {
        gUi->Process1_7->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProProcessInfo[7].bInJobQ)
    {
        gUi->Process1_8->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProProcessInfo[8].bInJobQ)
    {
        gUi->Process1_9->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProProcessInfo[9].bInJobQ)
    {
        gUi->Process1_10->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProProcessInfo[10].bInJobQ)
    {
        gUi->Process1_11->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProProcessInfo[11].bInJobQ)
    {
        gUi->Process1_12->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProProcessInfo[12].bInJobQ)
    {
        gUi->Process1_13->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }
    if(ProProcessInfo[13].bInJobQ)
    {
        gUi->Process1_14->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 255, 255);");
    }

}
void prority::ClearLabelColour()
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

void prority::UpdateDisplayRQ()
{
    int i;
    for(i = 0;i<MAXIMUM_PROC-1;i++)
    {
        strcpy(ProDisplayRQ[i].cProc,ProDisplayRQ[i+1].cProc);
        ProDisplayRQ[i].bFill = ProDisplayRQ[i+1].bFill;
        ProDisplayRQ[i].iProcessID = ProDisplayRQ[i+1].iProcessID;
        ProDisplayRQ[i].iBurst = ProDisplayRQ[i+1].iBurst;
        ProDisplayRQ[i].iPriority = ProDisplayRQ[i+1].iPriority;
    }
}

void prority::UpdateReadyQueue()
{
    gUi->ReadyQ1->setText(ProDisplayRQ[0].cProc);
    gUi->ReadyQ1_2->setText(ProDisplayRQ[1].cProc);
    gUi->ReadyQ1_3->setText(ProDisplayRQ[2].cProc);
    gUi->ReadyQ1_4->setText(ProDisplayRQ[3].cProc);
    gUi->ReadyQ1_5->setText(ProDisplayRQ[4].cProc);
    gUi->ReadyQ1_6->setText(ProDisplayRQ[5].cProc);
    gUi->ReadyQ1_7->setText(ProDisplayRQ[6].cProc);
    gUi->ReadyQ1_8->setText(ProDisplayRQ[7].cProc);
    gUi->ReadyQ1_9->setText(ProDisplayRQ[8].cProc);
    gUi->ReadyQ1_10->setText(ProDisplayRQ[9].cProc);
    gUi->ReadyQ1_11->setText(ProDisplayRQ[10].cProc);
    gUi->ReadyQ1_12->setText(ProDisplayRQ[11].cProc);
    gUi->ReadyQ1_13->setText(ProDisplayRQ[12].cProc);
    gUi->ReadyQ1_14->setText(ProDisplayRQ[13].cProc);
}

void prority::PutIntoTheCPU()
{
    bCPUBusy = true;
    gUi->CPU->setText(ProDisplayRQ[0].cProc);
    gUi->CPU->setStyleSheet("background-color: rgb(255, 0, 0);");
    ProProcessInfo[(ProDisplayRQ[0].iProcessID)-1].bInCpu = true;
    ProProcessInfo[(ProDisplayRQ[0].iProcessID)-1].bInReadyQ = false;
    memcpy(&ProCpu,&ProDisplayRQ[0],sizeof(DispProc));
    iProcInRQ--;
    printf("Process in ready q 222222 is %d\n",iProcInRQ);
    UpdateColourWhenInCPU();
    UpdateDisplayRQ();
    UpdateReadyQueue();
}

void prority::FillDisplayRQ(ProcessDef proc)
{
    int i;
    for(i = 0;i<MAXIMUM_PROC;i++)
    {
        if(false == ProDisplayRQ[i].bFill)
        {
            ProDisplayRQ[i].bFill = true;
            strcpy(ProDisplayRQ[i].cProc,proc.cProcessName);
            ProDisplayRQ[i].iProcessID = proc.iProcessID;
            ProDisplayRQ[i].iBurst = proc.iBurstTime;
            ProDisplayRQ[i].iPriority = proc.iPriority;
            iProcInRQ++;
            printf("Process ID in Fill Display is %d\n",ProDisplayRQ[i].iProcessID);
            printf("Process in ready q 11111 is %d\n",iProcInRQ);
            break;
        }
        else
        {
            if(ProDisplayRQ[i].iProcessID == proc.iProcessID)
            {
                ProDisplayRQ[i].bFill = true;
                strcpy(ProDisplayRQ[i].cProc,proc.cProcessName);
                ProDisplayRQ[i].iProcessID = proc.iProcessID;
                ProDisplayRQ[i].iBurst = proc.iBurstTime;
                ProDisplayRQ[i].iPriority = proc.iPriority;
                iProcInRQ++;
                break;
            }
            printf("Process ID in Fill Display is %d\n",ProDisplayRQ[i].iProcessID);
        }
    }
    Sorting(ProDisplayRQ,iProcInRQ,PRIORITY);
 }


void prority::Dispatcher()
{
    int i;
    for(i = 0;i<iProcessInSystem;i++)
    {
        ProProcessInfo[i].iArrivalTime--;
        if((0 == ProProcessInfo[i].iArrivalTime)/* && (false == ProProcessInfo[i].bInReadyQ)*/)
        {
            ProProcessInfo[i].bInReadyQ = true;
            ProProcessInfo[i].bInJobQ = false;
            FillDisplayRQ(ProProcessInfo[i]);
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
            if(ProProcessInfo[i].bInCpu)
            {
                char cBurst[4];
                sprintf(cBurst,"%d",ProProcessInfo[i].iBurstTime);
                gUi->BTTimerLeft->setText(cBurst);

                if(iPreemptiveness == PREEMPTIVE)
                {
                    if(0 == ProProcessInfo[i].iBurstTime)
                    {
                        bCPUBusy = false;
                        ProProcessInfo[i].bInCpu = false;
                        ProProcessInfo[i].bInJobQ = true;
                        ProProcessInfo[i].bInReadyQ = false;
                        UpdateColourWhenInNormal();
                    }
                    else if(ProProcessInfo[i].iPriority > ProDisplayRQ[0].iPriority)
                    {
                        ProProcessInfo[i].bInCpu = false;
                        ProProcessInfo[i].bInReadyQ = true;
                        FillDisplayRQ(ProProcessInfo[i]);
                        Sorting(ProDisplayRQ,iProcInRQ,PRIORITY);
                        UpdateColourWhenInRQ();
                        printf("Process in ready q 33333 is %d\n",iProcInRQ);
                        UpdateDisplayRQAtEveryBeat(ProDisplayRQ,ProProcessInfo[i]);
                        PutIntoTheCPU();
                    }
                    else
                    {
                        UpdateDisplayRQAtEveryBeat(ProDisplayRQ,ProProcessInfo[i]);
                    }
                }
                else if(0 == (ProProcessInfo[i].iBurstTime))
                {
                    bCPUBusy = false;
                    ProProcessInfo[i].bInCpu = false;
                    ProProcessInfo[i].bInJobQ = true;
                    UpdateColourWhenInNormal();
                }
                ProProcessInfo[i].iBurstTime--;
            }
        }
    }
}

void prority::CallDispatcher()
{
    Dispatcher();
}
void prority::DisplayMessage(int iMessageType)
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
         case PRORITY_MSG:
         msgBox.setText("Priority time less then One is not allowed");
         msgBox.exec();
         break;
    }
}

void prority::on_CreateProcess_clicked()
{
    EnableWidgetAfterProcessCreation();
    FillProcessInfo(ProProcStructInUse);
    gUi->ChangeBT->addItem(ProProcStructInUse[iProcessInSystem].cProcessName);
    gUi->ChangeAT->addItem(ProProcStructInUse[iProcessInSystem].cProcessName);
    gUi->ChangePro->addItem(ProProcStructInUse[iProcessInSystem].cProcessName);
    memcpy(&ProProcessInfo[iProcessInSystem],&ProProcStructInUse[iProcessInSystem],sizeof(ProcessDef));
    iProcessInSystem++;
    ProcessDisplayOnHMi(ProProcStructInUse);
}

void prority::on_BurstTime_textChanged(QString )
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
        ProProcessInfo[iBurstIndex-1].iBurstTime = atoi(cBurst);
        ProProcStructInUse[iBurstIndex-1].iBurstTime = atoi(cBurst);
        ProcessDisplayOnHMi(ProProcessInfo);
    }
}

void prority::on_ChangeBT_highlighted(int index)
{
    iBurstIndex = index;
}

void prority::on_ChangeAT_highlighted(int index)
{
    iArrivalIndex = index;
}

void prority::on_ArrivalTime_textChanged(QString )
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
        ProProcessInfo[iArrivalIndex-1].iArrivalTime = atoi(cArrival);
        ProProcStructInUse[iArrivalIndex-1].iArrivalTime = atoi(cArrival);
        ProcessDisplayOnHMi(ProProcessInfo);
    }
}

void prority::CreateTimer()
{
    Timer = new QTimer(pro);
    connect(Timer,SIGNAL(timeout()),pro,SLOT(CallDispatcher()));
    Timer->start(1000);
}
void prority::DisableWidgetsWhenStart()
{
    gUi->CreateProcess->setEnabled(false);
    gUi->ChangeBT->setEnabled(false);
    gUi->BurstTime->setEnabled(false);
    gUi->ChangeAT->setEnabled(false);
    gUi->ArrivalTime->setEnabled(false);
    gUi->Prority->setEnabled(false);
    gUi->Preemptiveness->setEnabled(false);
}
void prority::EnableWidgetWhenStop()
{
    gUi->CreateProcess->setEnabled(true);
    gUi->ChangeBT->setEnabled(true);
    gUi->BurstTime->setEnabled(true);
    gUi->ChangeAT->setEnabled(true);
    gUi->ArrivalTime->setEnabled(true);
    gUi->Prority->setEnabled(true);
    gUi->Preemptiveness->setEnabled(true);
}
void prority::on_StartSim_clicked()
{
    if(!bProStart)
    {
        DisableWidgetsWhenStart();
        int i = 0;
        for(i=0;i<MAXIMUM_PROC;i++)
        {
            memcpy(&ProProcessInfo[i],&ProProcStructInUse[i],sizeof(ProcessDef));
        }
        CreateTimer();
        gUi->StartSim->setText("Stop");
        bProStart= true;
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
        bProStart = false;
        gUi->StartSim->setText("Start");
        ProcessDisplayOnHMi(ProProcStructInUse);
        gUi->PauseSim->setEnabled(false);

    }
}
void prority::ResetSimulator()
{
    ClearLabelColour();
    ClearProcessLabels();
    ClearBTLabels();
    ClearATLabels();
    ClearProLabels();
    ClearRQLabels();
    ClearCPULabels();
    ClearBTTimeLabel();
    ClearAllStructure();
    gUi->StartSim->setText("Start");
    gUi->PauseSim->setText("Pause");
}

void prority::on_StepSim_clicked()
{
    Dispatcher();

}

void prority::on_ResetSim_clicked()
{
    DisableWidgetsBeforeProcessCreation();
    EnableWidgetWhenStop();
    DeleteTimer();
    ResetSimulator();
}

void prority::on_PauseSim_clicked()
{
    if(!bProPause)
    {
        Timer->stop();
        gUi->PauseSim->setText("Resume");
        gUi->StepSim->setEnabled(true);
        gUi->StartSim->setEnabled(false);
        bProPause = true;
    }
    else
    {
        gUi->PauseSim->setText("Pause");
        Timer->start(1000);
        bProPause = false;
        gUi->StepSim->setEnabled(false);
        gUi->StartSim->setEnabled(true);
    }
}

void prority::on_Preemptiveness_highlighted(int index)
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

void prority::on_ChangePro_highlighted(int index)
{
    iPriority = index;
}

void prority::on_Prority_textChanged(QString )
{
    if(iPriority != 0)
    {
        char cPriority[4];
        strcpy(cPriority,ui->Prority->text().toStdString().c_str());
        if(atoi(cPriority) <= 0)
        {
            DisplayMessage(PRORITY_MSG);
            return;
        }
        ProProcessInfo[iPriority-1].iPriority = atoi(cPriority);
        ProcessDisplayOnHMi(ProProcessInfo);
    }
}
