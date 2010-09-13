/*!
 * \file cf_pthread.h
 * \author Christian Kruse, <cjk@wwwtech.de>
 * \brief Pthread function and datatype wrappers
 *
 * This file contains some datatypes to debug pthread
 * locking functions
 */


#ifndef CF_PTHREAD_H
#define CF_PTHREAD_H

#include "config.h"

#include <pthread.h>

/*!
 * This struct is used to store a mutex and it's name
 */
typedef struct s_cf_mutex {
  char *name; /*!< The name of the mutex */
  pthread_mutex_t mutex; /*!< The mutex itself */
} cf_mutex_t;

/*!
 * This struct is used to store a rwlock and it's name
 */
typedef struct s_cf_rwlock {
  char *name; /*!< The name of the rwlock */
  pthread_rwlock_t rwlock; /*!< The rwlock itself */
} cf_rwlock_t;

/*!
 * Stores a PThread conditional, it's name and the lock mutex
 */
typedef struct s_cf_cond {
  char *name; /*!< The name of the condition variable */
  pthread_mutex_t lock; /*!< The locking mutex for this conditional */
  pthread_cond_t cond; /*!< the conditional itself */
} cf_cond_t;

#include "serverlib.h"

#define CF_THREAD_LM(context,mutex) (cf_lock_mutex((context),__FILE__,__LINE__,__FUNCTION__,(mutex)))   /*!< Lock a mutex */
#define CF_THREAD_UM(context,mutex) (cf_unlock_mutex((context),__FILE__,__LINE__,__FUNCTION__,(mutex))) /*!< Unlock a mutex */

#define CF_THREAD_RW_RD(context,rwlock) (cf_rwlock_rdlock((context),__FILE__,__LINE__,__FUNCTION__,(rwlock))) /*!< Lock a rwlock shared */
#define CF_THREAD_RW_WR(context,rwlock) (cf_rwlock_wrlock((context),__FILE__,__LINE__,__FUNCTION__,(rwlock))) /*!< Lock a rwlock writable */
#define CF_THREAD_RW_UN(context,rwlock) (cf_rwlock_unlock((context),__FILE__,__LINE__,__FUNCTION__,(rwlock))) /*!< Unlock a rwlock */

#define CF_THREAD_CD_SG(context,cnd) cf_cond_signal((context),__FILE__,__LINE__,__FUNCTION__,(cnd)) /*!< Signal a conditional */
#define CF_THREAD_CD_BC(context,cnd) cf_cond_broadcast((context),__FILE__,__LINE__,__FUNCTION__,(cnd)) /*!< Broadcast a conditional */
#define CF_THREAD_CD_WT(context,cnd) cf_cond_wait((context),__FILE__,__LINE__,__FUNCTION__,(cnd)) /*!< wait for a conditional */
#define CF_THREAD_CD_TW(context,cnd,ts) cf_cond_timedwait((context),__FILE__,__LINE__,__FUNCTION__,(cnd),(ts)) /*!< Wait max time for a conditional */

/*!
 * Initialization function for the cf_mutex_t
 * \param context The server context
 * \param mutex The cf_mutex_t object
 * \param name The name of the mutex
 * \param attr The mutex attribute
 */
void cf_mutex_init(cf_server_context_t *context,cf_mutex_t *mutex,const char *name,const pthread_mutexattr_t *attr);

/*!
 * Destroys a mutex
 * \param context The server context
 * \param mutex The mutex to destroy
 */
void cf_mutex_destroy(cf_server_context_t *context,cf_mutex_t *mutex);

/*!
 * This function initializes a rwlock
 * \param context The server context
 * \param rwlock The read-write lock object
 * \param name The name of the read-write lock
 * \param attr The rwlock attribute
 */
void cf_rwlock_init(cf_server_context_t *context,cf_rwlock_t *rwlock,const char *name,const pthread_rwlockattr_t *attr);

/*!
 * Destroys a rwlock
 * \param context The server context
 * \param rwlock The read-write lock object
 */
