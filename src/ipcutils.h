

#ifndef CF_IPC_UTILS_H
#define CF_IPC_UTILS_H

/*!
 * \param \c filename the filename to execute
 * \param \c argv the argv[] array of the program
 * \param \c envp the environment of the program
 * \param \c result a pointer to int[2] that will contain the two file descriptors: the first for reading, the second for writing
 * \param \c res_pid a pointer where the pid of the process should be stored (or NULL if the information is not wanted)
 * \return Returns -1 on failure, 0 on success
 *
 * this function opens two pipes, executes a program and returns the two file
 * descriptors for communicating with that program.
 */
int cf_ipc_dpopen(const char *filename,char *const argv[],char *const envp[],int *result,pid_t *res_pid);

/*!
 * \param \c pipes a pointer to int[2] that contains the two pipes
 * \param \c pid a pointer to the pid of the process or NULL (then, wait() will be used instead of waitpid())
 * \return Returns -1 on failure, 0 on success
 *
 * this function closes two pipes that were opened by ipc_dpopen() and then
 * waits for the process to terminate
 */
int cf_ipc_dpclose(int *pipes,pid_t *pid);

#endif

/* eof */