/* -*- mode: c -*- */
/* $Id$ */

/* Copyright (C) 2003-2013 Alexander Chernov <cher@ejudge.ru> */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "checker_internal.h"

#include "l10n_impl.h"

#if defined __MINGW32__
#include <malloc.h>
#endif

enum { BUFSIZE = 1048576 };

int
checker_read_buf(int ind,
                 const char *name,
                 int eof_error_flag,
                 char *buf,
                 size_t buf_size)
{
  unsigned char format_str[128];
  unsigned char *local_buf;
  size_t format_len, read_len;
  int r;

  if (!buf_size || buf_size > BUFSIZE)
    fatal_CF(_("Invalid buf_size %zu"), buf_size);

  local_buf = (unsigned char*) alloca(buf_size + 1);
  if (!local_buf) fatal_CF(_("alloca(%zu) failed"), buf_size+1);
  memset(local_buf, 0, buf_size + 1);
  format_len = snprintf(format_str, sizeof(format_str), "%%%zus", buf_size);
  if (format_len >= sizeof(format_str))
    fatal_CF(_("Format string is too long: %zu"), format_len);

  r = fscanf(f_arr[ind], format_str, local_buf);
  if (r == 1) {
    read_len = strlen(local_buf);
    if (read_len > buf_size - 1)
      fatal_read(ind, "String `%s' is too long (>= %zu) in %s file",
                 name, read_len, gettext(f_arr_names[ind]));
    strcpy(buf, local_buf);
    return read_len;
  }
  if (r == 0) fatal_CF(_("fscanf returned 0!!!"));
  if (ferror(f_arr[ind]))
    fatal_CF(_("Input error from %s file"), gettext(f_arr_names[ind]));
  if (!eof_error_flag) return -1;
  fatal_read(ind, _("Unexpected EOF while reading '%s'"), name);
}
