/**
 * \file cf_server.c
 * \brief The Classic Forum application server
 * \author Christian Kruse, <cjk@wwwtech.de>
 *
 * This is the Classic Forum application server.
 */

#include "cf_server.h"

static cf_server_context_t global_context;

int setup_server_environment(const char *pidfile,int check_only) {
  struct stat st;
  FILE *fd;
  pid_t pid;
  char buff[250];
  size_t len;

  if(stat(pidfile,&st) == 0) {
    if((fd = fopen(pidfile,"r")) == NULL) {
      fprintf(stderr,"the PID file (%s) exists! Maybe there is already an instance running " \
        "or the server crashed. However, if there is no instance running you " \
        "should remove the file. Sorry, but I have to exit\n",
        pidfile
      );

      return -1;
    }

    if((len = fread(buff,1,250,fd)) > 0 && len < 249) {
      buff[len] = 0;
      pid = (pid_t)atoi(buff);

      if(kill(pid,0) == 0) {
        fprintf(stderr,"the PID file (%s, PID: %d) exists! Maybe there is already an instance running " \
          "or the server crashed. However, if there is no instance running you " \
          "should remove the file. Sorry, but I have to exit\n",
          pidfile, pid
        );

        fclose(fd);
        return -1;
      }
    }
    else {
      fprintf(stderr,"the PID file (%s) exists! Maybe there is already an instance running " \
        "or the server crashed. However, if there is no instance running you " \
        "should remove the file. Sorry, but I have to exit\n",
        pidfile
      );

      fclose(fd);
      return -1;
    }

    fclose(fd);
  }

  if(check_only == 0) {
    if((fd = fopen(pidfile,"w")) == NULL) {
      fprintf(stderr,"Could not open PID file (%s)!\n",strerror(errno));
      return -1;
    }

    pid = getpid();
    len = snprintf(buff,250,"%d",pid);
    fwrite(buff,len,1,fd);
    fclose(fd);
  }

  return 0;
}

void usage(void) {
  fprintf(stderr,"Usage:\n" \
    "[CF_CONF_FILE=\"/path/to/config\"] cf_server [options]\n\n" \
    "where options are:\n" \
    "\t-p, --pid-file          Path to the pid file (optional)\n" \
    "\t-c, --config-file       Path to the configuration file\n" \
    "\t-d, --daemonize         Detach process from shell\n" \
    "\t-h, --help              Show this help screen\n\n" \
    "One of both must be set: config-file option or CF_CONF_FILE\n" \
    "environment variable\n\n"
  );

  exit(-1);
}

void terminate(int sig) {
  (void)sig;
}

gid_t get_gid(const UChar *gname_u) {
  char *gname = cf_to_utf8(gname_u,-1,NULL);
  struct group *gr = getgrnam(gname);
  free(gname);

  if(gr) return gr->gr_gid;

  perror("getgrnam");
  return 0;
}

uid_t get_uid(const UChar *uname_u) {
  char *uname = cf_to_utf8(uname_u,-1,NULL);
  struct passwd *pwd = getpwnam(uname);
  free(uname);

  if(pwd) return pwd->pw_uid;

  perror("getpwnam");
  return 0;
}

void cleanup_env(cf_server_context_t *cntxt,char *cfgfile,cf_cfg_contexts_t contexts,size_t num,cf_cfg_t *cfg) {
  if(cfgfile) free(cfgfile);
  if(contexts) cf_cfg_destroy_contexts(contexts,num);

  if(cntxt) {
    if(cntxt->std_file) free(cntxt->std_file);
    if(cntxt->err_file) free(cntxt->err_file);
    if(cntxt->pid_file) {
      remove(cntxt->pid_file);
      free(cntxt->pid_file);
    }

    if(cntxt->log_std) fclose(cntxt->log_std);
    if(cntxt->log_err) fclose(cntxt->log_err);
  }

  if(cfg) {
    cf_cfg_destroy_cfg(cfg);
    free(cfg);
  }
}

static struct option server_cmdline_options[] = {
  { "pid-file",         1, NULL, 'p' },
  { "config-file",      1, NULL, 'c' },
  { "daemonize",        0, NULL, 'd' },
  { "help",             0, NULL, 'h' },
  { NULL,               0, NULL, 0   }
};

