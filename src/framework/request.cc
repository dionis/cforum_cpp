/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Request information, including CGI parameters
 * \package framework
 *
 * Request information, including route, CGI parameters, etc, pp
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

#include "framework/request.hh"

namespace CForum {
  Request::Request() : requestUri(), user(), tpl() { }
  Request::Request(const Request &rq) : requestUri(rq.requestUri), user(rq.user), tpl(rq.tpl) { }

  void Request::initTemplate(boost::shared_ptr<Configparser> configparser) {
    v8::Local<v8::Value> views_js = configparser->getByPath("system/views-js");
    if(!views_js->IsNull() && !views_js->IsUndefined()) {
      v8::String::Utf8Value views_js_u(views_js->ToString());

      std::ifstream fd(*views_js_u, std::ifstream::in);
      std::stringstream sst;

      if(!fd) { /* TODO: throw exception */
        perror("fopen");
        exit(-1);
      }
      sst << fd.rdbuf();
      fd.close();

      v8::RegisterExtension(new v8::Extension("viewsjs", strdup(sst.str().c_str())));
      const char *extension_names[] = { "viewsjs" };
      v8::ExtensionConfiguration *extensions = new v8::ExtensionConfiguration(1, extension_names);

      tpl = boost::make_shared<Template>(extensions);
    }
    else {
      tpl = boost::make_shared<Template>();
    }
  }

  Request &Request::operator=(const Request &rq) {
    if(this != &rq) {
      requestUri = rq.requestUri;
      user       = rq.user;
    }

    return *this;
  }

  void Request::setHeader(const std::string &name, const std::string &value) {
    headers[name] = value;
  }

  Request::~Request() { }
}

/* eof */
