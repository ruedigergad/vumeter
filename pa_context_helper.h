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

#ifndef PA_CONTEXT_HELPER_H
#define PA_CONTEXT_HELPER_H

#include <pulse/context.h>
#include <pulse/thread-mainloop.h>

extern pa_context           *context;
extern pa_threaded_mainloop *threaded_main_loop;
extern bool                 connected;
extern bool                 failed;

int     context_create();
void    context_state_callback(pa_context *context, void *data);
void    context_stop();

#endif // PA_CONTEXT_HELPER_H
