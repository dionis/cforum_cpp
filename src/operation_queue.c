/**
 * \file operation_queue.c
 * \brief The Classic Forum operation queue implementation
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * An operation queue interface, much like NSOperation with
 * OSX/Cocoa
 */

#include "serverlib.h"

void cf_opqueue_init(cf_server_context_t *context,cf_operation_queue_t *queue,const char *name) {
  queue->num_operations = 0;
  queue->num_workers = 0;
  queue->name = strdup(name);
  queue->shall_run = 1;

  cf_list_init(&queue->operations);
  cf_mutex_init(context,&queue->lock,name,NULL);
  cf_cond_init(context,&queue->cond,name,NULL,NULL);
}

int cf_opqueue_append_op(cf_server_context_t *context,cf_operation_queue_t *queue,cf_operation_t *op,int statc) {
  CF_THREAD_LM(context,&queue->lock);

  if(statc) cf_list_append_static(&context->opqueue.operations,op,sizeof(*op));
  else cf_list_append(&context->opqueue.operations,op,sizeof(*op));

  queue->num_operations++;

  CF_THREAD_UM(context,&queue->lock);
  CF_THREAD_CD_SG(context,&queue->cond);

  return 0;
}

static void destroy_op(void *data) {
  cf_operation_t *op = (cf_operation_t *)data;

  if(op->arg) {
    if(op->cleanup) op->cleanup(op->arg);
    free(op->arg);
  }
}

void cf_opqeue_destroy(cf_server_context_t *context,cf_operation_queue_t *queue) {
  CF_THREAD_LM(context,&queue->lock);
  cf_list_destroy(&queue->operations,destroy_op);
  queue->shall_run = 0;

  CF_THREAD_UM(context,&queue->lock);
  CF_THREAD_CD_BC(context,&queue->cond);
  CF_THREAD_LM(context,&queue->lock);

  do {
    CF_THREAD_UM(context,&queue->lock);
    sleep(3);
    CF_THREAD_LM(context,&queue->lock);
  } while(queue->num_workers > 0);

  CF_THREAD_UM(context,&queue->lock);

  free(queue->name);
  cf_mutex_destroy(context,&queue->lock);
  cf_cond_destroy(context,&queue->cond);
}

void *cf_opqueue_worker(void *arg) {
  cf_opqueue_arg_t *myarg = (cf_opqueue_arg_t *)arg;
  cf_list_element_t *el;
  cf_operation_t *op;

  while(myarg->queue->shall_run) {
    CF_THREAD_CD_WT(myarg->context,&myarg->queue->cond);

    CF_LOG(myarg->context,CF_LOG_DBG(1),"(%p) Woke up!",pthread_self());

    CF_THREAD_LM(myarg->context,&myarg->queue->lock);

    if(myarg->queue->num_operations > 0) {
      el = myarg->queue->operations.elements;
      cf_list_delete(&myarg->queue->operations,el);
      myarg->queue->num_operations--;

      CF_THREAD_UM(myarg->context,&myarg->queue->lock);

      CF_LOG(myarg->context,CF_LOG_DBG(1),"(%p) Working on %p",pthread_self(),op);

      op = el->data;

      op->operator(op->arg);
      destroy_op(op);

      free(op);
      free(el);
    }
    else CF_THREAD_UM(myarg->context,&myarg->queue->lock);
  }

  CF_THREAD_LM(myarg->context,&myarg->queue->lock);
  myarg->queue->num_workers--;
  CF_THREAD_UM(myarg->context,&myarg->queue->lock);

  free(arg);

  return NULL;
}

/* eof */