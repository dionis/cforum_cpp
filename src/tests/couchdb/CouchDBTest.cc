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

#include "CouchDBTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(CouchDBTest);

void CouchDBTest::testInterface() {
  curl_global_init(CURL_GLOBAL_ALL);

  CForum::CouchDB::Server srv("test","localhost");
  srv.setDatabase("test",true);

  CForum::CouchDB::Document doc("{\"_id\": \"test\",\"lala\":\"lulu\"}");

  try {
  srv.putDocument(doc);
  }
  catch(CForum::CouchDB::CouchErrorException &e) {
    printf("error: %s\n",e.getMessage().c_str());
    return;
  }

  CForum::CouchDB::Document doc1 = srv.getDocument("test");
  boost::shared_ptr<CForum::JSON::String> id = boost::dynamic_pointer_cast<CForum::JSON::String>(doc1.getValue("_id"));

  std::string str;
  id->getValue().toUTF8String(str);
  CPPUNIT_ASSERT_EQUAL(std::string("test"),str);

  str = "";
  boost::shared_ptr<CForum::JSON::String> lala = boost::dynamic_pointer_cast<CForum::JSON::String>(doc1.getValue("lala"));
  lala->getValue().toUTF8String(str);
  CPPUNIT_ASSERT_EQUAL(std::string("lulu"),str);

  srv.deleteDatabase();

  curl_global_cleanup();
}



/* eof */
