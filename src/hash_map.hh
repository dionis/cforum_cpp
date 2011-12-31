/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief abstracts the hash map debacle
 * \package hash_map
 *
 * This simply abstracts the hash_map debacle of std::hash_map
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

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <tr1/unordered_map>
#include <unicode/unistr.h>

namespace std {
  using namespace tr1;
}

namespace CForum {
  struct hash_unicodestring {
    std::size_t operator()(const UnicodeString &ustr) const {
      return static_cast<std::size_t>(ustr.hashCode());
    }
  };

}

#include <boost/config.hpp>

#include <boost/serialization/utility.hpp>
#include <boost/serialization/collections_save_imp.hpp>
#include <boost/serialization/collections_load_imp.hpp>
#include <boost/serialization/split_free.hpp>

namespace boost {
  namespace serialization {

    template<class Archive, class Type, class Key, class Hash, class Compare, class Allocator> inline void save(Archive &ar, const std::unordered_map<Key, Type, Hash, Compare, Allocator> &t, const unsigned int v) {
      (void)v;

      boost::serialization::stl::save_collection<Archive, std::unordered_map<Key, Type, Hash, Compare, Allocator> >(ar, t);
    }

    template<class Archive, class Type, class Key, class Hash, class Compare, class Allocator> inline void load(Archive &ar, std::unordered_map<Key, Type, Hash, Compare, Allocator> &t, const unsigned int v) {
      (void)v;

      boost::serialization::stl::load_collection<Archive, std::unordered_map<Key, Type, Hash, Compare, Allocator>, boost::serialization::stl::archive_input_map<Archive, std::unordered_map<Key, Type, Hash, Compare, Allocator> >, boost::serialization::stl::no_reserve_imp<std::unordered_map<Key, Type, Hash, Compare, Allocator> > >(ar, t);
    }

    // split non-intrusive serialization function member into separate
    // non intrusive save/load member functions
    template<class Archive, class Type, class Key, class Hash, class Compare, class Allocator> inline void serialize(Archive &ar, std::unordered_map<Key, Type, Hash, Compare, Allocator> &t, const unsigned int file_version) {
      boost::serialization::split_free(ar, t, file_version);
    }

  }
}


# endif

/* eof */
