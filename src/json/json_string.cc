/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief JSONString interface
 * \package JSON
 *
 * This implements the JSONString interface
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

#include "json/json_string.h"

namespace CForum {
  namespace JSON {
    String::String() : Element(), _data() { }
    String::String(const UnicodeString &str) : Element(), _data(str) { }
    String::String(const String &str) : Element(), _data(str._data) { }

    String &String::operator=(const String &str) {
      if(this != &str) {
        _data = str._data;
      }

      return *this;
    }

    std::string String::toJSONString(const UnicodeString &str) {
      std::ostringstream ostr;

      const UChar *data = str.getBuffer();

      UCharCharacterIterator cit(data,str.length());
      UChar c = cit.first();

      ostr << "\"";

      do {
        if(c != CharacterIterator::DONE) {
          if(c < 127) {
            switch(c) {
            case '"':
              ostr << "\\\"";

            case '\\':
              ostr << "\\\\";

            case '/':
              ostr << "\\/";

            case '\b':
              ostr << "\\b";

            case '\f':
              ostr << "\\f";

            case '\n':
              ostr << "\\n";

            case '\r':
              ostr << "\\r";

            case '\t':
              ostr << "\\t";

            default:
              ostr << (char)c;
            }
          }
          else {
            ostr << std::hex << std::setw(4) << std::setfill('0') << c << std::dec << std::setw(0);
          }

          c = cit.next();
        }
      } while(c != CharacterIterator::DONE);

      ostr << "\"";

      return ostr.str();

    }

    String::~String() { }

  }
}
