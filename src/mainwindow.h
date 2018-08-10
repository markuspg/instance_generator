#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "histogram.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
} // namespace Ui

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *const argParent = nullptr);
    ~MainWindow() override;
    
private slots:
    void on_PBCreateProblemInstance_clicked();
    void on_RBNormal50Distribution_toggled(const bool argChecked);
    void on_RBNormal20Distribution_toggled(const bool argChecked);
    void on_RBUniform1Distribution_toggled(const bool argChecked);
    void on_RBUniform20Distribution_toggled(const bool argChecked);
    void on_RBUniform50Distribution_toggled(const bool argChecked);
    void on_PBCreateDefaultInstances_clicked();

private:
    enum class EDistribution {
        NORMAL20,
        NORMAL50,
        UNIFORM1,
        UNIFORM20,
        UNIFORM50
    };

    void GenerateDefaultInstances();
    void GenerateProblem ();

    EDistribution choice = EDistribution::NORMAL20;
    const std::unique_ptr<Ui::MainWindow> ui;
};

#endif // MAINWINDOW_H
