/**
 * \file serverlib.c
 * \brief The Classic Forum server library
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * This is the Classic Forum application server library, containing
 * functions used only by the server
 */

#include "serverlib.h"

void cf_log(cf_server_context_t *context,const char *file,int line,const char *func,unsigned int level,const char *msg,...) {
  va_list argp;
  cf_cfg_value_t *val = cf_cfg_get_value_c(context->cfg,context->contexts,context->clen,"LogLevel");
  unsigned int my_level = (unsigned int)-1;
  char *clevel,buff[512];
  time_t t = 0;
  struct tm tm;
  size_t n;

  #ifndef CF_DEBUG /* parameters not needed in non-debug mode */
  (void)file;
  (void)line;
  (void)func;
  #endif

  if(val != NULL) level = (unsigned int)val->value.ival;

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

void cf_cleanup_client(void *arg) {
  cf_srv_client_t *cl = (cf_srv_client_t *)arg;
  cf_mem_cleanup(&cl->wbuff);
}

void cf_srv_append_client(cf_server_context_t *context,int connfd,cf_listener_t *listener) {
  cf_operation_t op;
  cf_srv_client_t *arg = cf_alloc(NULL,1,sizeof(*arg),CF_ALLOC_MALLOC);

  arg->listener = listener;
  arg->sock = connfd;
  memset(&arg->rbuff,0,sizeof(arg->rbuff));
  cf_mem_init(&arg->wbuff);

  op.operator = listener->listener;
  op.arg = arg;
  op.cleanup = cf_cleanup_client;

  cf_opqueue_append_op(context,&context->opqueue,&op,0);
}

int cf_srv_create_listener(cf_server_context_t *context,UChar *sockdesc) {
  struct sockaddr_un *unaddr;
  struct sockaddr *addr;

  char buff[512];
  struct addrinfo hints, *res = NULL;

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
  else if(strncmp(sockdsc,"inet:",5) == 0) {
    ptr = sockdsc + 5;
    if(*ptr == '[') {
      for(ptr1=ptr++;*ptr1 && *ptr1 != ']';++ptr1);

      if(*ptr1 != '\0') {
        if(*(ptr1+1) != '\0') port = atoi(ptr1+2);
        *ptr1 = '\0';
      }
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    snprintf(buff,512,"%d",port);

    if(*ptr == '*') {
      CF_LOG(context,CF_LOG_DBG(5),"getaddrinfo(NULL,%s)",buff);
      if(getaddrinfo(NULL,buff,&hints,&res) != 0) {
        CF_ERROR(context,"Error in getaddrinfo(): %s",strerror(errno));
        free(sockdsc);
        return -1;
      }
    }
    else {
      CF_LOG(context,CF_LOG_DBG(5),"getaddrinfo(%s,%s)",ptr,buff);
      if(getaddrinfo(ptr,buff,&hints,&res) != 0) {
        CF_ERROR(context,"Error in getaddrinfo(): %s",strerror(errno));
        free(sockdsc);
        return -1;
      }
    }

    if((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
      CF_ERROR(context,"Error in socket(): %s (%d)",strerror(errno),errno);
      free(sockdsc);
      return -1;
    }

    if((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one))) == -1) {
      CF_ERROR(context,"Error in setsockopt(): %s",strerror(errno));
      close(sock);
      free(sockdsc);
      return -1;
    }

    addr = (struct sockaddr *)res->ai_addr;
    lstner.size = res->ai_addrlen;
  }
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

  if(res) lstner.ai = res;
  else lstner.ai = NULL;

  lstner.addr = addr;
  lstner.sock = sock;
  lstner.listener = cf_srv_http_request;
  cf_list_append(&context->listeners,&lstner,sizeof(lstner));

  return sock;
}

void cf_destroy_listener(void *arg) {
  cf_listener_t *lst = (cf_listener_t *)arg;
  freeaddrinfo(lst->ai);
}

/* eof */
