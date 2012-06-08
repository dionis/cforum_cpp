/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CForum threadlist controller
 *
 * CForum threadlist controller, responds to /
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

#ifndef THREADLIST_CONTROLLER_H
#define THREADLIST_CONTROLLER_H

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <sstream>
#include <iostream>
#include <string>

#include "framework/controller.hh"
#include "framework/route.hh"

#include "template/template.hh"

#include "json/json_parser.hh"

namespace CForum {
  class ThreadlistController : public Controller {
  public:
    ThreadlistController();

    virtual void registerController(Application *);
    virtual const std::string handleRequest(boost::shared_ptr<Request>, const std::map<std::string, std::string> &);

    virtual ~ThreadlistController();

  private:
    ThreadlistController(const ThreadlistController &);

  };
}


# endif

/* eof */
