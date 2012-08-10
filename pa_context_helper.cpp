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

#include <QDebug>

#include "pa_context_helper.h"
#include "pa_streams_helper.h"

pa_context *context = NULL;
pa_threaded_mainloop *threaded_main_loop = NULL;
bool connected = false;
bool failed = false;

int context_create(){
    if(context != NULL){
        return 0;
    }
    connected = false;
    failed = false;

    threaded_main_loop = pa_threaded_mainloop_new();
    pa_mainloop_api *main_loop_api = pa_threaded_mainloop_get_api(threaded_main_loop);
    context =  pa_context_new(main_loop_api, "vumeter");

    pa_context_set_state_callback(context, context_state_callback, NULL);

    if(pa_context_connect(context, NULL, (pa_context_flags_t) PA_CONTEXT_NOAUTOSPAWN, NULL)){
        qDebug("Error connecting to server...");
        pa_threaded_mainloop_stop(threaded_main_loop);
        pa_threaded_mainloop_free(threaded_main_loop);
        return -1;
    }

    pa_threaded_mainloop_start(threaded_main_loop);

    pa_threaded_mainloop_lock(threaded_main_loop);
    while ((! connected) && (! failed)) {
        qDebug("Waiting until connection is established...");
        pa_threaded_mainloop_wait(threaded_main_loop);
        qDebug("Received signal...");
    }
    pa_threaded_mainloop_unlock(threaded_main_loop);

    if(failed){
        pa_context_unref(context);
        context = NULL;
        return -2;
    }

    return 0;
}

void context_state_callback(pa_context *context, void *data){
    (void) data;

    pa_context_state_t state = pa_context_get_state(context);
    qDebug("Context state changed: %d", state);
    switch(state){
    case PA_CONTEXT_READY:
        qDebug("Ready, set, go...");
        connected = true;
        pa_threaded_mainloop_signal(threaded_main_loop, 0);
        break;
    case PA_CONTEXT_FAILED:
        qDebug("Connecting context failed.");
        failed = true;
        pa_threaded_mainloop_signal(threaded_main_loop, 0);
        break;
    default:
        break;
    }
}

void context_stop(){
    if(stream != NULL){
        pa_threaded_mainloop_lock(threaded_main_loop);
        pa_stream_disconnect(stream);
        qDebug("Waiting for stream to terminate...");
        pa_threaded_mainloop_wait(threaded_main_loop);
        stream = NULL;
        pa_threaded_mainloop_unlock(threaded_main_loop);
    }

    if(context != NULL && connected){
        pa_context_disconnect(context);
        context = NULL;
        connected = false;
    }

    pa_threaded_mainloop_stop(threaded_main_loop);
    pa_threaded_mainloop_free(threaded_main_loop);
    threaded_main_loop = NULL;
}