void cf_rwlock_destroy(cf_server_context_t *context,cf_rwlock_t *rwlock);

/*!
 * This function locks a mutex
 * \param context The server context
 * \param file The caller's file name
 * \param line The caller's line number
 * \param func The caller's function name
 * \param mutex The mutex object
 */
void cf_lock_mutex(cf_server_context_t *context,const char *file,const int line,const char *func,cf_mutex_t *mutex);

/*!
 * This function unlocks a mutex
 * \param context The server context
 * \param file The caller's file name
 * \param line The caller's line number
 * \param func The caller's function name
 * \param mutex The mutex object
 */
void cf_unlock_mutex(cf_server_context_t *context,const char *file,const int line,const char *func,cf_mutex_t *mutex);

/*!
 * This function locks a rwlock shared
 * \param context The server context
 * \param file The caller's file name
 * \param line The caller's line number
 * \param func The caller's function name
 * \param rwlock The read-write lock
 */
void cf_rwlock_rdlock(cf_server_context_t *context,const char *file,const int line,const char *func,cf_rwlock_t *rwlock);

/*!
 * This function locks a read-write lock writable
 * \param context The server context
 * \param file The caller's file name
 * \param line The caller's line number
 * \param func The caller's function name
 * \param rwlock The read-write lock
 */
void cf_rwlock_wrlock(cf_server_context_t *context,const char *file,const int line,const char *func,cf_rwlock_t *rwlock);

/*!
 * This function unlocks a read-write lock
 * \param context The server context
 * \param file The caller's file name
 * \param line The caller's line number
 * \param func The caller's function name
 * \param rwlock The read-write lock
 */
void cf_rwlock_unlock(cf_server_context_t *context,const char *file,const int line,const char *func,cf_rwlock_t *rwlock);


/*!
 * This function initializes a thread conditional
 * \param context The server context
 * \param name The name of the conditional
 * \param cond The conditional
 * \param attr The attribute for the conditional (can be NULL)
 * \param mattr The attribute for the mutex (can be NULL)
 */
void cf_cond_init(cf_server_context_t *context,cf_cond_t *cond,const char *name,const pthread_condattr_t *attr,const pthread_mutexattr_t *mattr);

/*!
 * This function destroys a thread conditional
 * \param context The server context
 * \param cond The conditional
 */
void cf_cond_destroy(cf_server_context_t *context,cf_cond_t *cond);

/*!
 * Send a signal for the conditional
 * \param context The server context
 * \param file The filename of the caller (__FILE__)
 * \param line The line number of the caller (__LINE__)
 * \param func The caller function (__FUNCTION__)
 * \param cond The conditional
 */
void cf_cond_signal(cf_server_context_t *context,const char *file,const int line,const char *func,cf_cond_t *cond);

/*!
 * Send a broadcast for the conditional
 * \param context The server context
 * \param file The filename of the caller (__FILE__)
 * \param line The line number of the caller (__LINE__)
 * \param func The caller function (__FUNCTION__)
 * \param cond The conditional
 */
void cf_cond_broadcast(cf_server_context_t *context,const char *file,const int line,const char *func,cf_cond_t *cond);

/*!
 * Wait for a signal and/or a broadcast
 * \param context The server context
 * \param file The filename of the caller (__FILE__)
 * \param line The line number of the caller (__LINE__)
 * \param func The caller function (__FUNCTION__)
 * \param cond The conditional
 */
void cf_cond_wait(cf_server_context_t *context,const char *file,const int line,const char *func,cf_cond_t *cond);

/*!
 * Wait for a signal and/or broadcast, but not more than \c ts time
 * \param context The server context
 * \param file The filename of the caller (__FILE__)
 * \param line The line number of the caller (__LINE__)
 * \param func The caller function (__FUNCTION__)
 * \param cond The conditional
 * \param ts The time to wait max
 */
int cf_cond_timedwait(cf_server_context_t *context,const char *file,const int line,const char *func,cf_cond_t *cond,const struct timespec *ts);


#endif

/* eof */
