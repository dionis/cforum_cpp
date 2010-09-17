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
typedef void *(*cf_operator_t)(void *);
typedef void (*cf_operator_cleanup_t)(void *);

typedef struct cf_operation_s cf_operation_t;
typedef struct cf_operation_queue_s cf_operation_queue_t;

#include "config.h"

#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#include "cf_pthread.h"
#include "readline.h"

#include "utils/listutils.h"
#include "configparser.h"


struct cf_operation_s {
  cf_operator_t operator;
  cf_operator_cleanup_t cleanup;
  void *arg;
};

struct cf_operation_queue_s {
  size_t num_operations,num_workers;

  volatile sig_atomic_t shall_run;
  char *name;

  cf_list_head_t operations;
  cf_mutex_t lock;

  cf_cond_t cond;
};

typedef struct cf_opqueue_arg_s {
  cf_operation_queue_t *queue;
  cf_server_context_t *context;
} cf_opqueue_arg_t;

void cf_opqueue_init(cf_server_context_t *context,cf_operation_queue_t *queue,const char *name);
int cf_opqueue_append_op(cf_server_context_t *context,cf_operation_queue_t *queue,cf_operation_t *op,int statc);
void cf_opqeue_destroy(cf_server_context_t *context,cf_operation_queue_t *queue);
void *cf_opqueue_worker(void *arg);

typedef struct cf_listener_s {
  cf_operator_t listener;
  int sock;
  struct sockaddr *addr;
  struct addrinfo *ai;
  socklen_t size;
} cf_listener_t;

typedef struct cf_client_s {
  int sock;
  cf_listener_t *listener;
  cf_rline_t rbuff;
  cf_mem_pool_t wbuff;
} cf_srv_client_t;

struct cf_server_context_s {
  cf_operation_queue_t opqueue;
  cf_array_t workers;

  cf_list_head_t listeners;

  volatile sig_atomic_t shall_run;

  cf_cfg_t *cfg;
  cf_cfg_contexts_t contexts;
  size_t clen;

  char *log_file,*pid_file;

  cf_mutex_t lock;
  FILE *log;
};



void cf_log(cf_server_context_t *context,const char *file,int line,const char *func,unsigned int level,const char *msg,...);
#define CF_LOG(context,level,msg,...) cf_log((context),__FILE__,__LINE__,__FUNCTION__,(level),(msg),## __VA_ARGS__)
#define CF_ERROR(context,msg,...) cf_log((context),__FILE__,__LINE__,__FUNCTION__,CF_LOG_ERROR,(msg),## __VA_ARGS__)
#define CF_WARN(context,msg,...) cf_log((context),__FILE__,__LINE__,__FUNCTION__,CF_LOG_WARN,(msg),## __VA_ARGS__)
#define CF_INFO(context,msg,...) cf_log((context),__FILE__,__LINE__,__FUNCTION__,CF_LOG_INFO,(msg),## __VA_ARGS__)
#define CF_NOTICE(context,msg,...) cf_log((context),__FILE__,__LINE__,__FUNCTION__,CF_LOG_NOTICE,(msg),## __VA_ARGS__)

cf_srv_client_t *cf_srv_get_client(int connfd,cf_listener_t *listener);
void cf_srv_append_client(cf_server_context_t *context,cf_srv_client_t *arg);

int cf_srv_create_listener(cf_server_context_t *context,UChar *sockdesc);
void cf_destroy_listener(void *arg);

void *cf_srv_http_request(void *arg);

/* cf_server API */
int cf_main_loop_init(cf_server_context_t *context);
int cf_main_loop(cf_server_context_t *context);
int cf_main_loop_destroy(cf_server_context_t *context);

enum cf_socket_type_e {
  CF_SOCKET_TYPE_LISTENER=0,
  CF_SOCKET_TYPE_CLIENT
};

typedef struct {
  enum cf_socket_type_e type;
  void *data;
} cf_socket_t;

enum cf_selector_mode_e {
  CF_MODE_READ  = 1,
  CF_MODE_WRITE = 2
};

int cf_main_loop_add_socket(cf_server_context_t *context,cf_srv_client_t *client,enum cf_selector_mode_e mode);

int cf_threading_init_threads(cf_server_context_t *context);
int cf_threading_adjust_threads(cf_server_context_t *context);
int cf_threading_cleanup_threads(cf_server_context_t *context);

int cf_set_blocking(int fd);
int cf_set_nonblocking(int fd);

#endif

/* eof */
