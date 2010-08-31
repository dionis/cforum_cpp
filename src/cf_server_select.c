/**
 * \file cf_server_epoll.c
 * \brief The Classic Forum application server main loop implemented with select(2)
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * This is the Classic Forum application server main loop, implemented with
 * the select(2) API.
 */

#include "serverlib.h"

int cf_main_loop(cf_server_context_t *context) {
  fd_set rfds;
  cf_list_element_t *elem;
  int sock,ret,connfd;
  cf_listener_t *srv;
  struct timeval timeout;
  socklen_t size;

  FD_ZERO(&rfds);

  for(elem = context->listeners.elements;elem;elem = elem->next) {
    srv = (cf_listener_t *)elem->data;

    if(sock < srv->sock) sock = srv->sock;
    FD_SET(srv->sock,&rfds);
  }

  /*
   * since linux developers had the silly idea to modify
   * the timeout struct of select(), we have to re-initialize
   * it in each loop
   */
  memset(&timeout,0,sizeof(timeout));

  /* check every 10 seconds if we shall exit */
  timeout.tv_sec = 10;

  /* wait for incoming connections */
  ret = select(sock+1,&rfds,NULL,NULL,&timeout);

  if(ret > 0) {
    for(elem = context->listeners.elements;elem;elem = elem->next) {
      srv = (cf_listener_t *)elem->data;

      if(FD_ISSET(srv->sock,&rfds)) {
        size = srv->size;
        connfd = accept(srv->sock,srv->addr,&size);

        /* accept-error? */
        if(connfd <= 0) {
          CF_LOG(context,CF_LOG_ERROR,"accept: %s\n",strerror(errno));
          return 0;
        }

        cf_srv_append_client(context,connfd,srv->listener);
      }
    }
  }

  return 0;
}

/* eof */
