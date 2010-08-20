/**
 * \file cf_pthread.c
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief Pthread function and datatype wrappers
 *
 * This file contains some functions to debug pthread
 * locking functions
 */

#include "cf_pthread.h"


void cf_mutex_init(cf_server_context_t *context,cf_mutex_t *mutex,const char *name,const pthread_mutexattr_t *attr) {
  (void)context;
  mutex->name = strdup(name);
  pthread_mutex_init(&mutex->mutex,attr);
}

void cf_mutex_destroy(cf_server_context_t *context,cf_mutex_t *mutex) {
  (void)context;
  free(mutex->name);
  pthread_mutex_destroy(&mutex->mutex);
}

void cf_lock_mutex(cf_server_context_t *context,const char *file,const int line,const char *func,cf_mutex_t *mutex) {
  #ifdef CF_LOCK_DEBUG
  struct timeval tv1,tv2;
  struct timezone tz;

  unsigned long long diff;
  #endif

  int status;

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv1,&tz) == -1) return;

  cf_log(context,file,line,func,CF_LOG_LOCK,"PTHREAD MUTEX TRY LOCK '%s'\n",mutex->name);
  #endif

  if((status = pthread_mutex_lock(&mutex->mutex)) != 0) {
    CF_LOG(context,CF_LOG_ERROR,"[%s:%d:%s] pthread_mutex_lock(%s) %s\n",file,line,func,mutex->name,strerror(status));
    exit(-1);
  }

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv2,&tz) == -1) return;

  diff  = tv2.tv_sec * 1000 + tv2.tv_usec;
  diff -= tv1.tv_sec * 1000 + tv1.tv_usec;

  /* why does this happen? It shouldn't… */
  if(diff < 0) diff = 0;

  cf_log(context,file,line,func,CF_LOG_LOCK,"PTHREAD MUTEX LOCKED '%s' %lld\n",mutex->name,diff);
  #endif
}

void cf_unlock_mutex(cf_server_context_t *context,const char *file,const int line,const char *func,cf_mutex_t *mutex) {
  #ifdef CF_LOCK_DEBUG
  struct timeval tv1,tv2;
  struct timezone tz;
  unsigned long long diff;
  #endif

  int status;

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv1,&tz) == -1) return;

  cf_log(context,file,line,func,CF_LOG_LOCK,"PTHREAD MUTEX TRY UNLOCK '%s'\n",mutex->name);
  #endif

  if((status = pthread_mutex_unlock(&mutex->mutex)) != 0) {
    CF_LOG(context,CF_LOG_ERROR,"[%s:%d:%s] pthread_mutex_unlock(%s): %s\n",file,line,func,mutex->name,strerror(status));
    exit(-1);
  }

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv2,&tz) == -1) return;

  diff  = tv2.tv_sec * 1000 + tv2.tv_usec;
  diff -= tv1.tv_sec * 1000 + tv1.tv_usec;

  /* why does this happen? It shouldn't… */
  if(diff < 0) diff = 0;

  cf_log(context,file,line,func,CF_LOG_LOCK,"PTHREAD MUTEX UNLOCK '%s' %lld\n",mutex->name,diff);
  #endif
}


void cf_rwlock_init(cf_server_context_t *context,cf_rwlock_t *rwlock,const char *name,const pthread_rwlockattr_t *attr) {
  (void)context;
  rwlock->name = strdup(name);
  pthread_rwlock_init(&rwlock->rwlock,attr);
}

void cf_rwlock_destroy(cf_server_context_t *context,cf_rwlock_t *rwlock) {
  (void)context;
  free(rwlock->name);
  pthread_rwlock_destroy(&rwlock->rwlock);
}

void cf_rwlock_rdlock(cf_server_context_t *context,const char *file,const int line,const char *func,cf_rwlock_t *rwlock) {
  #ifdef CF_LOCK_DEBUG
  struct timeval tv1,tv2;
  struct timezone tz;
  unsigned long long diff;
  #endif

  int status;

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv1,&tz) == -1) return;

  cf_log(context,file,line,func,CF_LOG_LOCK,"PTHREAD RWLOCK TRY RDLOCK '%s'\n",rwlock->name);
  #endif

  if((status = pthread_rwlock_rdlock(&rwlock->rwlock)) != 0) {
    CF_LOG(context,CF_LOG_ERROR,"[%s:%d:%s] pthread_rwlock_rdlock(%s): %s\n",file,line,func,rwlock->name,strerror(status));
    exit(-1);
  }

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv2,&tz) == -1) return;

  diff  = tv2.tv_sec * 1000 + tv2.tv_usec;
  diff -= tv1.tv_sec * 1000 + tv1.tv_usec;

  /* why does this happen? It shouldn't… */
  if(diff < 0) diff = 0;

  cf_log(context,file,line,func,CF_LOG_LOCK,"PTHREAD RWLOCK RDLOCKED '%s' %lld\n",rwlock->name,diff);
  #endif
}

