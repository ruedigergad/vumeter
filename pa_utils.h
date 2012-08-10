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

#ifndef PA_UTILS_H
#define PA_UTILS_H

#include <pulse/context.h>
#include <pulse/introspect.h>

#include <QStringList>

#include "pa_context_helper.h"
#include "pa_streams_helper.h"

void get_source_info_list_callback(pa_context *context, const pa_source_info *info, int eol, void *data);

const QStringList *get_source_names();

bool pa_available();

void pa_shutdown();

#endif //PA_UTILS_H
