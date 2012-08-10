/*
 *  Copyright 2010, 2011 Ruediger Gad
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

#ifndef PULSEAUDIOADAPTER_H
#define PULSEAUDIOADAPTER_H

#include <QObject>

class PulseAudioAdapter : public QObject
{
    Q_OBJECT

public:
    explicit PulseAudioAdapter(QObject *parent = 0);
    ~PulseAudioAdapter();

    void audioDataCallBack(const float *data);

    Q_INVOKABLE void connectAudioStream(const QString &device);
    Q_INVOKABLE void connectAudioStream(const char *device);

    Q_INVOKABLE void disconnectAudioStream();

    Q_INVOKABLE QStringList* getAvailableSources();

    Q_INVOKABLE double linearVolToDb(float vol);

signals:
    void audioDataReceived(float data);

public slots:

};

#endif // PULSEAUDIOADAPTER_H
