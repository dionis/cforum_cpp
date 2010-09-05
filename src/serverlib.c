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
  char *clevel,buff[512];
  time_t t = 0;
  struct tm tm;
  size_t n;

  if(val != NULL) level = val->value.ival;

  /* we don't log messages the user don't wants to see; but errors should always be logged */
  if(level > my_level && level > CF_LOG_ERROR) return;

  switch(level) {
    case CF_LOG_ERROR:
      clevel = "ERROR";
      break;

    case CF_LOG_WARN:
      clevel = "WARNING";
      break;

    case CF_LOG_INFO:
      clevel = "INFO";
      break;

    case CF_LOG_NOTICE:
      clevel = "NOTICE";
      break;
    
    default:
      clevel = "DEBUG";
  }

  CF_THREAD_LM(context,&context->lock);

  if(time(&t) != (time_t)-1) {
    localtime_r(&t,&tm);
    n = strftime(buff,512,"[%Y-%m-%d %H:%M:%S] ",&tm);
    fwrite(buff,1,n,context->log);

    #ifdef CF_DEBUG
    fwrite(buff,1,n,stderr);
    #endif
  }

  #ifdef CF_DEBUG
  fprintf(context->log,"[%s:%s:%d] [%s] ",file,func,line,clevel);
  #else
  fprintf(context->log,"[%s] ",clevel);
  #endif

  va_start(argp,msg);
  vfprintf(context->log,msg,argp);
  va_end(argp);

  fwrite("\n",sizeof("\n")-1,1,context->log);

  #ifdef CF_DEBUG
  fprintf(stderr,"[%s:%s:%d] [%s] ",file,func,line,clevel);
  va_start(argp,msg);
  vfprintf(stderr,msg,argp);
  va_end(argp);
  fprintf(stderr,"\n");
  #endif

  #ifndef NO_FLUSH_LOGS
  fflush(context->log);
  #endif

  CF_THREAD_UM(context,&context->lock);
}

void cf_srv_append_client(cf_server_context_t *context,int connfd,cf_listener_t *listener) {
  cf_operation_t op;
  cf_listener_arg_t *arg = cf_alloc(NULL,1,sizeof(*arg),CF_ALLOC_MALLOC);

  arg->listener = listener;
  arg->sock = connfd;
  op.operator = listener->listener;
  op.arg = listener;

  cf_opqueue_append_op(context,&context->opqueue,&op,0);
}

int cf_srv_create_listener(cf_server_context_t *context,UChar *sockdesc) {
  struct sockaddr_in *inaddr;
  struct sockaddr_un *unaddr;
  struct sockaddr *addr;

  #ifdef IPV6
  struct sockaddr_in6 *in6addr;
  #endif

  int sock,is_unix = 0,one = 1,port = 6666;
  char *sockdsc = cf_to_utf8(sockdesc,-1,NULL),*ptr,*ptr1;
  cf_listener_t lstner;

  if(strncmp(sockdsc,"unix:",5) == 0) {
    is_unix = 1;

    if((sock = socket(AF_LOCAL,SOCK_STREAM,0)) == -1) {
      CF_ERROR(context,"Error creating socket: %s",strerror(errno));
      free(sockdsc);
      return -1;
    }

    ptr = sockdsc + 5;
    unlink(ptr);

    unaddr = cf_alloc(NULL,1,sizeof(*unaddr),CF_ALLOC_CALLOC);
    unaddr->sun_family = AF_LOCAL;
    strncpy(unaddr->sun_path,ptr,108);

    addr = (struct sockaddr *)unaddr;
    lstner.size = sizeof(*unaddr);
  }
  else if(strncmp(sockdsc,"inet4:",6) == 0) {
    if((sock = socket(AF_INET,SOCK_STREAM,0)) == -1) {
      CF_ERROR(context,"Error creating socket: %s",strerror(errno));
      free(sockdsc);
      return -1;
    }

    ptr = sockdsc + 6;

    if((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one))) == -1) {
      CF_ERROR(context,"Error in setsockopt(): %s",strerror(errno));
      close(sock);
      free(sockdsc);
      return -1;
    }

    for(ptr1=ptr;*ptr1 && *ptr1 != ':';++ptr1);

    if(*ptr1 != '\0') {
      port = atoi(ptr1+1);
      *ptr1 = '\0';
    }

    inaddr = cf_alloc(NULL,1,sizeof(*inaddr),CF_ALLOC_CALLOC);

    if(*ptr == '*') inaddr->sin_addr.s_addr = htonl(INADDR_ANY);
    else {
      if(inet_aton(ptr,&(inaddr->sin_addr)) != 0) {
        CF_ERROR(context,"inet_aton(%s): %s",ptr,strerror(errno));
        free(sockdsc);
        close(sock);
        return -1;
      }
    }
    inaddr->sin_family = AF_INET;
    inaddr->sin_port   = htons(port);

    addr = (struct sockaddr *)inaddr;
    lstner.size = sizeof(*inaddr);
  }
  #ifdef IPV6
  else if(strncmp(sockdsc,"inet6:",6) == 0) {
  }
  #endif
  else {
    CF_ERROR(context,"error: %s is an unknown socket type!",sockdsc);
    free(sockdsc);
    return -1;
  }

  free(sockdsc);

  if(bind(sock,addr,lstner.size) < 0) {
    CF_ERROR(context,"Error in bind(): %s",strerror(errno));
    free(addr);
    close(sock);
    return -1;
  }

  if(listen(sock,CF_LISTENQ) != 0) {
    CF_ERROR(context,"Error in listen(): %s",strerror(errno));
    free(addr);
    close(sock);
    return -1;
  }

  /*
   * we don't care for errors in this chmod() call; if it
   * fails, the administrator has to do it by hand
   */
  if(is_unix) chmod(unaddr->sun_path,S_IRWXU|S_IRWXG|S_IRWXO);

  lstner.addr = addr;
  lstner.sock = sock;
  lstner.listener = cf_srv_http_request;
  cf_list_append(&context->listeners,&lstner,sizeof(lstner));

  return sock;
}

void *cf_srv_http_request(void *arg) {
  (void)arg;
  return NULL;
}

/* eof */
