#include "mainwindow.h"
#include "histogram.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    choice(0)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generate_problem() {
    for (unsigned int k = 1; k <= ui->LEInstances->text().toUInt(); k++) {
        // Create the filename and open the file
        QString filename(ui->LEFilename->text());
        filename.append(QString("%1").arg(k));
        filename.append(".pbl");
        // Open the file to write to
        QFile output_file(filename);
        output_file.open(QIODevice::WriteOnly | QIODevice::Text);

        // Create the vector storing the processes' durations
        std::vector<unsigned int> process_durations;
        unsigned short int processes_quantity = ui->LEProcessQuantity->text().toUShort();
        process_durations.reserve(processes_quantity);

        // Create the process durations and store them sorted in the vector
        unsigned int seed = QDateTime::currentMSecsSinceEpoch();
        std::default_random_engine engine(seed);
        switch(choice) {
        case NORMAL20: {
            std::normal_distribution<double> generator(100, 20);
            for (unsigned short int j = 0; j < processes_quantity; j++) {
                process_durations.push_back(static_cast<unsigned int>(generator(engine) + 0.5));
            }
            break;
        }
        case NORMAL50: {
            std::normal_distribution<double> generator(100, 50);
            for (unsigned short int j = 0; j < processes_quantity; j++) {
                process_durations.push_back(static_cast<unsigned int>(generator(engine) + 0.5));
            }
            break;
        }
        case UNIFORM1: {
            std::uniform_int_distribution<unsigned int> generator(1, 100);
            for (unsigned short int j = 0; j < processes_quantity; j++) {
                process_durations.push_back(generator(engine));
            }
            break;
        }
        case UNIFORM20: {
            std::uniform_int_distribution<unsigned int> generator(20, 100);
            for (unsigned short int j = 0; j < processes_quantity; j++) {
                process_durations.push_back(generator(engine));
            }
            break;
        }
        case UNIFORM50: {
            std::uniform_int_distribution<unsigned int> generator(50, 100);
            for (unsigned short int j = 0; j < processes_quantity; j++) {
                process_durations.push_back(generator(engine));
            }
        }
        }
        std::sort (process_durations.begin(), process_durations.end());

        // Storing the problem's general settings
        QTextStream out(&output_file);
        out << "# Machines\n" << ui->LEMachineQuantity->text() << "\n# Processes\n" << ui->LEProcessQuantity->text() << "\n# Process durations\n";

        // Store the problem's process durations. The vector is iterated reversely because of a wrong order of the sort function.
        for (std::vector<unsigned int>::reverse_iterator rit = process_durations.rbegin(); rit != process_durations.rend(); ++rit) {
            out << *rit;
            if (rit != (process_durations.rend() - 1))
                out << ";";
        }
        out << "\n";
        output_file.close();
        ui->statusBar->showMessage("The problem instances have been created");

        Histogram *histogram;
        histogram = new Histogram(process_durations, filename);
        histogram->show();
    }
}

void MainWindow::on_PBCreateProblemInstance_clicked() {
    generate_problem();
}

void MainWindow::on_RBNormal50Distribution_toggled(bool checked)
{
    if (checked)
        choice = NORMAL50;
}

void MainWindow::on_RBNormal20Distribution_toggled(bool checked)
{
    if (checked)
        choice = NORMAL20;
}

void MainWindow::on_RBUniform1Distribution_toggled(bool checked)
{
    if (checked)
        choice = UNIFORM1;
}

void MainWindow::on_RBUniform20Distribution_toggled(bool checked)
{
    if (checked)
        choice = UNIFORM20;
}

void MainWindow::on_RBUniform50Distribution_toggled(bool checked)
{
    if (checked)
        choice = UNIFORM50;
}
