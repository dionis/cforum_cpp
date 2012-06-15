/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief The template class
 * \package templates
 *
 * Contains the basic template class
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

#include "template/template.hh"

namespace CForum {
  static v8::Handle<v8::Value> _pCallback(const v8::Arguments &args) {
    v8::Local<v8::Object> self = args.Holder();
    v8::Local<v8::Object> proto = v8::Local<v8::Object>::Cast(self->GetPrototype());

    if(proto->InternalFieldCount() < 1) {
      return v8::ThrowException(v8::String::New("Oops! Global object not found."));
    }

    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(proto->GetInternalField(0));
    Template *tpl = reinterpret_cast<Template *>(wrap->Value());

    if(tpl == NULL) {
      return v8::ThrowException(v8::String::New("Oops! Global object is NULL."));
    }

    if(args.Length() < 1 || args[0].IsEmpty() || !args[0]->IsString()) {
      return v8::ThrowException(v8::String::New("A variable name is needed as first argument!"));
    }

    v8::Local<v8::Value> val = tpl->getVariable(args[0]);
    if(val->IsUndefined()) {
      if(args.Length() > 1) {
        val = args[1];
      }
    }

    v8::String::Utf8Value value(val->ToString());
    *tpl->getStream() << *value;

    return v8::Undefined();
  }

  static v8::Handle<v8::Value> _vCallback(const v8::Arguments &args) {
    v8::Local<v8::Object> self = args.Holder();
    v8::Local<v8::Object> proto = v8::Local<v8::Object>::Cast(self->GetPrototype());

    if(proto->InternalFieldCount() < 1) {
      return v8::ThrowException(v8::String::New("Oops! Global object not found."));
    }

    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(proto->GetInternalField(0));
    Template *tpl = reinterpret_cast<Template *>(wrap->Value());

    if(tpl == NULL) {
      return v8::ThrowException(v8::String::New("Oops! Global object is NULL."));
    }

    if(args.Length() < 1 || args[0].IsEmpty() || !args[0]->IsString()) {
      return v8::ThrowException(v8::String::New("A variable name is needed as first argument!"));
    }

    v8::Local<v8::Value> val = tpl->getVariable(args[0]);
    if(val->IsUndefined()) {
      if(args.Length() > 1) {
        v8::Local<v8::String> str = args[1]->ToString();
        return str;
      }
    }

    return val;
  }

  static v8::Handle<v8::Value> _eCallback(const v8::Arguments &args) {
    v8::Local<v8::Object> self = args.Holder();
    v8::Local<v8::Object> proto = v8::Local<v8::Object>::Cast(self->GetPrototype());

    if(proto->InternalFieldCount() < 1) {
      return v8::ThrowException(v8::String::New("Oops! Global object not found."));
    }

    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(proto->GetInternalField(0));
    Template *tpl = reinterpret_cast<Template *>(wrap->Value());

    if(tpl == NULL) {
      return v8::ThrowException(v8::String::New("Oops! Global object is NULL."));
    }

    if(args.Length() >= 1) {
      v8::HandleScope scope;

      for(int i=0;i<args.Length();++i) {
        v8::String::Utf8Value value(args[i]);

        *tpl->getStream() << *value;
      }
    }

    return v8::Undefined();
  }

  static v8::Handle<v8::Value> extendCallback(const v8::Arguments &args) {
    v8::Local<v8::Object> self = args.Holder();
    v8::Local<v8::Object> proto = v8::Local<v8::Object>::Cast(self->GetPrototype());

    if(proto->InternalFieldCount() < 1) {
      return v8::ThrowException(v8::String::New("Oops! Global object not found."));
    }

    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(proto->GetInternalField(0));
    Template *tpl = reinterpret_cast<Template *>(wrap->Value());

    if(tpl == NULL) {
      return v8::ThrowException(v8::String::New("Oops! Global object is NULL."));
    }

    if(args.Length() < 1 || args[0].IsEmpty() || !args[0]->IsString()) {
      return v8::ThrowException(v8::String::New("A variable name is needed as first argument!"));
    }

    v8::String::Utf8Value part_name(args[0]);
    std::string fname = tpl->generateFileName(part_name);

    v8::Local<v8::Object> vars;
    if(args.Length() == 2 && !args[1].IsEmpty() && args[1]->IsObject()) {
      vars = args[1]->ToObject();
    }

    tpl->setExtend(fname,vars);

    return v8::Undefined();
  }

