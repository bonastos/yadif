/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <catch.hpp>

#include <yadif.hpp>

using namespace yadif::detail;

namespace {

  struct A {};

}

TEST_CASE("template index sequence")
{
  using S0 = index_sequence<>;
  using S1 = index_sequence<0>;
  using S2 = index_sequence<0,1>;
  using S3 = index_sequence<0,1,2>;
  using S4 = index_sequence<0,1,2,3>;

  SECTION("generate from size")
  {
    CHECK(( std::is_same< S0, make_index_sequence<0> >::value ));
    CHECK(( std::is_same< S1, make_index_sequence<1> >::value ));
    CHECK(( std::is_same< S2, make_index_sequence<2> >::value ));
    CHECK(( std::is_same< S3, make_index_sequence<3> >::value ));
    CHECK(( std::is_same< S4, make_index_sequence<4> >::value ));
  }

  SECTION("size() member function")
  {
    CHECK( S0::size() == 0 );
    CHECK( S1::size() == 1 );
    CHECK( S2::size() == 2 );
    CHECK( S3::size() == 3 );
    CHECK( S4::size() == 4 );
  }
}
