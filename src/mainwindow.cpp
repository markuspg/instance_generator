/*
 * Copyright 2013, 2018 Markus Prasser
 *
 * This file is part of instance_generator.
 *
 * instance_generator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * instance_generator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with instance_generator.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "exceptions.h"
#include "histogram.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QException>
#include <QFile>
#include <QTextStream>
#include <QThread>

#include <fstream>
#include <random>

MainWindow::MainWindow(QWidget *const argParent) :
    QMainWindow{argParent},
    ui{std::make_unique<Ui::MainWindow>()}
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
}

void MainWindow::GenerateProblem() {
    const auto instanceQuantity = ui->SBInstances->value();
    for (int k = 0; k < instanceQuantity; ++k) {
        // Create the filenames and open the files
        const auto filename{
            QString{"%1%2%3"}.arg(ui->LEFilename->text()).arg(k).arg(".pbl")};

        // Open the files to write to
        QFile outputFile{filename};
        QFile xPressOutputFile{
            QString{"%1%2%3"}.arg(ui->LEFilename->text()).arg(k).arg(".txt")};

        if (outputFile.open(QIODevice::Text | QIODevice::WriteOnly) == false) {
            throw FileOpenFailureException{};
        }
        if (xPressOutputFile.open(QIODevice::Text | QIODevice::WriteOnly) == false) {
            throw FileOpenFailureException{};
        }

        // Create the vector storing the processes' durations
        std::vector<unsigned int> processDurations;
        const auto processesQuantity = ui->SBProcesses->value();
        processDurations.reserve(
                    static_cast<decltype(processDurations)::size_type>(
                        processesQuantity));

        // Create the process durations and store them sorted in the vector
        const auto seed = QDateTime::currentMSecsSinceEpoch();
        std::default_random_engine engine{seed};
        switch(choice) {
        case EDistribution::NORMAL20: {
            std::normal_distribution<double> generator{100.0, 20.0};
            for (unsigned short int j = 0; j < processesQuantity; ++j) {
                double temp = 1;
                do {
                    temp = generator(engine) + 0.5;
                } while (temp < 1);
                processDurations.emplace_back(static_cast<unsigned int>(temp));
            }
            break;
        }
        case EDistribution::NORMAL50: {
            std::normal_distribution<double> generator{100.0, 50.0};
            for (unsigned short int j = 0; j < processesQuantity; ++j) {
                double temp = 1;
                do {
                    temp = generator(engine) + 0.5;
                } while (temp < 1);
                processDurations.emplace_back(static_cast<unsigned int>(temp));
            }
            break;
        }
        case EDistribution::UNIFORM1: {
            std::uniform_int_distribution<unsigned int> generator{1, 100};
            for (unsigned short int j = 0; j < processesQuantity; ++j) {
                processDurations.emplace_back(generator(engine));
            }
            break;
        }
        case EDistribution::UNIFORM20: {
            std::uniform_int_distribution<unsigned int> generator{20, 100};
            for (unsigned short int j = 0; j < processesQuantity; ++j) {
                processDurations.emplace_back(generator(engine));
            }
            break;
        }
        case EDistribution::UNIFORM50: {
            std::uniform_int_distribution<unsigned int> generator{50, 100};
            for (unsigned short int j = 0; j < processesQuantity; ++j) {
                processDurations.emplace_back(generator(engine));
            }
        }
        }
        std::sort(processDurations.begin(), processDurations.end());

        // Storing the problem's general settings
        QTextStream out(&outputFile);
        QTextStream xpress_out(&xPressOutputFile);
        out << "# Machines\n" << ui->SBMachines->value()
            << "\n# Processes\n" << ui->SBProcesses->value()
            << "\n# Process durations\n";
        xpress_out << "Machines: " << ui->SBMachines->value()
                   << "\nProcesses: " << ui->SBProcesses->value()
                   << "\nDurations: [";

        // Store the problem's process durations. The vector is iterated reversely because of a wrong order of the sort function.
        for (std::vector<unsigned int>::reverse_iterator rit = processDurations.rbegin(); rit != processDurations.rend(); ++rit) {
            out << *rit;
            xpress_out << *rit;
            if (rit != (processDurations.rend() - 1)) {
                out << ";";
                xpress_out << ",";
            }
        }
        out << "\n";
        xpress_out << "]\n";
        outputFile.close();
        xPressOutputFile.close();
        ui->statusBar->showMessage("The problem instances have been created");

        Histogram *histogram;
        histogram = new Histogram(std::move(processDurations), filename);
        histogram->show();
        histogram->setAttribute(Qt::WA_DeleteOnClose);
    }
}

void MainWindow::GenerateDefaultInstances() {
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
                histogram = new Histogram(std::move(process_durations), tempfilename);
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
    GenerateProblem();
}

void MainWindow::on_RBNormal50Distribution_toggled(const bool argChecked)
{
    if (argChecked == true)
        choice = EDistribution::NORMAL50;
}

void MainWindow::on_RBNormal20Distribution_toggled(const bool argChecked)
{
    if (argChecked == true)
        choice = EDistribution::NORMAL20;
}

void MainWindow::on_RBUniform1Distribution_toggled(const bool argChecked)
{
    if (argChecked == true)
        choice = EDistribution::UNIFORM1;
}

void MainWindow::on_RBUniform20Distribution_toggled(const bool argChecked)
{
    if (argChecked == true)
        choice = EDistribution::UNIFORM20;
}

void MainWindow::on_RBUniform50Distribution_toggled(const bool argChecked)
{
    if (argChecked == true)
        choice = EDistribution::UNIFORM50;
}

void MainWindow::on_PBCreateDefaultInstances_clicked()
{
    GenerateDefaultInstances();
}
