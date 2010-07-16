/**
 * \file ipcutils.c
 * \author Christian Seiler, <self@christian-seiler.de>
 * \brief IPC utilities for the Classic Forum
 *
 * Small IPC abstraction for the Classic Forum
 */

#include "config.h"
#include "defines.h"

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ipcutils.h"

int cf_ipc_dpopen(const char *filename,char *const argv[],char *const envp[],int *result,pid_t *res_pid) {
  /*
   * "input" here means that the programming calling this function gets its
   * input from that pipe
   */
  int input_pipe[2];
  int output_pipe[2];
  int res;
  int saved_errno;
  pid_t pid;
  
  res = pipe(input_pipe);
  if(res) return -1;

  res = pipe(output_pipe);
  if(res) {
    saved_errno = errno;
    close(input_pipe[0]);
    close(input_pipe[1]);
    errno = saved_errno;
    return -1;
  }

  if(!(pid = fork())) {
    /* child process */
    close(input_pipe[0]);
    close(output_pipe[1]);
    res = dup2(output_pipe[0],0);
    if(res == -1) exit(1);
    res = dup2(input_pipe[1],1);
    if(res == -1) exit(1);
    res = execve(filename,argv,envp);
    exit(1);
  }
  else if(pid < 0) {
    /* failure */
    saved_errno = errno;

    close(input_pipe[0]);
    close(input_pipe[1]);
    close(output_pipe[0]);
    close(output_pipe[1]);

    errno = saved_errno;
    return -1;
  }

  /* parent process */
  close(input_pipe[1]);
  close(output_pipe[0]);

  result[0] = input_pipe[0];
  result[1] = output_pipe[1];
  if(res_pid) *res_pid = pid;

  return 0;
}

int cf_ipc_dpclose(int *pipes,pid_t *pid) {
  int res;
  if(pipes) {
    res = close(pipes[0]);
    if(res) return res;

    res = close(pipes[1]);
    if(res) return res;
  }

  if(pid) return waitpid(*pid,NULL,0);
  else {
    /* just do a generic wait and ignore everything else... */
    wait(NULL);
    return 0;
  }

  return 0;
}


/* eof */
