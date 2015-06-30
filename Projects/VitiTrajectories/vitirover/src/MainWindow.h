#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "SimulationViewer.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void refreshComponents();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QTimer _timer_refresh;

private slots:
};

#endif // MAINWINDOW_H
