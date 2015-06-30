#include "RobotInformationWidget.h"
#include "../bin/ui_RobotInformationWidget.h"

RobotInformationWidget::RobotInformationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RobotInformationWidget)
{
    ui->setupUi(this);
}

RobotInformationWidget::~RobotInformationWidget()
{
    delete ui;
}

void RobotInformationWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void RobotInformationWidget::refreshComponents(Robot *robot)
{
    ui->posX->setText( QString::number(robot->getCenter().getX()) );
    ui->posY->setText( QString::number(robot->getCenter().getY()) );
    ui->posZ->setText( QString::number(robot->getCenter().getZ()) );

}
