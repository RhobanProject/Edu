#ifndef CONTROLER_H
#define CONTROLER_H

#include <QWidget>
#include "Camera.h"

#include "SimulationViewer.h"

namespace Ui {
    class Controler;
}

class Controler : public QWidget {
    Q_OBJECT
public:
    Controler(QWidget *parent = 0);
    ~Controler();
    void setSimulationViewer( SimulationViewer* simulation_viewer);

public slots:
    void refreshComponents(SimulationViewer* simuViewer);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Controler *ui;
    SimulationViewer* _simulationViewer;

private slots:
    void on_boxTrack_clicked(bool checked);
    void on_boxGrass_clicked(bool checked);
    void on_boxMap_clicked(bool checked);
    void on_boxStakes_clicked(bool checked);
    void on_boxVines_clicked(bool checked);
    void on_spinBox_valueChanged(int );
    void on_horizontalSlider_valueChanged(int value);
    void on_controlZ_valueChanged(int value);
    void on_controlY_valueChanged(int value);
    void on_controlX_valueChanged(int value);
    void on_controlZoom_valueChanged(int value);
};

#endif // CONTROLER_H