void cf_rwlock_wrlock(cf_server_context_t *context,const char *file,const int line,const char *func,cf_rwlock_t *rwlock) {
  #ifdef CF_LOCK_DEBUG
  struct timeval tv1,tv2;
  struct timezone tz;
  unsigned long long diff;
  #endif

  int status;

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv1,&tz) == -1) return;

  cf_log(context,file,line,func,CF_LOG_LOCK,"PTHREAD RWLOCK TRY WRLOCK '%s'\n",rwlock->name);
  #endif

  if((status = pthread_rwlock_wrlock(&rwlock->rwlock)) != 0) {
    CF_LOG(context,CF_LOG_ERROR,"[%s:%d:%s] pthread_rwlock_wrlock(%s): %s\n",file,line,func,rwlock->name,strerror(status));
    exit(-1);
  }

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv2,&tz) == -1) return;

  diff  = tv2.tv_sec * 1000 + tv2.tv_usec;
  diff -= tv1.tv_sec * 1000 + tv1.tv_usec;

  /* why does this happen? it shouldn't… */
  if(diff < 0) diff = 0;

  cf_log(context,file,line,func,CF_LOG_LOCK,"PTHREAD RWLOCK WRLOCKED '%s' %lld\n",rwlock->name,diff);
  #endif
}

void cf_rwlock_unlock(cf_server_context_t *context,const char *file,const int line,const char *func,cf_rwlock_t *rwlock) {
  #ifdef CF_LOCK_DEBUG
  struct timeval tv1,tv2;
  struct timezone tz;
  unsigned long long diff;
  #endif

  int status;

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv1,&tz) == -1) return;
  #endif

  if((status = pthread_rwlock_unlock(&rwlock->rwlock)) != 0) {
    CF_LOG(context,CF_LOG_ERROR,"[%s:%d:%s] pthread_rwlock_unlock(%s): %s\n",file,line,func,rwlock->name,strerror(status));
    exit(-1);
  }

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv2,&tz) == -1) return;

  diff  = tv2.tv_sec * 1000 + tv2.tv_usec;
  diff -= tv1.tv_sec * 1000 + tv1.tv_usec;

  /* why does this happen? It shouldn't… */
  if(diff < 0) diff = 0;

  cf_log(CF_ERR,file,line,"PTHREAD RWLOCK UNLOCK '%s' %lld\n",rwlock->name,diff);
  #endif
}


void cf_cond_init(cf_server_context_t *context,const char *name,cf_cond_t *cond,const pthread_condattr_t *attr,const pthread_mutexattr_t *mattr) {
  (void)context;
  cond->name = strdup(name);
  pthread_mutex_init(&cond->lock,mattr);
  pthread_cond_init(&cond->cond,attr);
}

void cf_cond_destroy(cf_server_context_t *context,cf_cond_t *cond) {
  (void)context;

  free(cond->name);
  pthread_mutex_destroy(&cond->lock);
  pthread_cond_destroy(&cond->cond);
}

void cf_cond_signal(cf_server_context_t *context,const char *file,const int line,const char *func,cf_cond_t *cond) {
  #ifdef CF_LOCK_DEBUG
  struct timeval tv1,tv2;
  struct timezone tz;
  unsigned long long diff;
  #endif

  int status;

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv1,&tz) == -1) return;

  cf_log(context,file,line,func,CF_LOG_LOCK,"PTHREAD COND SIGNAL '%s'\n",cond->name);
  #endif

  if((status = pthread_mutex_lock(&cond->lock)) != 0) {
    CF_LOG(context,CF_LOG_ERROR,"[%s:%d:%s] pthread_mutex_lock(%s): %s\n",file,line,func,cond->name,strerror(status));
    exit(-1);
  }

  if((status = pthread_cond_signal(&cond->cond)) != 0) {
    CF_LOG(context,CF_LOG_ERROR,"[%s:%d:%s] pthread_cond_signal(%s): %s\n",file,line,func,cond->name,strerror(status));
    exit(-1);
  }

  pthread_mutex_unlock(&cond->lock);

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv2,&tz) == -1) return;

  diff  = tv2.tv_sec * 1000 + tv2.tv_usec;
  diff -= tv1.tv_sec * 1000 + tv1.tv_usec;

  /* why does this happen? It shouldn't… */
  if(diff < 0) diff = 0;

  cf_log(context,file,line,func,CF_LOG_LOCK,"PTHREAD COND SIGNAL '%s' %lld\n",lock->name,diff);
  #endif
}

