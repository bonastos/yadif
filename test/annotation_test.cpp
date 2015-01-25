/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <catch.hpp>

#include <yadif.hpp>

using namespace yadif;

namespace {

  struct A {};

}

TEST_CASE("is_annotation<> predicate")
{
  CHECK(  is_annotation<Annotation<>>::value );
  CHECK(  is_annotation<Annotation<A>>::value );
  CHECK( !is_annotation<A>::value );
}
