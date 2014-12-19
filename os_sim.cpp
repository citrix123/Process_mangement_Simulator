#include "os_sim.h"
#include "ui_os_sim.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <fcfs.h>
#include <sjf.h>
#include <prority.h>
#include <rr.h>
OS_Sim::OS_Sim(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OS_Sim)
{
    ui->setupUi(this);
}

OS_Sim::~OS_Sim()
{
    delete ui;
}


void OS_Sim::on_SchedulingType_highlighted(int index)
{
    iTypeOfScheduling = index;
}

void OS_Sim::on_SelectScheduling_clicked()
{
    switch(iTypeOfScheduling)
    {
    case FCFSS:
        OpenFCFSWindow();
        break;
    case SJF:
        OpenSjfWindow();
        break;
    case PRIORITY:
        OpenPriorityWIndow();
        break;
    case RR:
        OpenRRWindow();
        break;
    default:
        break;
    }


}
