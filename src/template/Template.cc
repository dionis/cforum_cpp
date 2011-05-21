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

#include "Template.h"

namespace CForum {
  static void standard_sender(const std::string &str) {
    std::cout << str;
  }

  v8::Handle<v8::Value> _pCallback(const v8::Arguments &args) {
    v8::Local<v8::Object> self = args.Holder();
    v8::Handle<v8::Object> proto = v8::Handle<v8::Object>::Cast(self->GetPrototype());

    if(proto->InternalFieldCount() < 1) {
      return v8::ThrowException(v8::String::New("Oops! Global object not found."));
    }

    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(proto->GetInternalField(0));
    Template *tpl = reinterpret_cast<Template *>(wrap->Value());

    if(tpl == NULL) {
      return v8::ThrowException(v8::String::New("Oops! Global object is NULL."));
    }


    return v8::Undefined();
  }

  v8::Handle<v8::Value> _vCallback(const v8::Arguments &args) {
    v8::Local<v8::Object> self = args.Holder();
    v8::Handle<v8::Object> proto = v8::Handle<v8::Object>::Cast(self->GetPrototype());

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

    v8::Handle<v8::Value> val = tpl->_vars->Get(args[0]);
    if(val->IsUndefined()) {
      if(args.Length() > 1) {
        v8::Handle<v8::String> str = args[1]->ToString();
        return str;
      }
    }

    return val;
  }

  v8::Handle<v8::Value> _eCallback(const v8::Arguments &args) {
    v8::Local<v8::Object> self = args.Holder();
    v8::Handle<v8::Object> proto = v8::Handle<v8::Object>::Cast(self->GetPrototype());

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

        tpl->_sender(std::string(*value));
      }
    }

    return v8::Undefined();
  }

  Template::Global::Global() : _global(v8::ObjectTemplate::New()) {
    _global->SetInternalFieldCount(1);

    _global->Set(v8::String::New("_e"), v8::FunctionTemplate::New(_eCallback));
    _global->Set(v8::String::New("_v"), v8::FunctionTemplate::New(_vCallback));
    _global->Set(v8::String::New("_p"), v8::FunctionTemplate::New(_pCallback));
  }

  Template::Template() : _sender(standard_sender), _handle_scope(), _global(), _context(v8::Context::New(NULL, _global.getGlobal())), _scope(_context), _vars(v8::Object::New()) {
    v8::Handle<v8::Object>::Cast(_context->Global()->GetPrototype())->SetInternalField(0, v8::External::New(this));

    //v8::Handle<v8::Object> obj = _global->NewInstance();
    //obj->SetInternalField(0, v8::External::New(this));
    //_context->Global()->Set(v8::String::New("tpl"), obj);
    //_context->Global()->Set(v8::String::New("firstname"), v8::String::New("Christian"));
    //_context->Global()->Set(v8::String::New("mood"), v8::String::New("sad"));
  }

  std::string Template::parseFile(const std::string &filename) {
    std::ifstream fd(filename.c_str(), std::ifstream::in);
    std::string str;

    if(!fd) {
      throw CForumException(); // TODO: throw correct exception
    }

    str.reserve(1024);

    std::istream_iterator<std::string> begin(fd);
    std::istream_iterator<std::string> end;

    while(begin != end) {
      str += *begin;
      ++begin;
    }

    fd.close();

    return parseString(str);
  }

  std::string Template::parseString(const char *str,size_t len) {
    int mode = TemplateParseModeString;
    const char *ptr;
    std::string tmp,rslt;
    int i;

    for(ptr=str; static_cast<size_t>(ptr-str)<len; ++ptr) {
      if(mode == TemplateParseModeString) {
        switch(*ptr) {
          case '\'':
            tmp += "\\'";
            break;
          case '\\':
            tmp += "\\\\";
            break;
          case '\n':
            tmp += "\\n";
            break;
          case '<':
            if(strncmp(ptr,"<?js",4) == 0) {
              mode = TemplateParseModeInJS;

              if(tmp.length() > 0) {
                rslt += "_e('";
                rslt += tmp;
                rslt += "');";
              }

              tmp = "";
              ptr += 3;
            }
            else {
              tmp += "<";
            }
            break;

          default:
            tmp += *ptr;
            break;
        }
      }
      else {
        if(*ptr == '?' && strncmp(ptr,"?>",2) == 0) {
          if(tmp.length() > 0) {
            rslt += " ";
            rslt += tmp;

            for(i=tmp.length();i>0;--i) {
              if(!isspace(tmp[i]) && tmp[i] != ';') {
                rslt += ';';
                break;
              }
            }
          }

          tmp = "";
          ptr += 1;
          mode = TemplateParseModeString;
        }
        else {
          tmp += *ptr;
        }
      }
    }

    rslt += " ";

    if(mode == TemplateParseModeString) {
      if(tmp.length() > 0) {
        rslt += "_e('";
        rslt += tmp;
        rslt += "');";
      }
    }
    else {
      if(tmp.length() > 0) {
        rslt += tmp;

        for(i=tmp.length();i>0;--i) {
          if(!isspace(tmp[i]) && tmp[i] != ';') {
            rslt += ';';
            break;
          }
        }
      }
    }

    return rslt;
  }

  std::string Template::evaluate(const v8::Handle<v8::Script> &script) {
    v8::Handle<v8::Value> result = script->Run();
    return std::string();
  }

  v8::Handle<v8::Script> Template::compile(const std::string &src) {
    return v8::Script::Compile(v8::String::New(src.c_str()));
  }

  Template::~Template() {
    _context.Dispose();
  }

}


/* eof */
