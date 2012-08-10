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

#include "pa_utils.h"
#include "pulseaudioadapter.h"

PulseAudioAdapter::PulseAudioAdapter(QObject *parent) :
    QObject(parent)
{
    context_create();
}

PulseAudioAdapter::~PulseAudioAdapter(){
    disconnectAudioStream();
    pa_shutdown();
}

void PulseAudioAdapter::audioDataCallBack(const float *data){
    float vol = *data;
    emit audioDataReceived(vol);
}

void PulseAudioAdapter::connectAudioStream(const QString &device) {
    connectAudioStream(device.toLocal8Bit().constData());
}

void PulseAudioAdapter::connectAudioStream(const char *device){
    disconnectAudioStream();

    connect_to_vu_meter_stream(device, this, 1);
}

void PulseAudioAdapter::disconnectAudioStream(){
    disconnect_stream();
}

QStringList* PulseAudioAdapter::getAvailableSources(){
    return (QStringList *) get_source_names();
}

double PulseAudioAdapter::linearVolToDb(float vol){
    return pa_sw_volume_to_dB(pa_sw_volume_from_linear((double) vol));
}
