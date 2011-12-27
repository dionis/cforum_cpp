/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief The template class
 * \package templates
 *
 * Contains the template parser
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

#include "template.hh"

namespace CForum {
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
          case '$':
            if(strncmp(ptr,"${",2) == 0) {
              rslt += "_e('";
              rslt += tmp;
              rslt += "'); _e(_v('";

              for(ptr+=2;static_cast<size_t>(ptr-str) < len && *ptr != '}';++ptr) {
                rslt += *ptr;
              }

              rslt += "',''));";
              tmp = "";
            }
            else {
              tmp += "$";
            }
            break;

          case '<':
            if(strncmp(ptr,"<%",2) == 0) {
              mode = TemplateParseModeInJS;

              if(tmp.length() > 0) {
                rslt += "_e('";
                rslt += tmp;
                rslt += "');";
              }

              tmp = "";
              ptr += 1;
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
        if(*ptr == '%' && strncmp(ptr,"%>",2) == 0) {
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
}

/* eof */
