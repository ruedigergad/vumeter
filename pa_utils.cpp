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

#include "pa_utils.h"

#include <string.h>
#include <pulse/ext-stream-restore.h>
#include <QString>
#include <QStringList>

static bool availability_checked = false;
static bool is_available = false;
static bool finished = false;

static QStringList *source_names  = NULL;

void get_source_info_list_callback(pa_context *context, const pa_source_info *info, int eol, void *data){
    (void) context;
    (void) data;

    //char *source_name;

    if(eol == 0){
        qDebug("Source: %s Index: %d", info->name, info->index);

#ifdef Q_WS_MAEMO_5
        if(strstr(info->name, "source") != NULL){
#endif
            qDebug("Using %s as source.", info->name);
            //source_name = new char[strlen(info->name)];
            //strcpy(source_name, info->name);
            source_names->append(QString(info->name));
#ifdef Q_WS_MAEMO_5
        }
#endif

    }else{
        finished = true;
        qDebug("Sending signal...");
        pa_threaded_mainloop_signal(threaded_main_loop, 0);
    }
}

const QStringList *get_source_names(){
    if(source_names == NULL){
        source_names = new QStringList();
        finished = false;

        pa_context_get_source_info_list(context, get_source_info_list_callback, NULL);

        do{
            pa_threaded_mainloop_lock(threaded_main_loop);
            pa_threaded_mainloop_wait(threaded_main_loop);
            qDebug("Received signal...");
        }while(! finished);
        pa_threaded_mainloop_unlock(threaded_main_loop);
    }

    return source_names;
}

bool pa_available(){
    if(! availability_checked){
        int context_available;
        context_available = context_create();
        context_stop();

        is_available = (context_available == 0);
        availability_checked = true;
    }
    return is_available;
}

void pa_shutdown(){
    if(source_names != NULL){
        delete source_names;
    }
    context_stop();
}
