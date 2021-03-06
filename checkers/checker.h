/* -*- mode: c -*- */

/* Copyright (C) 2003-2016 Alexander Chernov <cher@ejudge.ru> */

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

#ifndef NEED_CORR
#error You must define NEED_CORR macro
#endif /* NEED_CORR */
#ifndef NEED_INFO
#define NEED_INFO 0
#endif /* NEED_INFO */
#ifndef NEED_TGZ
#define NEED_TGZ 0
#endif /* NEED_TGZ */

#include "checker_internal.h"

#if NEED_INFO == 1
#include "testinfo.h"
struct testinfo_subst_handler;
extern int (*testinfo_parse_func)(const char*,testinfo_t*,struct testinfo_subst_handler*);
extern const char *(*testinfo_strerror_func)(int);
//extern testinfo_t test_info;
extern testinfo_t *get_test_info_ptr(void);
#define test_info (*get_test_info_ptr())
#else
struct testinfo_struct;
struct testinfo_subst_handler;
extern int (*testinfo_parse_func)(const char*,struct testinfo_struct*,struct testinfo_subst_handler*);
extern const char *(*testinfo_strerror_func)(int);
#endif /* NEED_INFO */

#ifndef NEED_TGZ
#define NEED_TGZ 0
#endif

#if !defined NEED_MAIN || NEED_MAIN != 0
extern int checker_main(int, char **);
int
main(int argc, char **argv)
{
#if NEED_INFO == 1
  testinfo_parse_func = testinfo_parse;
  testinfo_strerror_func = testinfo_strerror;
#endif

  checker_do_init(argc, argv, NEED_CORR, NEED_INFO, NEED_TGZ);
  return checker_main(argc, argv);
}
#endif
