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
#include "defines.h"

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

typedef struct cf_server_context_s {
  cf_operation_queue_t opqueue;
  volatile sig_atomic_t shall_run;

  char *std_file,*err_file,*pid_file;
  FILE *log_std,*log_err;
} cf_server_context_t;

#endif

/* eof */
