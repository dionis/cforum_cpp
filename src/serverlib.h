/**
 * \file cf_server.h
 * \brief The Classic Forum application server
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * This is the Classic Forum application server.
 */

#ifndef CF_SERVERLIB_H
#define CF_SERVERLIB_H

typedef struct cf_server_context_s cf_server_context_t;
typedef struct cf_operation_queue_s  cf_operation_queue_t;
typedef struct cf_operation_s cf_operation_t;

#include "config.h"
#include "defines.h"

#include "cf_pthread.h"

#include "utils/listutils.h"
#include "configparser.h"

typedef void *(*cf_operator_t)(void *);

struct cf_operation_s {
  cf_operator_t operator;
  void *arg;
};

struct cf_operation_queue_s {
  size_t num_operations;
  cf_list_head_t operations;
  cf_mutex_t lock;
};

struct cf_server_context_s {
  cf_operation_queue_t opqueue;
  volatile sig_atomic_t shall_run;

  cf_cfg_t *cfg;
  cf_cfg_contexts_t contexts;
  size_t clen;

  char *std_file,*err_file,*pid_file;

  cf_mutex_t lock;
  FILE *log_std,*log_err;
};



void cf_log(cf_server_context_t *context,const char *file,int line,const char *func,int level,const char *msg,...);
#define CF_LOG(context,level,msg,...) cf_log((context),__FILE__,__LINE__,__FUNCTION__,(level),(msg),## __VA_ARGS__)

#endif

/* eof */
