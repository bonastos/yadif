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

  struct Test{
    Test(bool& dtorCalled)
      : dtorCalled_{dtorCalled}
    {}

    ~Test()
    {
      dtorCalled_ = true;
    }
    
    bool& dtorCalled_;
  };

}

TEST_CASE("ReferenceProvider")
{
  bool dtorCalled = false;
  
  {
    Test test{dtorCalled};
    
    ReferenceProvider<Test> provider{test};

    CHECK( provider().get() == &test );
    CHECK( dtorCalled == false );
  }
  
  CHECK(  dtorCalled == true );
}

TEST_CASE("makeReferenceProvider()")
{
  bool dtorCalled = false;
  
  {
    Test test{dtorCalled};
    
    ReferenceProvider<Test> provider = makeReferenceProvider(test);
    
    CHECK( provider().get() == &test );
    CHECK( dtorCalled == false );
  }
  
  CHECK(  dtorCalled == true );
}
