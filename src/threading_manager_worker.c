/**
 * \file threading_manager_worker.c
 * \brief The Classic Forum application server manager/worker threading implementation
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * This implements a manager/worker threading model for the Classic Forum server
 */

#include "cf_server.h"


static size_t min_threads = 0;
static size_t spare_threads = 0;
static size_t max_threads = 0;

static pthread_attr_t thread_attr;
static struct sched_param param;

int cf_threading_init_threads(cf_server_context_t *context) {
  cf_cfg_value_t *cval;
  size_t i;
  int status;
  pthread_t thread;
  cf_opqueue_arg_t arg,*arg1;


  if((cval = cf_cfg_get_value_c(context->cfg,context->contexts,context->clen,"MinThreads")) == NULL || cval->type != CF_CFG_VALUE_INT) {
    CF_ERROR(context,"Error: no MinThreads defined or MinThreads is not an integer parameter!");
    return -1;
  }
  min_threads = cval->value.ival;

  if((cval = cf_cfg_get_value_c(context->cfg,context->contexts,context->clen,"SpareThreads")) == NULL || cval->type != CF_CFG_VALUE_INT) {
    CF_ERROR(context,"Error: no SpareThreads defined or SpareThreads is not an integer parameter!");
    return -1;
  }
  spare_threads = cval->value.ival;

  if((cval = cf_cfg_get_value_c(context->cfg,context->contexts,context->clen,"MinThreads")) != NULL) {
    if(cval->type != CF_CFG_VALUE_INT) {
      CF_ERROR(context,"Error: MaxThreads is not an integer parameter!");
      return -1;
    }
    max_threads = cval->value.ival;
  }

  if(min_threads > max_threads && max_threads != 0) {
    CF_ERROR(context,"Error: MinThreads has to be smaller than MaxThreads!");
    return -1;
  }


  #ifdef sun
  /*
   * On Solaris 2.5, on a uniprocessor machine threads run not
   * asynchronously by default. So we increase the thread concurrency
   * level that threads can run asynchronous. In fact, in concurrency
   * level six, six threads can run "simultanously".
   */
  thr_setconcurrency(6);
  #endif

  pthread_attr_init(&thread_attr);

  /*
   * on very high traffic, the server does accept more and more
   * connections, but does not serve these connection in an
   * acceptable time. So we experience a little bit with thread
   * scheduling...
   */
  #ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
  memset(&param,0,sizeof(param));

  param.sched_priority = (sched_get_priority_min(SCHEDULING) + sched_get_priority_max(SCHEDULING)) / 2;

  pthread_setschedparam(pthread_self(),SCHEDULING,&param);

  param.sched_priority++;
  pthread_attr_setschedparam(&thread_attr,&param);
  pthread_attr_setinheritsched(&thread_attr,PTHREAD_INHERIT_SCHED);
  #endif

  arg.queue = &context->opqueue;
  arg.context = context;

  CF_THREAD_LM(context,&context->opqueue.lock);

  for(i=0;i<min_threads;++i) {
    arg1 = memdup(&arg,sizeof(arg));

    if((status = pthread_create(&thread,&thread_attr,cf_opqueue_worker,arg1)) != 0) {
      CF_THREAD_UM(context,&context->opqueue.lock);
      CF_ERROR(context,"error creating worker thread %u: %s",i,strerror(errno));
      free(arg1);
      return -1;
    }

    CF_NOTICE(context,"created worker %d now!",++context->opqueue.num_workers);
  }

  CF_THREAD_UM(context,&context->opqueue.lock);

  return 0;
}

int cf_threading_adjust_threads(cf_server_context_t *context) {
  cf_opqueue_arg_t arg,*arg1;
  int status;
  pthread_t thread;

  arg.queue = &context->opqueue;
  arg.context = context;

  CF_THREAD_LM(context,&context->opqueue.lock);

  while(context->opqueue.num_workers < context->opqueue.num_operations + spare_threads && (max_threads == 0 || context->opqueue.num_workers < max_threads)) {
    arg1 = memdup(&arg,sizeof(arg));

    if((status = pthread_create(&thread,&thread_attr,cf_opqueue_worker,arg1)) != 0) {
      CF_ERROR(context,"pthread_create: %s",strerror(status));
      CF_THREAD_UM(context,&context->opqueue.lock);
      free(arg1);
      return -1;
    }

    CF_NOTICE(context,"created worker %d now!",++context->opqueue.num_workers);
  }

  CF_THREAD_UM(context,&context->opqueue.lock);

  return 0;
}

int cf_threading_cleaup_threads(cf_server_context_t *context) {
  (void)context;
  return 0;
}


/* eof */
