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

  struct Base
  {
    virtual int f() = 0;
  };

  struct Derived : Base
  {
    int f() override { return 42; }
  };

  struct TestModule : Module
  {
    void configure() const override
    {
      bind<Derived>().toProvider([] () -> Derived* { return new Derived; });
      bind<Base>().to<Derived>();
    }
  };

}

TEST_CASE("basic funtionality")
{
  Injector injector{TestModule{}};

  SECTION("bind to provider")
  {
    std::shared_ptr<Derived> dp = injector.get<Derived>();
    CHECK (dp->f() == 42);
  }

  SECTION("bind to Implementation")
  {
    std::shared_ptr<Base> bp = injector.get<Base>();
    CHECK (bp->f() == 42);
  }

  SECTION("check each get() returns new instance")
  {
    std::shared_ptr<Derived> dp1 = injector.get<Derived>();
    std::shared_ptr<Derived> dp2 = injector.get<Derived>();
    std::shared_ptr<Base>    bp1 = injector.get<Base>();
    std::shared_ptr<Base>    bp2 = injector.get<Base>();
    std::shared_ptr<Base>    bp3 = dp1;
    std::shared_ptr<Base>    bp4 = dp2;

    CHECK( dp1 != dp2 );
    CHECK( bp1 != bp2 );
    CHECK( bp1 != bp3 );
    CHECK( bp1 != bp4 );
    CHECK( bp2 != bp3 );
    CHECK( bp2 != bp4 );
    CHECK( bp3 != bp4 );
  }
}
