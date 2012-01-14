/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Configuration parser implementation
 * \package configparser
 *
 * This implements the configuration parser interface
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

#include "framework/configparser.hh"

namespace CForum {
  Configparser::Configparser() : _evaluator(), _script(NULL), _result(NULL) { }
  Configparser::Configparser(const Configparser &) : _evaluator(), _script(NULL), _result(NULL) { }

  std::string Configparser::findFile() {
    static const char *locations[] = {
      "./cforum.js",
      "/etc/cforum.js",
      "/usr/local/etc/cforum.js",
      NULL
    };

    int i;
    char *env_loc = getenv("CF_CFORUM_JS");
    struct stat st;

    if(env_loc != NULL && stat(env_loc,&st) == 0) {
      return std::string(env_loc);
    }


    for(i=0;locations[i] != NULL;++i) {
      if(stat(locations[i],&st) == 0) {
        return std::string(locations[i]);
      }
    }

    throw ConfigErrorException("No configuration file could be found!",CForumException::FileNotFound);
  }

  void Configparser::parse() {
    _filename = findFile();

    try {
      _script = _evaluator.compileFile(_filename);
      _result = _evaluator.evaluateScript(_script);
    }
    catch(JSEvaluatorException &e) {
      throw ConfigErrorException(e.getMessage(),e.getCode());
    }
  }

  v8::Handle<v8::Value> Configparser::getValue(const std::string &name) {
    v8::Local<v8::Object> obj = _result->ToObject();
    v8::Handle<v8::String> str = v8::String::New(name.c_str(), name.length());
    v8::Local<v8::Value> val = obj->Get(str);

    return val;
  }

  Configparser::~Configparser() {
  }

}


/* eof */
