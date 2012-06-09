/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CForum wrapper binary
 *
 * CForum wrapper binary, just initializing and dispatching to the right controllers
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

#include "cforum.hh"

typedef union _sa_t {
  struct sockaddr    sa;
  struct sockaddr_un sa_unix;
  struct sockaddr_in sa_inet;
} sa_t;

int main(int argc, char *argv[]) {
  sa_t sa;
  socklen_t len = sizeof(sa);

  /* if we can get the peer name on stdout (fd 0) or the error is not
   * „not a connection“ (ENOTCON), then we are in FastCGI mode. Shamelessly
   * stolen from PHP ;-)
   */
  errno = 0;
  if(getpeername(0, (struct sockaddr *)&sa, &len) != 0 && errno == ENOTCONN) {
    // fastcgi
  }
  else {
    // not fastcgi
    try {
      CForum::CGIApplication app;
      app.init(argc, argv);
      app.handleRequest();
    }
    catch(CForum::NotFoundException &e) {
      std::cout << "Status: 404 Not Found\015\012Content-Type: text/plain; charset=utf-8\015\012\015\012";
      std::cout << "ERROR: " << e.getMessage() << " (" << std::hex << e.getCode() << std::dec << ")" << std::endl;
    }
    catch(CForum::CForumException &e) {
      std::cout << "Status: 500 Internal Server Error\015\012Content-Type: text/plain; charset=utf-8\015\012\015\012";
      std::cout << "ERROR: " << e.getMessage() << " (" << std::hex << e.getCode() << std::dec << ")" << std::endl;
    }
  }

  return EXIT_SUCCESS;
}

/* eof */
