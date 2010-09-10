/**
 * \file ipcutils.h
 * \author Christian Seiler, <self@christian-seiler.de>
 * \brief IPC utilities for the Classic Forum
 *
 * Small IPC abstraction for the Classic Forum
 */

#ifndef CF_IPC_UTILS_H
#define CF_IPC_UTILS_H

#include "config.h"

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


/*!
 * \param filename the filename to execute
 * \param argv the argv[] array of the program
 * \param envp the environment of the program
 * \param result a pointer to int[2] that will contain the two file descriptors: the first for reading, the second for writing
 * \param res_pid a pointer where the pid of the process should be stored (or NULL if the information is not wanted)
 * \return Returns -1 on failure, 0 on success
 *
 * this function opens two pipes, executes a program and returns the two file
 * descriptors for communicating with that program.
 */
int cf_ipc_dpopen(const char *filename,char *const argv[],char *const envp[],int *result,pid_t *res_pid);

/*!
 * \param pipes a pointer to int[2] that contains the two pipes
 * \param pid a pointer to the pid of the process or NULL (then, wait() will be used instead of waitpid())
 * \return Returns -1 on failure, 0 on success
 *
 * this function closes two pipes that were opened by ipc_dpopen() and then
 * waits for the process to terminate
 */
int cf_ipc_dpclose(int *pipes,pid_t *pid);

#endif

/* eof */
