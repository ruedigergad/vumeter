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

#ifndef COLORCHOOSERBUTTON_H
#define COLORCHOOSERBUTTON_H

#include <QColor>
#include <QPushButton>

/**
 * Special button for color selections.
 *
 * The background of the button is set to the currently selected color.
 * On push a color selection dialog is presented.
 * When the new color is accepted the active color is set to the new color
 * and the background is painted using the new color.
 */
class ColorChooserButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ColorChooserButton(QWidget *parent = 0);

    QColor  getColor();
    void    setColor(QColor color);

private slots:
    void openColorDialog();

private:
    QColor color;
};

#endif // COLORCHOOSERBUTTON_H
