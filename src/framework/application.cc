/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Application interface
 * \package framework
 *
 * Application interface
 */

/*
 * Copyright (C) 2011 by Christian Kruse <cjk@wwwtech.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "framework/application.hh"

void usage(const char *bin) {
  fprintf(
    stderr,
    "Usage:\n"                                                       \
    "%s [options]\n\n"                                               \
    "where options are:\n"                                           \
    "\t-c, --config-directory Path to the configuration directory\n" \
    "\t-h, --help Show this help screen\n\n",
    bin
  );
  exit(-1);
}

namespace CForum {
  const char *Application::NOTIFY_PRE_RUN = "notify: just about to run";

  Application::Application() : configparser(boost::make_shared<Configparser>()), router(boost::make_shared<Router>()), notificationCenter(boost::make_shared<NotificationCenter>()), modules(), hooks() {
  }

  Application::Application(const Application &) { }
  Application &Application::operator=(const Application &) {
    return *this;
  }

  void Application::init() {
    if(configfile.length() == 0) {
      configfile = configparser->findFile();
    }

    configparser->parse(configfile);
    loadModules();
  }

  void Application::init(int argc, char *argv[]) {
    scanArgs(argc, argv);
    init();
  }

  static const char *cmdline = "c:h";
  static struct option cmdline_long[] = {
    { "config", 1, NULL, 'c' },
    { "help", 0, NULL, 'h' },
    { NULL, 0, NULL, 0 }
  };

  void Application::scanArgs(int argc, char *argv[]) {
    int c;

    while((c = getopt_long(argc, argv, cmdline, cmdline_long, NULL)) > 0) {
      switch(c) {
        case 'c':
          if(!optarg) {
            usage(argv[0]);
          }

          configfile = optarg;
          break;

        default:
          usage(argv[0]);
      }
    }
  }

  void Application::loadModules() {
    v8::Local<v8::Object> mods = configparser->getByPath("system/modules")->ToObject();
    v8::Local<v8::String> v8path = configparser->getByPath("system/modpath")->ToString();
    v8::Local<v8::Array> keys = mods->GetPropertyNames();
    v8::Local<v8::Number> num;
    v8::Local<v8::String> mod;
    v8::String::Utf8Value path(v8path);

    for(int i = 0, len = keys->Length(); i <= len; ++i) {
      num = v8::Number::New(i);
      mod = mods->Get(num)->ToString();
      v8::String::Utf8Value mod_utf8(mod);

      loadModule(*path, *mod_utf8);
    }

    std::vector<cf_module_t>::iterator it, end = modules.end();

    for(it = modules.begin(); it != end; ++it) {
      it->controller->registerController(this);
    }
  }

  void Application::registerHook(const std::string &nam, boost::shared_ptr<Controller> cntrl) {
    hooks[nam].push_back(cntrl);
  }

  const std::vector<boost::shared_ptr<Controller> > &Application::getHook(const std::string &nam) {
    return hooks[nam];
  }

  void Application::loadModule(const char *path, const char *mod) {
    std::string file = std::string(path) + "/" + mod;
    void *mod_hndl = dlopen(file.c_str(), RTLD_LAZY);
    boost::shared_ptr<Controller> cntrl;
    cf_module_t m;
    cf_init_fun_t fun;

    if(mod_hndl == NULL) {
      throw ModuleException(std::string("Could not open module ") + file, ModuleException::ModuleNotFoundError);
    }

    if((fun = ugly_cast<cf_init_fun_t>(dlsym(mod_hndl, "init"))) == NULL) {
      throw ModuleException(std::string("Could not find init function for module ") + file, ModuleException::InitFunNotFoundError);
    }

    if((cntrl = fun(this)) == NULL) {
      throw ModuleException(std::string("init function returned NULL for module ") + file, ModuleException::InitFunReturnedNullError);
    }

    cntrl->initController(this);

    m.handle = mod_hndl;
    m.controller = cntrl;

    modules.push_back(m);
  }

  void Application::run(boost::shared_ptr<Request> rq) {
    std::vector<cf_module_t>::iterator it, end = modules.end();

    for(it = modules.begin(); it != end; ++it) {
      it->controller->preRoute(rq);
    }

    std::string retval = router->dispatch(rq);

    for(it = modules.begin(); it != end; ++it) {
      it->controller->postRoute(rq);
    }

    rq->output(retval);
  }

  Application::~Application() { }

}

/* eof */
