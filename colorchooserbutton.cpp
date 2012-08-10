/*
 *  Copyright 2010 Ruediger Gad
 *
 *  This file is part of vumeter.
 *
 *  vumeter is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  vumeter is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with vumeter.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "colorchooserbutton.h"

#include <QColorDialog>

ColorChooserButton::ColorChooserButton(QWidget *parent) :
    QPushButton(parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(openColorDialog()));
}

QColor ColorChooserButton::getColor(){
    return color;
}

void ColorChooserButton::openColorDialog(){
    QColor col = QColorDialog::getColor(color);
    if(col.isValid()){
        setColor(col);
    }
}

void ColorChooserButton::setColor(QColor color){
    this->color = color;

    setStyleSheet("background-color: rgb(" +
                  QString::number(color.red()) + "," +
                  QString::number(color.green()) + "," +
                  QString::number(color.blue()) + ");");
    /*
    QPalette palette;
    palette.setColor(backgroundRole(), this->color);
    setPalette(palette);
    */
}
