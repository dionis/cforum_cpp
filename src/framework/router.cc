/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Router implementation; routes request to the specific modules
 * \package framework
 *
 * Routing is done via several criterias: of course via path, via desired
 * media, via POST/GET difference, etc, pp. Each module can specify a path
 * and one to N additional criterias. If all match, the module is called.
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

#include "framework/router.hh"

namespace CForum {
  Router::Router() : routes() {}

  Router::Router(const Router &router) : routes(router.routes) { }

  Router &Router::operator=(const Router &r) {
    if(this != &r) {
      routes = r.routes;
    }

    return *this;
  }

  std::string Router::dispatch(boost::shared_ptr<Request> rq) {
    const URI uri = rq->getUri();
    std::string path = uri.getPath();
    std::unordered_map<std::string, boost::shared_ptr<Route> >::iterator end = routes.end(), it;
    boost::shared_ptr<Route::ACL> acl;
    bool runIt = false;
    int handlers = 0;
    std::ostringstream retval;
    std::string str;
    size_t i;
    boost::shared_ptr<pcrepp::Pcre> regex;
    std::map<std::string, std::string> vars;
    std::vector<Route::Pattern>::const_iterator pats_end, pats_it;


    for(it = routes.begin(); it != end; ++it) {
      const std::vector<Route::Pattern> &patterns = it->second->getPatterns();
      pats_end = patterns.end();

      for(pats_it = patterns.begin(); pats_it != pats_end; ++pats_it) {
        regex = pats_it->getCompiledPattern();

        if(regex->search(path)) {
          const std::vector<std::string> &names = pats_it->getNames();
          vars.clear();

          for(i = 0; i < names.size(); ++i) {
            vars[names[i]] = (*regex)[i];
          }

          runIt = false;
          acl = it->second->getAcl();

          if(acl && acl->check(rq, vars)) {
            runIt = true;
          }
          else if(!acl) {
            runIt = true;
          }

          if(runIt) {
            retval << it->second->getController()->handleRequest(rq, vars);
            ++handlers;
          }
        }
      }
    }

    if(handlers == 0) {
      throw NotFoundException("No route matched!", NotFoundException::NoRouteMatchedError);
    }

    str = retval.str();

    if(str.empty()) {
      throw InternalErrorException("No output has been generated!", InternalErrorException::NoOutputGeneratedError);
    }

    return str;
  }

}


/* eof */
