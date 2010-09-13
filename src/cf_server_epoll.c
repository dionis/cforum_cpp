/**
 * \file cf_server_epoll.c
 * \brief The Classic Forum application server main loop implemented with epoll (Linux)
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * This is the Classic Forum application server main loop, implemented with
 * the Linux epoll(7) API.
 */

#include <sys/epoll.h>

#include "cf_server.h"

#define CF_MAX_EVENTS 10

static int epoll_fd;
static struct epoll_event ev;

int cf_main_loop_init(cf_server_context_t *context) {
  int npolls;
  cf_list_element_t *elem;
  cf_listener_t *srv;

  for(npolls=0,elem=context->listeners.elements;elem;elem=elem->next,++npolls) ;

  if((epoll_fd = epoll_create(npolls)) == -1) {
    CF_ERROR(context,"epoll_create(%d): %s",npolls,strerror(errno));
    return -1;
  }
  ev.events = EPOLLIN;

  for(elem=context->listeners.elements;elem;elem=elem->next) {
    srv = (cf_listener_t *)elem->data;
    ev.data.ptr = srv;

    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, srv->sock, &ev) == -1) {
      CF_ERROR(context,"epoll_ctl: %s",strerror(errno));
      return -1;
    }
  }

  return 0;
}

int cf_main_loop(cf_server_context_t *context) {
  struct epoll_event events[CF_MAX_EVENTS];
  int nfds,i,connfd;
  cf_listener_t *srv;
  socklen_t size;

  nfds = epoll_wait(epoll_fd, events, CF_MAX_EVENTS, 10000);
  if(nfds == -1) {
    CF_ERROR(context,"epoll_wait: %s\n",strerror(errno));
    return -1;
  }

  for(i=0;i<nfds;++i) {
    srv = (cf_listener_t *)events[i].data.ptr;
    size = srv->size;
    connfd = accept(srv->sock,srv->addr,&size);

    /* accept-error? */
    if(connfd <= 0) {
      CF_ERROR(context,"accept: %s\n",strerror(errno));
      return 0;
    }

    cf_srv_append_client(context,connfd,srv);
  }

  return 0;
}

int cf_main_loop_destroy(cf_server_context_t *context) {
  (void)context;
  close(epoll_fd);
  return 0;
}

/* eof */
