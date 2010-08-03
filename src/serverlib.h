/**
 * \file cf_server.h
 * \brief The Classic Forum application server
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * This is the Classic Forum application server.
 */

#ifndef CF_SERVERLIB_H
#define CF_SERVERLIB_H

#include "config.h"
#include "defines.h"

#include "utils/listutils.h"

#include <pthread.h>

typedef void *(*cf_operator_t)(void *);

typedef struct cf_operation_s {
  cf_operator_t operator;
  void *arg;
} cf_operation_t;

typedef struct cf_operation_queue_s {
  size_t num_operations;
  cf_list_head_t operations;
  pthread_mutex_t lock;
} cf_operation_queue_t;

#endif

/* eof */
