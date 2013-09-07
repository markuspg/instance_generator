enum DISTRIBUTION {DISJOINT, GAMMA, NORMAL5, NORMAL9, UNIFORM};

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "histogram.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_PBCreateProblemInstance_clicked();

    void on_RBDisjointUniformDistribution_toggled(bool checked);

    void on_RBGammaDistribution_toggled(bool checked);

    void on_RBNormal5Distribution_toggled(bool checked);

    void on_RBNormal9Distribution_toggled(bool checked);

    void on_RBUniformDistribution_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    void generate_problem ();
    unsigned int choice;
};

#endif // MAINWINDOW_H
