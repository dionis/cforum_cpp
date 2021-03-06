/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Controller interface: each controller may implement an action
 * \package framework
 *
 * A controller handles a request. Each request may be handled by N controllers.
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

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "framework/request.hh"

namespace CForum {
  class Application; // needed due to circular dependencies

  class Controller {
  public:
    Controller();
    virtual ~Controller();

    virtual void initController(Application *);
    virtual void registerController(Application *) = 0;

    virtual void preRoute(boost::shared_ptr<Request>);
    virtual const std::string handleRequest(boost::shared_ptr<Request>, const std::map<std::string, std::string> &);
    virtual void postRoute(boost::shared_ptr<Request>);

  protected:
    virtual std::string generateFilename(const std::string &);

    boost::shared_ptr<Request> request;
    std::string view;
    Application *app;

  };

  inline void Controller::initController(Application *app) {
    this->app = app;
  }
}

#include "framework/application.hh"

#endif

/* eof */
