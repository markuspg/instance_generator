enum DISTRIBUTION {NORMAL20, NORMAL50, UNIFORM1, UNIFORM20, UNIFORM50};

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

    void on_RBNormal50Distribution_toggled(bool checked);

    void on_RBNormal20Distribution_toggled(bool checked);

    void on_RBUniform1Distribution_toggled(bool checked);

    void on_RBUniform20Distribution_toggled(bool checked);

    void on_RBUniform50Distribution_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    void generate_problem ();
    unsigned int choice;
};

#endif // MAINWINDOW_H
