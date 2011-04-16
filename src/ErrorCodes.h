/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief List of all error codes
 * \package JSON
 *
 * This is a list of all available error codes
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

#ifndef ERRORCODES_H
#define ERRORCODES_H

namespace CForum {
  enum ErrorCode {
    ErrorCodeSuccess = 0,

    /* General */
    ErrorCodeFileNotFound = 0x4d8dfca0,

    /* JS */
    ErrorCodeJSException = 0x4d8dfcae,

    /* JSON */
    ErrorCodeJSONNoParseEnd               = 0x4d8dfcb5,
    ErrorCodeJSONCommentNotEnded          = 0x4d8dfcbc,
    ErrorCodeJSONStringNotTerminated      = 0x4d8dfcc2,
    ErrorCodeJSONObjectKeyMustBeString    = 0x4d8dfcc6,
    ErrorCodeJSONObjectColonMustFollowKey = 0x4d8dfccc,
    ErrorCodeJSONObjectCommaOrEOOMissing  = 0x4d8dfcd2,
    ErrorCodeJSONFloatNumberError         = 0x4d986b4c,
    ErrorCodeArraySyntaxError             = 0x4d986b70,
    ErrorCodeJSONUnknownTokenType         = 0x4d8e06a1,

    /* CGI */
    ErrorCodeCGIUnknownRealm              = 0x4da961f0,
    ErrorCodeCGIInvalidCGIEnvironment     = 0x4da96289,
    ErrorCodeCGINoCookiesGiven            = 0x4da962d0,
    ErrorCodeCGINoDecodeValueGiven        = 0x4da96346,

    /* CouchDB */
    ErrorCodeCouchDBValueNotFound         = 0x4da963fb,
    ErrorCodeCouchDBHttpError             = 0x4da993c6,
    ErrorCodeCouchDBHttpStatusError       = 0x4da99413

  };
}

#endif

/* eof */
