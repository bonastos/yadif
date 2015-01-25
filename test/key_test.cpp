/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <catch.hpp>

#include <yadif.hpp>

using namespace yadif;
using namespace yadif::detail;

namespace {

  struct S {};
  struct A {};

  struct X;
}

TEST_CASE("class Key test")
{
  SECTION("Comparison")
  {
    auto key1 = key<S>();
    auto key2 = key<S, Annotation<A>>();

    CHECK( ((key1 < key2) || (key2 < key1)) );
  }

  SECTION("create key from incomplete type")
  {
    auto key1 = key<X>();
    auto key2 = key<X, Annotation<A>>();

    CHECK( ((key1 < key2) || (key2 < key1)) );
  }

  SECTION("Copying/equivalence")
  {
    auto key1 = key<S, Annotation<A>>();
    auto key2 = key1;

    CHECK( (!(key1 < key2) && !(key2 < key1)) );
  }
}
