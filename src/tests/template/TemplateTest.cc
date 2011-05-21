/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief JSON parser interface testing
 * \package unittests
 *
 * Testing the JSON parser interface
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

#include "TemplateTest.h"
#include "template/Template.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TemplateTest);

void TemplateTest::testParser() {
  CForum::Template tpl;

  tpl.setVariable("firstname",v8::String::New("CK"));
  tpl.setVariable("mood",v8::String::New("froh"));

  tpl.evaluateString(std::string("<% /*extend('lala.html')*/ %>\n<h1>'la\nla'</h1>\n<p>Ich, <% _e(_v('firstname','Christian Kruse')) %>, im vollbesitz meiner geistigen kräfte, bin ${mood}. Deshalb..."));

}



/* eof */
