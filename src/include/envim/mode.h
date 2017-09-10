/*
 * Copyright (c) 2017 Jean Guyomarc'h
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef __ENVIM_MODE_H__
#define __ENVIM_MODE_H__

#include "envim/types.h"
#include "envim/log.h"
#include <Eina.h>

struct mode
{
   Eina_Stringshare *name;
   e_cursor_shape cursor_shape;
   unsigned int cell_percentage;
   unsigned int blinkon;
   unsigned int blinkoff;
   unsigned int blinkwait;
   unsigned int hl_id;
   unsigned int id_lm;
   char short_name[1];
};

s_mode *mode_new(Eina_Stringshare *name, const char *short_name, unsigned int short_size);
void mode_free(s_mode *mode);

#endif /* ! __ENVIM_MODE_H__ */