void cf_cond_broadcast(cf_server_context_t *context,const char *file,const int line,const char *func,cf_cond_t *cond) {
  #ifdef CF_LOCK_DEBUG
  struct timeval tv1,tv2;
  struct timezone tz;
  unsigned long long diff;
  #endif

  int status;

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv1,&tz) == -1) return;

  cf_log(context,file,line,func,CF_LOG_LOCK,"PTHREAD COND BROADCAST '%s'\n",cond->name);
  #endif

  if((status = pthread_mutex_lock(&cond->lock)) != 0) {
    CF_LOG(context,CF_LOG_ERROR,"[%s:%d:%s] pthread_mutex_lock(%s): %s\n",file,line,func,cond->name,strerror(status));
    exit(-1);
  }

  if((status = pthread_cond_broadcast(&cond->cond)) != 0) {
    CF_LOG(context,CF_LOG_ERROR,"[%s:%d:%s] pthread_cond_broadcast(%s): %s\n",file,line,func,cond->name,strerror(status));
    exit(-1);
  }

  pthread_mutex_unlock(&cond->lock);

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv2,&tz) == -1) return;

  diff  = tv2.tv_sec * 1000 + tv2.tv_usec;
  diff -= tv1.tv_sec * 1000 + tv1.tv_usec;

  /* why does this happen? It shouldn't… */
  if(diff < 0) diff = 0;

  cf_log(context,file,line,func,CF_LOG_LOCK,"PTHREAD COND BROADCAST '%s' %lld\n",lock->name,diff);
  #endif
}

void cf_cond_wait(cf_server_context_t *context,const char *file,const int line,const char *func,cf_cond_t *cond) {
  #ifdef CF_LOCK_DEBUG
  struct timeval tv1,tv2;
  struct timezone tz;
  unsigned long long diff;
  #endif

  int status;

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv1,&tz) == -1) return;

  cf_log(context,file,line,func,"PTHREAD COND WAIT '%s'\n",cond->name);
  #endif

  if((status = pthread_mutex_lock(&cond->lock)) != 0) {
    CF_LOG(context,CF_LOG_ERROR,"[%s:%d:%s] pthread_mutex_lock(%s): %s\n",file,line,func,cond->name,strerror(status));
    exit(-1);
  }

  if((status = pthread_cond_wait(&cond->cond,&cond->lock)) != 0) {
    CF_LOG(context,CF_LOG_ERROR,"[%s:%d:%s] pthread_cond_wait(%s): %s\n",file,line,func,cond->name,strerror(status));
    exit(-1);
  }

  pthread_mutex_unlock(&cond->lock);

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv2,&tz) == -1) return;

  diff  = tv2.tv_sec * 1000 + tv2.tv_usec;
  diff -= tv1.tv_sec * 1000 + tv1.tv_usec;

  /* why does this happen? It shouldn't… */
  if(diff < 0) diff = 0;

  cf_log(context,file,line,func,CF_LOG_LOCK,"PTHREAD COND WAIT '%s' %lld\n",lock->name,diff);
  #endif
}

int cf_cond_timedwait(cf_server_context_t *context,const char *file,const int line,const char *func,cf_cond_t *cond,const struct timespec *ts) {
  #ifdef CF_LOCK_DEBUG
  struct timeval tv1,tv2;
  struct timezone tz;
  unsigned long long diff;
  #endif

  int status;

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv1,&tz) == -1) return;

  cf_log(context,file,line,func,CF_LOG_LOCK,"PTHREAD COND WAIT '%s'\n",cond->name);
  #endif

  if((status = pthread_mutex_lock(&cond->lock)) != 0) {
    CF_LOG(context,CF_LOG_ERROR,"[%s:%d:%s] pthread_mutex_lock(%s): %s\n",file,line,func,cond->name,strerror(status));
    exit(-1);
  }

  if((status = pthread_cond_timedwait(&cond->cond,&cond->lock,ts)) != 0) {
    if(status != ETIMEDOUT) {
      CF_LOG(context,CF_LOG_ERROR,"[%s:%d:%s] pthread_cond_wait(%s): %s\n",file,line,func,cond->name,strerror(status));
      exit(-1);
    }
  }

  pthread_mutex_unlock(&cond->lock);

  #ifdef CF_LOCK_DEBUG
  if(gettimeofday(&tv2,&tz) == -1) return;

  diff  = tv2.tv_sec * 1000 + tv2.tv_usec;
  diff -= tv1.tv_sec * 1000 + tv1.tv_usec;

  /* why does this happen? It shouldn't… */
  if(diff < 0) diff = 0;

  cf_log(context,file,line,func,"PTHREAD COND TIMEDWAIT '%s' %lld\n",lock->name,diff);
  #endif

  return status;
}



/* eof */
