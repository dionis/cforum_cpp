/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief describes a route to a module
 * \package framework
 *
 * This class describes a route to a module; a route
 * is defined by (a) URL pattern(s); if the URL matches,
 * the module will get executed
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

#include "framework/controller.hh"

namespace CForum {
  Controller::Controller() : request() { }

  void Controller::preRoute(boost::shared_ptr<Request>) {}

  const std::string Controller::handleRequest(boost::shared_ptr<Request>, const std::map<std::string, std::string> &) {
    return std::string();
  }

  void Controller::postRoute(boost::shared_ptr<Request>) {}

  Controller::~Controller() { }

}

/* eof */