  static v8::Handle<v8::Value> partialCallback(const v8::Arguments &args) {
    v8::Local<v8::Object> self = args.Holder();
    v8::Local<v8::Object> proto = v8::Local<v8::Object>::Cast(self->GetPrototype());

    if(proto->InternalFieldCount() < 1) {
      return v8::ThrowException(v8::String::New("Oops! Global object not found."));
    }

    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(proto->GetInternalField(0));
    Template *tpl = reinterpret_cast<Template *>(wrap->Value());

    if(tpl == NULL) {
      return v8::ThrowException(v8::String::New("Oops! Global object is NULL."));
    }

    if(args.Length() < 1 || args[0].IsEmpty() || !args[0]->IsString()) {
      return v8::ThrowException(v8::String::New("A template name is needed as first argument!"));
    }

    v8::String::Utf8Value part_name(args[0]);
    std::string fname = tpl->generateFileName(part_name);

    v8::Local<v8::Object> vars;
    if(args.Length() == 2 && !args[1].IsEmpty() && args[1]->IsObject()) {
      vars = args[1]->ToObject();
    }

    *tpl->getStream() << tpl->evaluateFile(fname, vars);

    return v8::Undefined();
  }


  Template::Template() : _stream(NULL), _extends(), _base_dir() {
    v8::HandleScope scope;

    _global = v8::ObjectTemplate::New();

    _global->SetInternalFieldCount(1);

    _global->Set(v8::String::New("_e"), v8::FunctionTemplate::New(_eCallback));
    _global->Set(v8::String::New("_v"), v8::FunctionTemplate::New(_vCallback));
    _global->Set(v8::String::New("_p"), v8::FunctionTemplate::New(_pCallback));

    _global->Set(v8::String::New("partial"), v8::FunctionTemplate::New(partialCallback));
    _global->Set(v8::String::New("extend"), v8::FunctionTemplate::New(extendCallback));

    _context = v8::Context::New(NULL, _global);
    _scope = boost::make_shared<v8::Context::Scope>(_context);
    _vars = v8::Object::New();

    v8::Local<v8::Object>::Cast(_context->Global()->GetPrototype())->SetInternalField(0, v8::External::New(this));
  }

  Template::Template(v8::ExtensionConfiguration *ext) : _stream(NULL), _extends(), _base_dir() {
    v8::HandleScope scope;

    _global = v8::ObjectTemplate::New();

    _global->SetInternalFieldCount(1);

    _global->Set(v8::String::New("_e"), v8::FunctionTemplate::New(_eCallback));
    _global->Set(v8::String::New("_v"), v8::FunctionTemplate::New(_vCallback));
    _global->Set(v8::String::New("_p"), v8::FunctionTemplate::New(_pCallback));

    _global->Set(v8::String::New("partial"), v8::FunctionTemplate::New(partialCallback));
    _global->Set(v8::String::New("extend"), v8::FunctionTemplate::New(extendCallback));

    _context = v8::Context::New(ext, _global);
    _scope = boost::make_shared<v8::Context::Scope>(_context);
    _vars = v8::Object::New();

    v8::Local<v8::Object>::Cast(_context->Global()->GetPrototype())->SetInternalField(0, v8::External::New(this));
  }

  void Template::setGlobal(const char *nam, v8::Handle<v8::Value> val) {
    _context->Global()->Set(v8::String::New(nam), val);
  }


  void Template::setExtend(const std::string &fname, v8::Local<v8::Object> vars) {
    _extends = Extender(fname,vars);
  }

  std::string Template::generateFileName(const v8::String::Utf8Value &fn) { /* TODO: make a better algorithm to find the template file */
    std::string ret = _base_dir;

    if(ret.empty()) {
      ret = "./";
    }

    ret += *fn;

    return ret;
  }

  std::string Template::evaluate(const v8::Local<v8::Script> &script, v8::Local<v8::Object> vars) {
    v8::HandleScope scope;

    std::ostringstream ostr;
    std::ostream *tmp;
    std::string str;
    v8::Local<v8::Value> key;
    v8::Local<v8::Array> keys;
    v8::Local<v8::Object> tmp_vars,e_vars;
    std::string fname;

    if(vars.IsEmpty()) {
      vars = _vars->Clone();
    }
    else {
      keys = _vars->GetPropertyNames();

      for(uint32_t i = 0; i < keys->Length(); ++i) {
        key = keys->Get(i);
        vars->Set(key, _vars->Get(key));
      }
    }

    tmp_vars = _vars;
    _vars = vars;

    tmp = _stream;
    _stream = &ostr;
    script->Run();
    _stream = tmp;

    str = ostr.str();

    if(!_extends.isEmpty()) {
      e_vars = _extends.getVars();
      if(!e_vars.IsEmpty()) {
        keys = e_vars->GetPropertyNames();

        for(uint32_t i=0;i<keys->Length();++i) {
          key = keys->Get(i);
          vars->Set(key,e_vars->Get(key));
        }
      }

      vars->Set(v8::String::New("_content"),v8::String::New(str.c_str()));
      _vars = vars;

      fname = _extends.getFilename();
      _extends = Extender();

      tmp = _stream;
      _stream = &ostr;
      str = evaluateFile(fname);
      _stream = tmp;
    }

    _vars = tmp_vars;

    return str;
  }

  Template::~Template() {
    _context.Dispose();
  }

}


/* eof */
