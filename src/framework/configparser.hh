/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Configuration parser interface
 * \package configparser
 *
 * This defines the configuration parser interface
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

#ifndef CF_CONFIGPARSER_H
#define CF_CONFIGPARSER_H

#include <vector>
#include <string>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <cstdlib>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <v8.h>

#include "jsevaluator/js_evaluator.hh"
#include "framework/config_error_exception.hh"

namespace CForum {
  class Configparser {
  public:
    Configparser();

    ~Configparser();

    std::string findFile();
    void parse(std::string = "");

    v8::Local<v8::Value> getValue(const std::string &);
    v8::Local<v8::Value> getByPath(const std::string &);

    std::string getStrValue(const std::string &);

  private:
    Configparser(const Configparser &);


    std::string _filename;

    JSEvaluator _evaluator;

    v8::Handle<v8::Script> _script;
    v8::Handle<v8::Value> _result;

  };


}


#endif

/* eof */
