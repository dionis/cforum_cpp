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
  cf_socket_t *sock;

  kq = kqueue();
  if(kq == -1) {
    CF_ERROR(context,"Error in kqueue(): %s",strerror(errno));
    return -1;
  }

  memset(&change,0,sizeof(change));
  for(elem = context->listeners.elements;elem;elem = elem->next,++nchanges) {
    srv = (cf_listener_t *)elem->data;

    sock = cf_alloc(NULL,1,sizeof(*sock),CF_ALLOC_MALLOC);
    sock->type = CF_SOCKET_TYPE_LISTENER;
    sock->data = srv;

    EV_SET(&change,srv->sock,EVFILT_READ,EV_ADD|EV_ENABLE,0, 0, sock);
  }

  return 0;
}

int cf_main_loop_add_socket(cf_server_context_t *context,cf_srv_client_t *client,enum cf_selector_mode_e mode) {
  cf_socket_t *sock = cf_alloc(NULL,1,sizeof(*sock),CF_ALLOC_MALLOC);
  sock->type = CF_SOCKET_TYPE_CLIENT;
  sock->data = client;
  int filter = 0;

  (void)context; /* not needed yet */

  if(mode & CF_MODE_READ) filter |= EVFILT_READ;
  if(mode & CF_MODE_WRITE) filter |= EVFILT_WRITE;

  EV_SET(&change,client->sock,filter,EV_ADD|EV_ENABLE,0, 0, sock);
  ++nchanges;

  return 0;
}

int cf_mainloop_remove_socket(cf_server_context_t *context,cf_srv_client_t *client,enum cf_selector_mode_e mode) {
  int filter = 0;

  (void)context; /* not needed yet */

  if(mode & CF_MODE_READ) filter |= EVFILT_READ;
  if(mode & CF_MODE_WRITE) filter |= EVFILT_WRITE;

  EV_SET(&change,client->sock,mode,EV_DELETE,0,0,0);

  return 0;
}

int cf_main_loop(cf_server_context_t *context) {
  struct kevent *event;
  cf_listener_t *srv;
  struct timespec timeout;
  int numev,i,connfd,ret;
  socklen_t size;
  cf_srv_client_t *client;

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
      cf_socket_t *sock = (cf_socket_t *)event[i].udata;
      if(sock->type == CF_SOCKET_TYPE_LISTENER) {
        srv = (cf_listener_t *)sock->data;

        size = srv->size;
        connfd = accept(srv->sock,srv->addr,&size);

        /* accept-error? */
        if(connfd <= 0) {
          CF_ERROR(context,"accept: %s\n",strerror(errno));
          return 0;
        }

        cf_set_nonblocking(connfd);
        client = cf_srv_get_client(connfd,srv);
        cf_main_loop_add_socket(context,client,CF_MODE_READ);
      }
      else {
        /* TODO: handle client: read, write or dispatch */
        client = (cf_srv_client_t *)sock->data;

        if(event[i].filter & EVFILT_READ) {
          ret = cf_read_nonblocking(client->sock,&client->rbuff,1024);
          if(ret == 0) { /* nothing to do, just wait and read more */
          }
          else if(ret == 1) { /* dispatch */
          }
          else { /* must be an error: remove from kqueue */
            cf_mainloop_remove_socket(context,client,CF_MODE_WRITE|CF_MODE_WRITE);
            cf_cleanup_client(client);
            free(client);
            free(sock);
          }
        }

        if(event[i].filter & EVFILT_WRITE) {
          //ret = cf_write_nonblocking(sock->sock,&sock->rbuff,1024);
          ret = 0;
          /* nothing to do, just wait and write more */
          if(ret > 0) {
          }

          /* write buffer's empty. Remove it from the write queue */
          else if(ret == -1) cf_mainloop_remove_socket(context,client,CF_MODE_WRITE);

          /* 0 means, nothing written (because of different reasons, e.g. EWOULDBLOCK);
           * it's not > 0 (bytes written), it's not -1 (write buffer empty), so it has
           * to be an error – remove from kqueue */
          else if(ret != 0) {
            cf_mainloop_remove_socket(context,client,CF_MODE_WRITE|CF_MODE_WRITE);
            cf_cleanup_client(client);
            free(client);
            free(sock);
          }
        }
      }
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
