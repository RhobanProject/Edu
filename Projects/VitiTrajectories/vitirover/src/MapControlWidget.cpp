#include "MapControlWidget.h"
#include "../bin/ui_MapControlWidget.h"

MapControlWidget::MapControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapControlWidget)
{
    ui->setupUi(this);
}

MapControlWidget::~MapControlWidget()
{
    delete ui;
}

void MapControlWidget::changeEvent(QEvent *e)
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


void MapControlWidget::setSimulation(Simulation *simulation)
{
    _simulation = simulation;
    ui->size->setText( QString::number(simulation->getMap()->getSize()*SimulationViewer::cote/100) );
    ui->freq_layer->setText( "5" );
    ui->octave->setText(  QString::number(_simulation->getMap()->getOctave()) );
}

void MapControlWidget::on_pushButton_clicked()
{
    //QFrame* loading_sreen = new QFrame(this, Qt::Dialog  );
    QDialog* loading_screen = new QDialog( this->parentWidget()->parentWidget(), Qt::Popup );
    loading_screen->setWindowTitle("Generation du terrain...");
    QLabel*information = new QLabel("Generation du terrain...", loading_screen );
    information->setGeometry(10,10, 160, 40);
    information->setFocus();
    loading_screen->show();
    parentWidget()->parentWidget()->setEnabled(false);

    qApp->processEvents();

    if ( _simulation != NULL )
    {
        float min = _simulation->getMap()->getMinWeight();
        float max = _simulation->getMap()->getMaxWeight();

        _simulation->initializeMap( ui->size->text().toInt()/SimulationViewer::cote*100 ,
                                    ui->octave->text().toInt(),
                                    ui->weight->value(),
                                    min,
                                    max,
                                    5, ui->freq_layer->text().toInt() );
        _simulation->getMap()->processToFieldMap();
        _simulation->getMap()->generateLane(150/SimulationViewer::cote, 600/SimulationViewer::cote, 100/SimulationViewer::cote);
    }

    parentWidget()->parentWidget()->setEnabled(true);
    loading_screen->deleteLater();
}
