#include "FieldInformationWidget.h"
#include "../bin/ui_FieldInformationWidget.h"

FieldInformationWidget::FieldInformationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FieldInformationWidget)
{
    ui->setupUi(this);

}

FieldInformationWidget::~FieldInformationWidget()
{
    delete ui;
}

void FieldInformationWidget::changeEvent(QEvent *e)
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
void FieldInformationWidget::initialize(Simulation *simulation)
{
    ui->nbr_vine->setText( QString::number( simulation->getMap()->getVines()->size() ) );
}

void FieldInformationWidget::refreshComponents(Simulation *simulation)
{    
    ui->width->setText( QString::number(simulation->getMap()->getSize()) );
    ui->length->setText( QString::number(simulation->getMap()->getSize()) );
    ui->grass_high->setText( QString::number(simulation->getMap()->getAverageGrassHigh()) );
}
