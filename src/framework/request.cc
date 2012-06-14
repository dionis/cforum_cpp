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
  static v8::Handle<v8::Value> _getCfg(const v8::Arguments &args) {
    v8::Local<v8::Object> self = args.Holder();

    if(self->InternalFieldCount() < 1) {
      return v8::ThrowException(v8::String::New("Oops! Config object is NULL."));
    }

    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    Configparser *cfg = reinterpret_cast<Configparser *>(wrap->Value());

    if(cfg == NULL) {
      return v8::ThrowException(v8::String::New("Oops! Configparser object is NULL."));
    }

    if(args.Length() < 1 || args[0].IsEmpty() || !args[0]->IsString()) {
      return v8::ThrowException(v8::String::New("A config value path is needed as first argument!"));
    }

    bool may_be_null = true;
    if(args.Length() >= 2 && !args[1].IsEmpty() && args[1]->IsBooleanObject()) {
      may_be_null = args[1]->ToBoolean()->BooleanValue();
    }

    v8::String::Utf8Value nam(args[0]->ToString());
    v8::Local<v8::Value> ret;

    try {
      return cfg->getValue(*nam, may_be_null);
    }
    catch(ConfigErrorException &e) {
      return v8::ThrowException(v8::String::New("Config value is null!"));
    }

    return v8::Undefined();
  }

  static v8::Handle<v8::Value> _getByPathCfg(const v8::Arguments &args) {
    v8::Local<v8::Object> self = args.Holder();

    if(self->InternalFieldCount() < 1) {
      return v8::ThrowException(v8::String::New("Oops! Config object is NULL."));
    }

    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    Configparser *cfg = reinterpret_cast<Configparser *>(wrap->Value());

    if(cfg == NULL) {
      return v8::ThrowException(v8::String::New("Oops! Configparser object is NULL."));
    }

    if(args.Length() < 1 || args[0].IsEmpty() || !args[0]->IsString()) {
      return v8::ThrowException(v8::String::New("A config value path is needed as first argument!"));
    }

    bool may_be_null = true;
    if(args.Length() >= 2 && !args[1].IsEmpty() && args[1]->IsBooleanObject()) {
      may_be_null = args[1]->ToBoolean()->BooleanValue();
    }

    v8::String::Utf8Value nam(args[0]->ToString());

    v8::Local<v8::Value> ret;

    try {
      return cfg->getByPath(*nam, may_be_null);
    }
    catch(ConfigErrorException &e) {
      return v8::ThrowException(v8::String::New("Config value is null!"));
    }

    return v8::Undefined();
  }



  Request::Request() : requestUri(), user(), tpl() { }
  Request::Request(const Request &rq) : requestUri(rq.requestUri), user(rq.user), tpl(rq.tpl) { }

  void Request::initTemplate(boost::shared_ptr<Configparser> configparser) {
    v8::Local<v8::Value> exts = configparser->getByPath("system/views-js");
    v8::Local<v8::Object> extensions;
    v8::Local<v8::Array> keys;
    v8::Local<v8::String> file, name;
    uint32_t i, len;

    if(!exts->IsNull() && !exts->IsUndefined()) {
      extensions = exts->ToObject();
      keys = extensions->GetPropertyNames();
      char **extension_names = new char *[keys->Length()];

      for(len = keys->Length(), i = 0; i < len; ++i) {
        name = keys->Get(i)->ToString();
        file = extensions->Get(name)->ToString();
        v8::String::Utf8Value name_c(name);
        v8::String::Utf8Value file_c(file);

        std::ifstream fd(*file_c, std::ifstream::in);
        std::stringstream sst;

        if(!fd) {
          throw InternalErrorException(std::string("File ") + *file_c + " could not be found!", CForumErrorException::FileNotFound);
        }

        sst << fd.rdbuf();
        fd.close();

        char *nam = strdup(*name_c);
        v8::RegisterExtension(new v8::Extension(nam, strdup(sst.str().c_str())));
        extension_names[i] = nam;
      }

      v8::ExtensionConfiguration *extensions = new v8::ExtensionConfiguration((int)len, const_cast<const char **>(extension_names));
      tpl = boost::make_shared<Template>(extensions);
    }
    else {
      tpl = boost::make_shared<Template>();
    }


    v8::Handle<v8::ObjectTemplate> cfgparser_templ = v8::ObjectTemplate::New();
    cfgparser_templ->SetInternalFieldCount(1);
    cfgparser_templ->Set(v8::String::New("get"), v8::FunctionTemplate::New(_getCfg));
    cfgparser_templ->Set(v8::String::New("getByPath"), v8::FunctionTemplate::New(_getByPathCfg));

    v8::Local<v8::Object> obj = cfgparser_templ->NewInstance();
    obj->SetInternalField(0, v8::External::New(configparser.get()));
    tpl->setGlobal("configparser", obj);
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
