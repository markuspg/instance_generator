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

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QException>

class FileOpenFailureException : public QException {
public:
    void raise() const override;
    FileOpenFailureException *clone() const override;
};

#endif // EXCEPTIONS_H
