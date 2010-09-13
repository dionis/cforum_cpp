/**
 * \file cf_server_kqueue.c
 * \brief The Classic Forum application server main loop implemented with BSD's kqueue
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * This is the Classic Forum application server main loop, implemented with
 * the BSD's kqueue(2) API.
 */

#include <sys/event.h>
#include <sys/time.h>

#include "serverlib.h"

static int kq = -1,nchanges = 0;
static struct kevent change;

int cf_main_loop_init(cf_server_context_t *context) {
  cf_list_element_t *elem;
  cf_listener_t *srv;

  kq = kqueue();
  if(kq == -1) {
    CF_ERROR(context,"Error in kqueue(): %s",strerror(errno));
    return -1;
  }

  memset(&change,0,sizeof(change));
  for(elem = context->listeners.elements;elem;elem = elem->next,++nchanges) {
    srv = (cf_listener_t *)elem->data;
    EV_SET(&change,srv->sock,EVFILT_READ,EV_ADD|EV_ENABLE,0, 0, srv);
  }

  return 0;
}

int cf_main_loop(cf_server_context_t *context) {
  struct kevent *event;
  cf_listener_t *srv;
  struct timespec timeout;
  int numev,i,connfd;
  socklen_t size;

  event = cf_alloc(NULL,nchanges,sizeof(*event),CF_ALLOC_CALLOC);

  timeout.tv_sec = 10;
  timeout.tv_nsec = 0;

  numev = kevent(kq,&change,nchanges,event,nchanges,&timeout);
  CF_LOG(context,CF_LOG_DBG(3),"Returned from kqueue: %d",numev);

  if(numev == -1) {
    CF_ERROR(context,"Error in kevent(): %s",strerror(errno));
    free(event);
    return -1;
  }
  else if(numev > 0) {
    for(i=0;i<numev;++i) {
      srv = (cf_listener_t *)event[i].udata;

      size = srv->size;
      connfd = accept(srv->sock,srv->addr,&size);

      /* accept-error? */
      if(connfd <= 0) {
        CF_ERROR(context,"accept: %s\n",strerror(errno));
        return 0;
      }

      cf_srv_append_client(context,connfd,srv);
    }
  }

  free(event);

  return 0;
}

int cf_main_loop_destroy(cf_server_context_t *context) {
  (void)context;
  close(kq);
  return 0;
}

/* eof */
