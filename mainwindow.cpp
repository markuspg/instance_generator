#include "mainwindow.h"
#include "histogram.h"
#include "ui_mainwindow.h"

#include <fstream>
#include <string>
#include <QFile>
#include <QTextStream>
#include <QThread>
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
        // Create the filenames and open the files
        QString filename(ui->LEFilename->text());
        QString xpressdatafilename(ui->LEFilename->text());
        filename.append(QString("%1").arg(k));
        xpressdatafilename.append(QString("%1").arg(k));
        filename.append(".pbl");
        xpressdatafilename.append(".txt");
        // Open the files to write to
        QFile output_file(filename);
        QFile xpress_output_file(xpressdatafilename);
        output_file.open(QIODevice::WriteOnly | QIODevice::Text);
        xpress_output_file.open(QIODevice::WriteOnly | QIODevice::Text);

        // Create the vector storing the processes' durations
        std::vector<unsigned int> process_durations;
        unsigned short int processes_quantity = ui->LEProcessQuantity->text().toUShort();
        process_durations.reserve(processes_quantity);

        // Create the process durations and store them sorted in the vector
        unsigned int seed = QDateTime::currentMSecsSinceEpoch();
        std::default_random_engine engine(seed);
        switch(choice) {
        case NORMAL20: {
            std::normal_distribution<double> generator(100.0, 20.0);
            for (unsigned short int j = 0; j < processes_quantity; j++) {
                double temp = 1;
                do {
                    temp = generator(engine) + 0.5;
                } while (temp < 1);
                process_durations.push_back(static_cast<unsigned int>(temp));
            }
            break;
        }
        case NORMAL50: {
            std::normal_distribution<double> generator(100.0, 50.0);
            for (unsigned short int j = 0; j < processes_quantity; j++) {
                double temp = 1;
                do {
                    temp = generator(engine) + 0.5;
                } while (temp < 1);
                process_durations.push_back(static_cast<unsigned int>(temp));
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
        QTextStream xpress_out(&xpress_output_file);
        out << "# Machines\n" << ui->LEMachineQuantity->text() << "\n# Processes\n" << ui->LEProcessQuantity->text() << "\n# Process durations\n";
        xpress_out << "Machines: " << ui->LEMachineQuantity->text() << "\nProcesses: " << ui->LEProcessQuantity->text() << "\nDurations: [";

        // Store the problem's process durations. The vector is iterated reversely because of a wrong order of the sort function.
        for (std::vector<unsigned int>::reverse_iterator rit = process_durations.rbegin(); rit != process_durations.rend(); ++rit) {
            out << *rit;
            xpress_out << *rit;
            if (rit != (process_durations.rend() - 1)) {
                out << ";";
                xpress_out << ",";
            }
        }
        out << "\n";
        xpress_out << "]\n";
        output_file.close();
        xpress_output_file.close();
        ui->statusBar->showMessage("The problem instances have been created");

        Histogram *histogram;
        histogram = new Histogram(process_durations, filename);
        histogram->show();
        histogram->setAttribute(Qt::WA_DeleteOnClose);
    }
}

void MainWindow::generate_default_instances() {
    // The sizes
    unsigned short int sizes[17][2] = {{10,11},{100,110},{1000,1100},{6,9},{60,90},{600,900},{4,10},{40,100},{400,1000},{10,50},{100,500},{1000,5000},{10,100},{100,1000},{1000,10000},{10,1000},{100,10000}};

    // Iterate over the different distributions
    for (unsigned int t = 0; t < 5; t++) {
        std::string distribution;
        switch(t) {
        case 0:
            distribution = "normal20";
            break;
        case 1:
            distribution = "normal50";
            break;
        case 2:
            distribution = "uniform1";
            break;
        case 3:
            distribution = "uniform20";
            break;
        case 4:
            distribution = "uniform50";
            break;
        }

        // Iterate over the different sizes
        for (unsigned short int u = 0; u < 17; u++) {
            // Iterate over the ten instances
            for (unsigned short int v = 1; v < 11; v++) {
                std::string filename (distribution);
                filename.append("-");
                filename.append(std::to_string(sizes[u][0]));
                filename.append("-");
                filename.append(std::to_string(sizes[u][1]));
                filename.append(".");

                filename.append(std::to_string(v));
                std::string xpress_filename (filename);
                filename.append(".pbl");
                xpress_filename.append(".txt");

                // Create Processes
                std::vector<unsigned int> process_durations;
                process_durations.reserve(sizes[u][1]);

                unsigned int seed = QDateTime::currentMSecsSinceEpoch();
                std::default_random_engine engine(seed);
                switch(t) {
                case 0: {
                    std::normal_distribution<double> generator(100.0, 20.0);
                    for (unsigned short int w = 0; w < sizes[u][1]; w++) {
                        double temp = 1;
                        do {
                            temp = generator(engine) + 0.5;
                        } while (temp < 1);
                        process_durations.push_back(static_cast<unsigned int>(temp));
                    }
                    break;
                }
                case 1: {
                    std::normal_distribution<double> generator(100.0, 50.0);
                    for (unsigned short int w = 0; w < sizes[u][1]; w++) {
                        double temp = 1;
                        do {
                            temp = generator(engine) + 0.5;
                        } while (temp < 1);
                        process_durations.push_back(static_cast<unsigned int>(temp));
                    }
                    break;
                }
                case 2: {
                    std::uniform_int_distribution<unsigned int> generator(1, 100);
                    for (unsigned short int w = 0; w < sizes[u][1]; w++) {
                        process_durations.push_back(generator(engine));
                    }
                    break;
                }
                case 3: {
                    std::uniform_int_distribution<unsigned int> generator(20, 100);
                    for (unsigned short int w = 0; w < sizes[u][1]; w++) {
                        process_durations.push_back(generator(engine));
                    }
                    break;
                }
                case 4: {
                    std::uniform_int_distribution<unsigned int> generator(50, 100);
                    for (unsigned short int w = 0; w < sizes[u][1]; w++) {
                        process_durations.push_back(generator(engine));
                    }
                }
                }
                std::sort (process_durations.begin(), process_durations.end());

                std::ofstream output_file_stream, xpress_output_file_stream;
                output_file_stream.open (filename, std::ofstream::out | std::ofstream::trunc);
                xpress_output_file_stream.open (xpress_filename, std::ofstream::out | std::ofstream::trunc);

                // Storing the problem's general settings
                output_file_stream << "# Machines\n" << sizes[u][0] << "\n# Processes\n" << sizes[u][1] << "\n# Process durations\n";
                xpress_output_file_stream << "Machines: " << sizes[u][0] << "\nProcesses: " << sizes[u][1] << "\nDurations: [";

                // Store the problem's process durations. The vector is iterated reversely because of a wrong order of the sort function.
                for (std::vector<unsigned int>::reverse_iterator rit = process_durations.rbegin(); rit != process_durations.rend(); ++rit) {
                    output_file_stream << *rit;
                    xpress_output_file_stream << *rit;
                    if (rit != (process_durations.rend() - 1)) {
                        output_file_stream << ";";
                        xpress_output_file_stream << ",";
                    }
                }
                output_file_stream << "\n";
                xpress_output_file_stream << "]\n";

                output_file_stream.close ();
                xpress_output_file_stream.close ();
                ui->statusBar->showMessage("The problem instances have been created");

                Histogram *histogram;
                QString tempfilename = QString::fromUtf8(filename.c_str());
                histogram = new Histogram(process_durations, tempfilename);
                histogram->show();
                histogram->setAttribute(Qt::WA_DeleteOnClose);

                QThread::msleep(500);

                histogram->hide();
                delete histogram;
            }
        }
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

void MainWindow::on_PBCreateDefaultInstances_clicked()
{
    generate_default_instances();
}
