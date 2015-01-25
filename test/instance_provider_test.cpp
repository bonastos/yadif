/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <catch.hpp>

#include <yadif.hpp>

using namespace yadif;

TEST_CASE("InstanceProvider")
{
  InstanceProvider<int> provider{42};
  
  CHECK( *provider() == 42);
}

TEST_CASE("makeInstanceProvider()")
{
  InstanceProvider<int> provider = makeInstanceProvider(42);
    
  CHECK( *provider() == 42);
}
