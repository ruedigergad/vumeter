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

#include "pa_streams_helper.h"
#include "pa_utils.h"
#include "pulseaudioadapter.h"

#include <cmath>

pa_stream *stream = NULL;

void connect_to_vu_meter_stream(const char* source, void *data, int numberOfSamples){
    pa_buffer_attr attr;
    attr.fragsize = sizeof(float) * numberOfSamples;
    attr.maxlength = (uint32_t) -1;

    pa_sample_spec sample_specification;
    sample_specification.format = PA_SAMPLE_FLOAT32;
    sample_specification.rate = 25 * numberOfSamples;
    sample_specification.channels = 1;

    qDebug("Connecting to VU meter stream...");
    stream = pa_stream_new(context, "VU Meter Stream", &sample_specification, NULL);

    pa_stream_set_state_callback(stream, stream_state_callback, data);
    pa_stream_connect_record(stream,
                             source,
                             &attr,
                             (pa_stream_flags_t) (PA_STREAM_DONT_MOVE|PA_STREAM_PEAK_DETECT|PA_STREAM_ADJUST_LATENCY));
}

void disconnect_stream(){
    if(stream != NULL){
        pa_threaded_mainloop_lock(threaded_main_loop);
        pa_stream_disconnect(stream);
        qDebug("Waiting for stream to terminate...");
        pa_threaded_mainloop_wait(threaded_main_loop);
        stream = NULL;
        pa_threaded_mainloop_unlock(threaded_main_loop);
    }
}

void stream_read_callback(pa_stream *stream, size_t length, void *data){
    const void *audio_data;
    float vol = 0;
//    float currentVolume = 0;
//    int n = 0;
    PulseAudioAdapter *adapter;
    adapter = (PulseAudioAdapter *) data;

    pa_stream_peek(stream, &audio_data, &length);
    pa_stream_drop(stream);
/*
    if(vuMeter->getInterpolationEnabled()){
        for(size_t i = 0; i < length; i++){
            currentVolume = ((const float*) audio_data)[i];

            if(currentVolume != 0){
                vol += fabs(currentVolume);
                n++;
            }
        }

        if(n > 0){
            vol /= (float) n;
        }else{
            vol = 0.;
        }
    }else{
*/
    vol = ((const float*) audio_data)[0];
//    }

    adapter->audioDataCallBack(&vol);
}

void stream_state_callback(pa_stream *stream, void *data){
    pa_stream_state state = pa_stream_get_state(stream);
    qDebug("Stream state changed: %d", state);

    switch(state){
    case PA_STREAM_READY:
        const pa_sample_spec *spec;
        spec = pa_stream_get_sample_spec(stream);
        qDebug("Sample Specification:\n %d, %d, %s ",
               spec->channels,
               spec->rate,
               pa_sample_format_to_string(spec->format));

        qDebug("Connecting read callback...");
        pa_stream_set_read_callback(stream, stream_read_callback, data);
        break;
    case PA_STREAM_TERMINATED:
        qDebug("Stream terminated. Sending signal...");
        pa_threaded_mainloop_signal(threaded_main_loop, 0);
        break;
    case PA_STREAM_FAILED:
        qDebug("Connecting to stream failed!");
        break;
    default:
        break;
    }
}
