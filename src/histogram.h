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

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QMainWindow>

#include <memory>
#include <vector>

namespace Ui {
class Histogram;
} // namespace Ui

class Histogram : public QMainWindow
{
    Q_OBJECT
    
public:
    Histogram(std::vector<unsigned int> &&argDurations,
              const QString &argFilename, QWidget *const argParent = nullptr);
    ~Histogram() override;
    
private:
    const std::vector<unsigned int> processDurations;
    const std::unique_ptr<Ui::Histogram> ui;
};

#endif // HISTOGRAM_H
