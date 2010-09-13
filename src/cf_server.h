/**
 * \file cf_server.h
 * \brief The Classic Forum application server
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * This is the Classic Forum application server.
 */

#ifndef CF_SERVER_H
#define CF_SERVER_H

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <signal.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>

#include "utils/arrayutils.h"
#include "utils/stringutils.h"
#include "configparser.h"
#include "serverlib.h"

int cf_main_loop_init(cf_server_context_t *context);
int cf_main_loop(cf_server_context_t *context);
int cf_main_loop_destroy(cf_server_context_t *context);

int cf_threading_init_threads(cf_server_context_t *context);
int cf_threading_adjust_threads(cf_server_context_t *context);
int cf_threading_cleaup_threads(cf_server_context_t *context);

#endif

/* eof */
