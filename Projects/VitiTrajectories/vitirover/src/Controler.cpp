#include "Controler.h"
#include "../bin/ui_Controler.h"
#include <iostream>

Controler::Controler(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Controler)
{
    std::cout<<"Initialisation du widget de controle"<<std::endl;
    ui->setupUi(this);
}

Controler::~Controler()
{
    delete ui;
}

void Controler::changeEvent(QEvent *e)
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

void Controler::refreshComponents(SimulationViewer *simuViewer)
{

}
void Controler::setSimulationViewer(SimulationViewer *simulation_viewer)
{
    _simulationViewer = simulation_viewer;
}

void Controler::on_controlZoom_valueChanged(int value)
{
    if ( _simulationViewer )
    {
        Camera* c = _simulationViewer->getCamera();
        c->_delta_to.setX( value );
        c->_delta_to.setY( value );
        c->_delta_to.setZ( value );
        _simulationViewer->setCamera( c );
    }
}

void Controler::on_controlX_valueChanged(int value)
{
    if ( _simulationViewer )
    {
        Camera* c = _simulationViewer->getCamera();
        c->_x_angle =  value ;
        _simulationViewer->setCamera( c );
    }
}

void Controler::on_controlY_valueChanged(int value)
{
    if ( _simulationViewer )
    {
        Camera* c = _simulationViewer->getCamera();
        c->_y_angle = value ;
        _simulationViewer->setCamera( c );
    }
}

void Controler::on_controlZ_valueChanged(int value)
{
    if ( _simulationViewer )
    {
        Camera* c = _simulationViewer->getCamera();
        c->_z_angle = value ;
        _simulationViewer->setCamera( c );
    }

}

void Controler::on_horizontalSlider_valueChanged(int value)
{
    SimulationViewer::speedSimulation = value;
    _simulationViewer->_timerRepaint->start(Simulation::timeSlice*1000/ SimulationViewer::speedSimulation);
}

void Controler::on_spinBox_valueChanged(int value )
{
    SimulationViewer::precision = value;
}

void Controler::on_boxVines_clicked(bool checked)
{
    _simulationViewer->setDrawVines( checked );
}

void Controler::on_boxStakes_clicked(bool checked)
{
    _simulationViewer->setDrawStakes( checked );
}

void Controler::on_boxMap_clicked(bool checked)
{
    _simulationViewer->setDrawMap( checked );
}

void Controler::on_boxGrass_clicked(bool checked)
{
    _simulationViewer->setDrawGrass( checked );
}

void Controler::on_boxTrack_clicked(bool checked)
{
    _simulationViewer->setTrackRobot( checked );
}
