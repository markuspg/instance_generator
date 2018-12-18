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
#include <QFile>
#include <QTextStream>
#include <QThread>

#include <fstream>
#include <random>

using valVec = std::vector<unsigned int>;

MainWindow::MainWindow(QWidget *const argParent) :
    QMainWindow{argParent},
    ui{std::make_unique<Ui::MainWindow>()}
{
    ui->setupUi(this);
    ui->RBNormal50Distribution->click();
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
        valVec processDurations;
        const auto processesQuantity = ui->SBProcesses->value();
        processDurations.reserve(static_cast<valVec::size_type>(
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
        std::sort(processDurations.begin(), processDurations.end(),
                  std::greater<valVec::value_type>());

        // Storing the problem's general settings
        QTextStream out{&outputFile};
        QTextStream xpress_out{&xPressOutputFile};
        out << "# Machines\n" << ui->SBMachines->value()
            << "\n# Processes\n" << ui->SBProcesses->value()
            << "\n# Process durations\n";
        xpress_out << "Machines: " << ui->SBMachines->value()
                   << "\nProcesses: " << ui->SBProcesses->value()
                   << "\nDurations: [";

        // Store the problem's process durations.
        for (auto cit = processDurations.cbegin();
             cit != processDurations.cend(); ++cit) {
            out << *cit;
            xpress_out << *cit;
            if (cit != (processDurations.cend() - 1)) {
                out << ";";
                xpress_out << ",";
            }
        }
        out << "\n";
        xpress_out << "]\n";
        outputFile.close();
        xPressOutputFile.close();
        ui->statusBar->showMessage("The problem instances have been created");

        const auto histogram = new Histogram{std::move(processDurations),
                                             filename};
        histogram->show();
        histogram->setAttribute(Qt::WA_DeleteOnClose);
    }
}

void MainWindow::GenerateDefaultInstances() {
    // The sizes
    struct Combination {
        const unsigned short machineQty;
        const unsigned short processQty;
    };
    using combiVec = std::vector<Combination>;
    const combiVec combinations{
        {10, 11}, {100, 110}, {1000, 1100},     // ratio 1:1.1
        {6, 9}, {60, 90}, {600, 900},           // ratio 1:1.5
        {4, 10}, {40, 100}, {400, 1000},        // ratio 1:2.5
        {10, 50}, {100, 500}, {1000, 5000},     // ratio 1:5
        {10, 100}, {100, 1000}, {1000, 10000},  // ratio 1:10
        {10, 1000}, {100, 10000}                // ratio 1:100
    };

    enum class EDistributions {
        NORMAL20,
        NORMAL50,
        UNIFORM1,
        UNIFORM20,
        UNIFORM50,
    };
    struct dist_t {
        const EDistributions distribution;
        const std::string distributionName;
    };

    const std::vector<dist_t> distributions {
        {EDistributions::NORMAL20, "normal20"},
        {EDistributions::NORMAL50, "normal50"},
        {EDistributions::UNIFORM1, "uniform1"},
        {EDistributions::UNIFORM20, "uniform20"},
        {EDistributions::UNIFORM50, "uniform50"}
    };

    // Iterate over the different distributions
    for (const auto &distrib : distributions) {
        // Iterate over the different sizes
        for (const auto &comb : combinations) {
            // Iterate over the ten instances
            for (unsigned short int v = 1; v < 11; v++) {
                std::string filename{distrib.distributionName};
                filename.append("-");
                filename.append(std::to_string(comb.machineQty));
                filename.append("-");
                filename.append(std::to_string(comb.processQty));
                filename.append(".");
                filename.append(std::to_string(v));

                const std::string xpressFilename{filename + ".txt"};
                filename.append(".pbl");

                // Create Processes
                valVec processDurations;
                processDurations.reserve(comb.processQty);

                const auto seed = QDateTime::currentMSecsSinceEpoch();
                std::default_random_engine engine{seed};
                switch(distrib.distribution) {
                case EDistributions::NORMAL20: {
                    std::normal_distribution<double> generator{100.0, 20.0};
                    for (unsigned short int w = 0; w < comb.processQty; w++) {
                        double temp = 1;
                        do {
                            temp = generator(engine) + 0.5;
                        } while (temp < 1);
                        processDurations.emplace_back(static_cast<unsigned int>(temp));
                    }
                    break;
                }
                case EDistributions::NORMAL50: {
                    std::normal_distribution<double> generator{100.0, 50.0};
                    for (unsigned short int w = 0; w < comb.processQty; w++) {
                        double temp = 1;
                        do {
                            temp = generator(engine) + 0.5;
                        } while (temp < 1);
                        processDurations.emplace_back(static_cast<unsigned int>(temp));
                    }
                    break;
                }
                case EDistributions::UNIFORM1: {
                    std::uniform_int_distribution<unsigned int> generator{1, 100};
                    for (unsigned short int w = 0; w < comb.processQty; w++) {
                        processDurations.emplace_back(generator(engine));
                    }
                    break;
                }
                case EDistributions::UNIFORM20: {
                    std::uniform_int_distribution<unsigned int> generator(20, 100);
                    for (unsigned short int w = 0; w < comb.processQty; w++) {
                        processDurations.emplace_back(generator(engine));
                    }
                    break;
                }
                case EDistributions::UNIFORM50: {
                    std::uniform_int_distribution<unsigned int> generator(50, 100);
                    for (unsigned short int w = 0; w < comb.processQty; w++) {
                        processDurations.emplace_back(generator(engine));
                    }
                }
                }
                std::sort (processDurations.begin(), processDurations.end(),
                           std::greater<valVec::value_type>());

                std::ofstream outputFileStream;
                std::ofstream xpressOutputFileStream;
                outputFileStream.open(filename, std::ofstream::out
                                      | std::ofstream::trunc);
                if (outputFileStream.fail() == true) {
                    throw FileOpenFailureException{};
                }
                xpressOutputFileStream.open(xpressFilename, std::ofstream::out |
                                            std::ofstream::trunc);
                if (xpressOutputFileStream.fail() == true) {
                    throw FileOpenFailureException{};
                }

                // Storing the problem's general settings
                outputFileStream << "# Machines\n" << comb.machineQty
                                   << "\n# Processes\n" << comb.processQty
                                   << "\n# Process durations\n";
                xpressOutputFileStream << "Machines: " << comb.machineQty
                                          << "\nProcesses: " << comb.processQty
                                          << "\nDurations: [";

                // Store the problem's process durations.
                for (auto cit = processDurations.cbegin();
                     cit != processDurations.cend(); ++cit) {
                    outputFileStream << *cit;
                    xpressOutputFileStream << *cit;
                    if (cit != (processDurations.cend() - 1)) {
                        outputFileStream << ";";
                        xpressOutputFileStream << ",";
                    }
                }
                outputFileStream << "\n";
                xpressOutputFileStream << "]\n";

                outputFileStream.close ();
                xpressOutputFileStream.close ();
                ui->statusBar->showMessage("The problem instances have been created");

                QString tempfilename = QString::fromUtf8(filename.c_str());
                const auto histogram = new Histogram{std::move(processDurations),
                                                     tempfilename};
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
