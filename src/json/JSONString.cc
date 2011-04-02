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

#include "JSONString.h"

namespace CForum {
  namespace JSON {
    String::String() : Element(JSONTypeString), _data() {
    }

    String::String(UnicodeString &str) : _data(str) {
    }

    const UnicodeString &String::getData() {
      return _data;
    }

    std::string String::toJSON() {
      return String::toJSONString(_data);
    }

    std::string String::toJSONString(const UnicodeString &str) {
      std::string json("{");
      std::ostringstream ostr(json);
      char buff[50];
      size_t len;

      const UChar *data = str.getBuffer();

      UCharCharacterIterator cit(data,str.length());
      UChar c = cit.first();

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
            len = snprintf(buff,49,"%04x",c);
            buff[len] = 0;
            ostr << buff;
          }

          c = cit.next();
        }
      } while(c != CharacterIterator::DONE);

      ostr << "\"";

      return json;

    }

    String::~String() {
    }

  }
}