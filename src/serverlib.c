/**
 * \file serverlib.c
 * \brief The Classic Forum server library
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * This is the Classic Forum application server library, containing
 * functions used only by the server
 */

#include "serverlib.h"

void cf_log(cf_server_context_t *context,const char *file,int line,const char *func,int level,const char *msg,...) {
  va_list argp;
  cf_cfg_value_t *val = cf_cfg_get_value_c(context->cfg,context->contexts,context->clen,"LogLevel");
  int my_level = 4;
  FILE *fd;

  if(val != NULL) level = val->value.ival;

  /* we don't log messages the user don't wants to see */
  if(level > my_level) return;

  CF_THREAD_LM(context,&context->lock);

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

  CF_THREAD_UM(context,&context->lock);
}


/* eof */
