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
  Configparser::Configparser() : _evaluator(), _script(), _result(), _parsed(false) { }
  Configparser::Configparser(const Configparser &) : _evaluator(), _script(), _result(), _parsed(false) { }

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

    throw ConfigErrorException("No configuration file could be found!",CForumErrorException::FileNotFound);
  }

  void Configparser::parse(std::string fname) {
    if(fname == "") {
      _filename = findFile();
    }
    else {
      _filename = fname;
    }

    try {
      _script = _evaluator.compileFile(_filename);
      _result = _evaluator.evaluateScript(_script);
      _parsed = true;
    }
    catch(JSEvaluatorException &e) {
      throw ConfigErrorException(e.getMessage(),e.getCode());
    }
  }

  v8::Local<v8::Value> Configparser::getValue(const std::string &name, bool may_be_null) {
    v8::Local<v8::Object> obj = _result->ToObject();
    v8::Local<v8::String> str = v8::String::New(name.c_str(), name.length());
    v8::Local<v8::Value> val = obj->Get(str);

    if(!may_be_null && (val->IsNull() || val->IsUndefined())) {
      throw ConfigErrorException(std::string("Key ") + name + std::string(" does not exist or is null!"), ConfigErrorException::NotExistantOrNull);
    }

    return val;
  }

  std::string Configparser::getStrValue(const std::string &name) {
    v8::Local<v8::Value> val = getValue(name);
    v8::String::Utf8Value utf8(val);

    if(val.IsEmpty()) {
      throw ConfigErrorException("Return value could not be converted to a string!", ConfigErrorException::StringConversionError);
    }

    return std::string(*utf8);
  }

  inline int toInt(const std::string &str, bool *numeric) {
    const char *ptr = str.c_str();
    int num = 0;

    *numeric = true;

    for(; *ptr; ++ptr) {
      if(isdigit(*ptr)) {
        num = num * 10 + (*ptr - '0');
      }
      else {
        *numeric = false;
        return 0;
      }
    }

    return num;
  }

  v8::Local<v8::Value> Configparser::getByPath(const std::string &name, bool may_be_null) {
    std::istringstream iss(name);
    std::vector<std::string> names;
    std::vector<std::string>::iterator it,end;
    v8::Local<v8::Value> cfgval;
    v8::Local<v8::Object> obj;
    v8::Local<v8::Array> ary;
    v8::Local<v8::Value> key;
    v8::Local<v8::Number> nkey;

    std::string str;
    int i;
    bool numeric;

    if(name[0] == '/') {
      str = name.substr(1);
    }
    else {
      str = name;
    }

    if(str[str.length()-1] == '/') {
      str = str.substr(0, str.length() - 1);
    }

    boost::split(names, str, boost::is_any_of("/"));

    it  = names.begin();
    end = names.end();

    cfgval = getValue(*it);
    for(++it; it != end; ++it) {
      i = toInt(*it, &numeric);

      if(numeric) {
        if(!cfgval->IsArray()) {
          if(may_be_null) {
            return v8::Local<v8::Value>::New(v8::Undefined());
          }
          else {
            throw ConfigErrorException("Key \"" + *it + "\" in path <" + name + "> is not an array!", ConfigErrorException::NotAnObjectError);
          }
        }

        obj    = cfgval->ToObject();
        nkey   = v8::Number::New((double)i);
        cfgval = obj->Get(nkey);
      }
      else {
        if(!cfgval->IsObject()) {
          if(may_be_null) {
            return v8::Local<v8::Value>::New(v8::Undefined());
          }
          else {
            throw ConfigErrorException("Key \"" + *it + "\" in path <" + name + "> is not an object!", ConfigErrorException::NotAnObjectError);
          }
        }

        key    = v8::String::New(it->c_str());
        obj    = cfgval->ToObject();
        cfgval = obj->Get(key);
      }

    }

    if(!may_be_null && (cfgval->IsNull() || cfgval->IsUndefined())) {
      throw ConfigErrorException(std::string("Key ") + name + std::string(" does not exist or is null!"), ConfigErrorException::NotExistantOrNull);
    }

    return cfgval;
  }


  Configparser::~Configparser() {}

}


/* eof */
