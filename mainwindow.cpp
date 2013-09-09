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
        unsigned int interval_upper_bound = ui->LEInterval->text().toUInt();
        process_durations.reserve(processes_quantity);

        // Create the process durations and store them sorted in the vector
        unsigned int seed = QDateTime::currentMSecsSinceEpoch();
        std::default_random_engine engine(seed);
        switch(choice) {
        case DISJOINT: {
            std::uniform_int_distribution<unsigned int> generator_lower_interval(1, static_cast<int>(0.2 * (interval_upper_bound - 1)));
            for (unsigned short int j = 0; j < floor(processes_quantity * 0.02); j++) {
                process_durations.push_back(generator_lower_interval(engine));
            }
            std::uniform_int_distribution<unsigned int> generator_upper_interval(floor(0.9 * (interval_upper_bound - 1)), interval_upper_bound);
            for (unsigned short int j = (processes_quantity * 0.02); j < processes_quantity; j++) {
                process_durations.push_back(generator_upper_interval(engine));
            }
            break;
        }
        case GAMMA: {
            std::gamma_distribution<double> generator(2,10);
            for (unsigned short int j = 0; j < processes_quantity; j++) {
                process_durations.push_back(generator(engine));
            }
            break;
        }
        case NORMAL5: {
            std::normal_distribution<double> generator((interval_upper_bound / 2), 5.0);
            for (unsigned short int j = 0; j < processes_quantity; j++) {
                process_durations.push_back(generator(engine));
            }
            break;
        }
        case NORMAL9: {
            std::normal_distribution<double> generator((interval_upper_bound / 2), 9.0);
            for (unsigned short int j = 0; j < processes_quantity; j++) {
                process_durations.push_back(generator(engine));
            }
            break;
        }
        case UNIFORM: {
            std::uniform_int_distribution<unsigned int> generator(1, interval_upper_bound);
            for (unsigned short int j = 0; j < processes_quantity; j++) {
                process_durations.push_back(generator(engine));
            }
        }
        }
        std::sort (process_durations.begin(), process_durations.end());

        // Storing the problem's general settings
        QTextStream out(&output_file);
        out << "# Machines\n" << ui->LEMachineQuantity->text() << "\n# Processes\n" << ui->LEProcessQuantity->text() << "\n# Processing times upper bound\n" << ui->LEInterval->text() << "\n# Process durations\n";

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
        histogram = new Histogram(process_durations, ui->LEInterval->text().toUInt(), filename);
        histogram->show();
    }
}

void MainWindow::on_PBCreateProblemInstance_clicked() {
    generate_problem();
}

void MainWindow::on_RBDisjointUniformDistribution_toggled(bool checked)
{
    if (checked)
        choice = DISJOINT;
}

void MainWindow::on_RBGammaDistribution_toggled(bool checked)
{
    if (checked)
        choice = GAMMA;
}

void MainWindow::on_RBNormal5Distribution_toggled(bool checked)
{
    if (checked)
        choice = NORMAL5;
}

void MainWindow::on_RBNormal9Distribution_toggled(bool checked)
{
    if (checked)
        choice = NORMAL9;
}

void MainWindow::on_RBUniformDistribution_toggled(bool checked)
{
    if (checked)
        choice = UNIFORM;
}
