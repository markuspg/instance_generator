#include "mainwindow.h"
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
    // Open the file to write to
    QFile output_file(ui->LEFilename->text());
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
    /* case NORMAL: {
        std::normal_distribution<unsigned short int> generator();
        for (unsigned short int j = 0; j < processes_quantity; j++) {
            process_durations.push_back(generator(engine));
        }
    } */
    case UNIFORM: {
        std::uniform_int_distribution<unsigned short int> generator (1, interval_upper_bound);
        for (unsigned short int j = 0; j < processes_quantity; j++) {
            process_durations.push_back(generator(engine));
        }
    }
    }
    std::sort (process_durations.begin(), process_durations.end());

    // Storing the problem's general settings
    QTextStream out(&output_file);
    out << "# Machines\n" << ui->LEMachineQuantity->text() << "\n# Processes\n" << ui->LEProcessQuantity->text() << "\n# Processing times upper bound\n" << ui->LEInterval->text() << "\n# Process durations\n";
    for (std::vector<unsigned int>::reverse_iterator rit = process_durations.rbegin(); rit != process_durations.rend(); ++rit) {
        out << *rit;
        if (rit != (process_durations.rend() - 1))
            out << ";";
    }
    out << "\n";
    output_file.close();
}

void MainWindow::on_PBCreateProblemInstance_clicked() {
    generate_problem();
}

void MainWindow::on_RBNormalDistribution_toggled(bool checked)
{
    if (checked)
        choice = NORMAL;
}

void MainWindow::on_RBUniformDistribution_toggled(bool checked)
{
    if (checked)
        choice = UNIFORM;
}
