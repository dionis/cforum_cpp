/**
 * \file serverlib.c
 * \brief The Classic Forum server library
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * This is the Classic Forum application server library, containing
 * functions used only by the server
 */

#include "serverlib.h"

void cf_log(const char *file,int line,const char *func,cf_server_context_t *context,int level,const char *msg,...) {
  int  ret;
  va_list argp;
  cf_cfg_value_t *val = cf_cfg_get_value_c(context->cfg,context->contexts,context->clen,"LogLevel");
  int my_level = 4;
  FILE *fd;

  if(val != NULL) level = val->value.ival;

  /* we don't log messages the user don't wants to see */
  if(level > my_level) return;

  if((ret = pthread_mutex_lock(&context->lock)) != 0) {
    fprintf(context->log_err,"Error locking mutex: %s",strerror(ret));
  }


  if(level & CF_LOG_ERROR || level & CF_LOG_WARN) fd = context->log_err;
  else fd = context->log_std;

  fprintf(fd,"[%s:%d:%s] ",file,line,func);

  va_start(argp,msg);
  vfprintf(fd,msg,argp);
  va_end(argp);

  fwrite("\n",sizeof("\n")-1,1,fd);

  #ifdef CF_DEBUG
  fprintf(stderr,"[%s:%d:%s] ",file,line,func);
  va_start(argp,msg);
  vfprintf(stderr,msg,argp);
  va_end(argp);
  #endif

  #ifndef NO_FLUSH_LOGS
  fflush(fd);
  #endif

  pthread_mutex_unlock(&context->lock);
}


/* eof */
