/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief session testing
 * \package unittests
 *
 * session testing
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

#ifndef SESSION_TEST_H
#define SESSION_TEST_H

#include <cppunit/extensions/HelperMacros.h>

#include <boost/make_shared.hpp>

#include "framework/session.hh"

class SessionTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(SessionTest);
  CPPUNIT_TEST(testWriteSession);
  CPPUNIT_TEST(testReadSession);
  CPPUNIT_TEST(testFailSession);
  CPPUNIT_TEST_SUITE_END();

public:
  void testWriteSession();
  void testReadSession();
  void testFailSession();
};

#endif

/* eof */