int main(int argc,char *argv[]) {
  int demonize = 0,lid=0;
  char *cfgfile = NULL,*chdir_dir = NULL,c;
  cf_cfg_contexts_t contexts;
  static const char *cnts[] = {"cf_server"};
  cf_cfg_value_t *cval,*cval1;
  cf_cfg_t *cfg;
  gid_t gid;
  uid_t uid;
  pid_t pid;

  memset(&global_context,0,sizeof(global_context));

  /* read options from commandline */
  while((c = getopt_long(argc,argv,"p:c:dh",server_cmdline_options,&lid)) != (char)-1) {
    switch(c) {
      case 'p':
        if(!optarg) usage();
        global_context.pid_file = strdup(optarg);
        break;
      case 'c':
        if(!optarg) usage();
        cfgfile = strdup(optarg);
        break;
      case 'd':
        demonize = 1;
        break;
      default:
        printf("default: %d (%d)\n",c,lid);
        usage();
    }
  }

  if((cfg = cf_cfg_read_config(cfgfile)) == NULL) return EXIT_FAILURE;
  contexts = cf_cfg_create_contexts(cnts,1);

  signal(SIGPIPE,SIG_IGN);
  signal(SIGINT,terminate);
  signal(SIGTERM,terminate);

  if(global_context.pid_file == NULL) {
    if((cval = cf_cfg_get_value_c(cfg,contexts,1,"PIDFile")) == NULL) {
      fprintf(stderr,"Error: no PID file set, neither in config nor on command line!\n");
      cleanup_env(&global_context,cfgfile,contexts,1,cfg);
      return EXIT_FAILURE;
    }

    global_context.pid_file = cf_to_utf8(cval->value.cval,-1,NULL);
  }

  if((cval = cf_cfg_get_value_c(cfg,contexts,1,"Chroot")) != NULL) {
    chdir_dir = cf_to_utf8(cval->value.cval,-1,NULL);

    if(chdir(chdir_dir) == -1) {
      fprintf(stderr,"could not chdir to chroot dir '%s': %s\n",chdir_dir,strerror(errno));
      free(chdir_dir);
      cleanup_env(&global_context,cfgfile,contexts,1,cfg);
      return EXIT_FAILURE;
    }

    if(chroot(chdir_dir) == -1) {
      fprintf(stderr,"could not chroot to dir '%s': %s\n",chdir_dir,strerror(errno));
      free(chdir_dir);
      cleanup_env(&global_context,cfgfile,contexts,1,cfg);
      return EXIT_FAILURE;
    }

    free(chdir_dir);
  }

  if((cval = cf_cfg_get_value_c(cfg,contexts,1,"UserGroup")) != NULL) {
    cval1 = cf_array_element_at(cval->value.aval,1);
    if((gid = cval1->value.ival) == 0 && (gid = get_gid(cval1->value.cval)) == 0) {
      fprintf(stderr,"config error: cannot set gid!\n");
      cleanup_env(&global_context,cfgfile,contexts,1,cfg);
      return EXIT_FAILURE;
    }

    cval1 = cf_array_element_at(cval->value.aval,0);
    if((uid = cval1->value.ival) == 0 && (uid = get_uid(cval1->value.cval)) == 0) {
      fprintf(stderr,"config error: cannot set uid!\n");
      cleanup_env(&global_context,cfgfile,contexts,1,cfg);
      return EXIT_FAILURE;
    }

    if(setgid(gid) == -1 || setregid(gid,gid) == -1) {
      fprintf(stderr,"config error: cannot set gid! error: %s\n",strerror(errno));
      cleanup_env(&global_context,cfgfile,contexts,1,cfg);
      return EXIT_FAILURE;
    }

    if(setuid(uid) == -1 || setreuid(uid,uid) == -1) {
      fprintf(stderr,"config error: cannot set uid! error: %s\n",strerror(errno));
      cleanup_env(&global_context,cfgfile,contexts,1,cfg);
      return EXIT_FAILURE;
    }
  }
  else {
    if((uid = getuid()) == 0) {
      fprintf(stderr,"You should not run this server as root! Set UserGroup in cforum.conf to an appropriate value!\n");
      cleanup_env(&global_context,cfgfile,contexts,1,cfg);
      return EXIT_FAILURE;
    }

    if((gid = getgid()) == 0) {
      fprintf(stderr,"You should not run this server with gid 0! Set UserGroup in cforum.conf to an appropriate value!\n");
      cleanup_env(&global_context,cfgfile,contexts,1,cfg);
      return EXIT_FAILURE;
    }
  }

  /* be sure that only one instance runs */
  if(setup_server_environment(global_context.pid_file,1) == -1) {
    cleanup_env(&global_context,cfgfile,contexts,1,cfg);
    return EXIT_FAILURE;
  }

  if(demonize) {
    /* we daemonize... */
    switch(pid = fork()) {
      case -1:
        fprintf(stderr,"cf_server: could not fork: %s\n",strerror(errno));
        cleanup_env(&global_context,cfgfile,contexts,1,cfg);
        return EXIT_FAILURE;

      case 0:
        if(setsid() == -1) {
          fprintf(stderr,"cf_server: could not detach from shell: %s\n",strerror(errno));
          cleanup_env(&global_context,cfgfile,contexts,1,cfg);
          return EXIT_FAILURE;
        }
        break;

      default:
        printf("server forked. It's pid is: %d\n",pid);
        cleanup_env(&global_context,cfgfile,contexts,1,cfg);
        return EXIT_SUCCESS;
    }

    fclose(stdout);
    fclose(stderr);
    fclose(stdin);
  }

  cval = cf_cfg_get_value_c(cfg,contexts,1,"ErrorLog");
  global_context.err_file = cf_to_utf8(cval->value.cval,-1,NULL);
  cval = cf_cfg_get_value_c(cfg,contexts,1,"StdLog");
  global_context.std_file = cf_to_utf8(cval->value.cval,-1,NULL);

  global_context.shall_run = 1;
  global_context.log_std = fopen(global_context.std_file,"w");
  global_context.log_err = fopen(global_context.err_file,"w");

  /* be sure that only one instance runs */
  if(setup_server_environment(global_context.pid_file,0) == -1) {
    cleanup_env(&global_context,cfgfile,contexts,1,cfg);
    return EXIT_FAILURE;
  }


  /* TODO: Implement main loop */

  remove(global_context.pid_file);

  cleanup_env(&global_context,cfgfile,contexts,1,cfg);
  return EXIT_SUCCESS;
}

/* eof */
