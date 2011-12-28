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

#ifndef ROUTE_H
#define ROUTE_H

#include <sstream>
#include <pcre++.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "framework/controller.hh"
#include "framework/route_syntax_exception.hh"

namespace CForum {
  class Route {
  public:
    class Pattern {
    public:
      Pattern();
      Pattern(const std::string &, const std::vector<std::string> &);
      Pattern(const Pattern &);

      Pattern &operator=(const Pattern &);

      const std::string &getPattern() const;
      void setPattern(const std::string &);

      const boost::shared_ptr<pcrepp::Pcre> getCompiledPattern() const;

      const std::vector<std::string> &getNames() const;
      void setNames(const std::vector<std::string> &);

    protected:
      std::vector<std::string> names;
      std::string pattern;
      boost::shared_ptr<pcrepp::Pcre> regex;
    };

    class ACL {
    public:
      ACL();
      ACL(const Route::ACL &);

      virtual bool check(boost::shared_ptr<Request>, const std::map<std::string, std::string> &) = 0;

      virtual ~ACL();
    };


    Route(boost::shared_ptr<Controller>);
    Route(boost::shared_ptr<Controller>, const std::string &);
    Route(const Route &);

    Route &operator=(const Route &);

    const std::vector<Route::Pattern> &getPatterns() const;
    void addPattern(const std::string &);

    boost::shared_ptr<Controller> getController();

    void setAcl(boost::shared_ptr<Route::ACL>);
    const boost::shared_ptr<Route::ACL> getAcl() const;

    const std::string &getName() const;
    void setName(const std::string &);

    ~Route();

  protected:
    std::string name;
    std::vector<Route::Pattern> patterns;
    boost::shared_ptr<Controller> controller;
    boost::shared_ptr<Route::ACL> acl;

  };

  inline const std::vector<Route::Pattern> &Route::getPatterns() const {
    return patterns;
  }

  inline boost::shared_ptr<Controller> Route::getController() {
    return controller;
  }

  inline const std::string &Route::getName() const {
    return name;
  }
  inline void Route::setName(const std::string &nam) {
    name = nam;
  }


  inline const std::string &Route::Pattern::getPattern() const {
    return pattern;
  }

  inline void Route::Pattern::setPattern(const std::string &patt) {
    pattern = patt;
    regex   = boost::make_shared<pcrepp::Pcre>(patt);
  }

  inline const boost::shared_ptr<pcrepp::Pcre> Route::Pattern::getCompiledPattern() const {
    return regex;
  }

  inline const std::vector<std::string> &Route::Pattern::getNames() const {
    return names;
  }

  inline void Route::Pattern::setNames(const std::vector<std::string> &nams) {
    names = nams;
  }

  inline void Route::setAcl(boost::shared_ptr<Route::ACL> acl) {
    this->acl = acl;
  }

  inline const boost::shared_ptr<Route::ACL> Route::getAcl() const {
    return acl;
  }

}

#endif

/* eof */
