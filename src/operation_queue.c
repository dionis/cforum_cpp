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
  cf_list_init(&queue->operations);
  cf_mutex_init(context,&queue->lock,name,NULL);
}

int cf_opqueue_append_op(cf_server_context_t *context,cf_operation_queue_t *queue,cf_operation_t *op,int statc) {
  CF_THREAD_LM(context,&queue->lock);

  if(statc) cf_list_append_static(&context->opqueue.operations,&op,sizeof(op));
  else cf_list_append(&context->opqueue.operations,&op,sizeof(op));

  CF_THREAD_UM(context,&queue->lock);

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
  cf_list_destroy(&queue->operations,destroy_op);
  cf_mutex_destroy(context,&queue->lock);
}

/* eof */