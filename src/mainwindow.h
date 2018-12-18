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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <memory>

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
    void on_RBNormal50Distribution_toggled(const bool argChecked);
    void on_RBNormal20Distribution_toggled(const bool argChecked);
    void on_RBUniform1Distribution_toggled(const bool argChecked);
    void on_RBUniform20Distribution_toggled(const bool argChecked);
    void on_RBUniform50Distribution_toggled(const bool argChecked);

private:
    enum class EDistribution {
        NORMAL20,
        NORMAL50,
        UNIFORM1,
        UNIFORM20,
        UNIFORM50
    };

    void GenerateDefaultInstances();
    void GenerateProblemsAccordingToSettings();

    EDistribution choice = EDistribution::NORMAL20;
    const std::unique_ptr<Ui::MainWindow> ui;
};

#endif // MAINWINDOW_H
