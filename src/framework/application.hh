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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <dlfcn.h>

#include "framework/request.hh"
#include "framework/controller.hh"

#include "framework/module_exception.hh"

namespace CForum {
  typedef struct {
    Controller *controller;
    void *handle;
  } cf_module_t;

  class Application {
  public:
    Application();
    virtual ~Application();

    Configparser *getConfigparser();

    void loadModules();
    void run(CGI *, Request *);

  protected:
    Configparser *configparser;
    void loadModule(const char *, const char *);

    std::vector<cf_module_t> modules;

  private:
    Application(const Application &);
    Application &operator=(const Application &);
  };


  inline Configparser *Application::getConfigparser() {
    return configparser;
  }

  typedef Controller *(*cf_init_fun_t)(Application *);

  template <typename to_t, typename from_t> to_t ugly_cast(from_t f) {
    union {
      from_t f; to_t t;
    } u;
    u.f = f;
    return u.t;
  }
}

#endif

/* eof */
