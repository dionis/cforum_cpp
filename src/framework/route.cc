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

#include "framework/route.hh"

namespace CForum {
  Route::Pattern::Pattern() : names(), pattern(), regex() { }
  Route::Pattern::Pattern(const std::string &pat, const std::vector<std::string> &nams) : names(nams), pattern(pat), regex(boost::make_shared<pcrepp::Pcre>(pat)) { }
  Route::Pattern::Pattern(const Route::Pattern &pat) : names(pat.names), pattern(pat.pattern), regex(boost::make_shared<pcrepp::Pcre>(*pat.regex)) { }

  Route::Pattern &Route::Pattern::operator=(const Route::Pattern &pat) {
    if(this != &pat) {
      pattern = pat.pattern;
      names   = pat.names;
      *regex  = *pat.regex;
    }

    return *this;
  }

  Route::Route(boost::shared_ptr<Controller> cnt) : name(), patterns(), controller(cnt) { }
  Route::Route(boost::shared_ptr<Controller> cnt, const std::string &nam) : name(nam), patterns(), controller(cnt) { }
  Route::Route(const Route &r) : name(r.name), patterns(r.patterns), controller(r.controller) { }

  Route &Route::operator=(const Route &r) {
    if(this != &r) {
      name       = r.name;
      patterns   = r.patterns;
      controller = r.controller;
    }

    return *this;
  }

  void Route::addPattern(const std::string &pattern) {
    const char *ptr = pattern.c_str(), *start;
    std::ostringstream ostr;
    std::vector<std::string> names;

    for(; *ptr; ++ptr) {
      switch(*ptr) {
        case '<':
          if(isalpha(*(ptr + 1))) {
            start = ++ptr;

            for(; *ptr && isalnum(*ptr); ++ptr) ;

            /* now we've got a named pattern! */
            if(*ptr == ':') {
              names.push_back(std::string(start, ptr - start));
              start = ++ptr;
              ostr << '(';

              for(; *ptr && *ptr != '>'; ++ptr) {
                switch(*ptr) {
                  case '\\':
                    if(*(ptr + 1) == '>') {
                      ostr << '>';
                      ++ptr;
                      break;
                    }

                  default:
                    ostr << *ptr;
                }
              }

              if(*ptr != '>') {
                throw RouteSyntaxException("Error: named <pattern> is not closed!", RouteSyntaxException::NamedPatternNotClosedError);
              }

              ostr << ')';

              break;
            }
          }

          throw RouteSyntaxException("Error: <pattern> opened but invalid pattern id given!", RouteSyntaxException::NamedPatternIdInvalidError);

        case '\\':
          if(*(ptr + 1) == '<') {
            ostr << '<';
            ++ptr;
            break;
          }

        default:
          ostr << *ptr;
      }

    }

    patterns.push_back(Route::Pattern(ostr.str(), names));
  }

  Route::~Route() { }

}
