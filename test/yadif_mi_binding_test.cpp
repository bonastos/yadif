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

  struct Base1
  {
    virtual int f1() = 0;
  };

  struct Base2
  {
    virtual int f2() = 0;
  };

  struct Base3
  {
    virtual int f3() = 0;
  };

  struct Derived : Base1, Base2, Base3
  {
    int f1() override { return 41; }
    int f2() override { return 42; }
    int f3() override { return 43; }
  };

  struct TestModule : public Module
  {
    void configure() const override
    {
      bind<Derived>().toProvider([] () -> Derived* { return new Derived; });
      bind<Base1>().to<Derived>();
      bind<Base2>().to<Derived>();
      bind<Base3>().to<Derived>();
    }
  };

}

TEST_CASE("multiple inheritance adjustments")
{
  Injector injector{TestModule{}};

  SECTION("check instance pointer gets adjusted")
  {
    std::shared_ptr<Base1> bp1 = injector.get<Base1>();
    std::shared_ptr<Base2> bp2 = injector.get<Base2>();
    std::shared_ptr<Base3> bp3 = injector.get<Base3>();
    CHECK ( bp1->f1() == 41 );
    CHECK ( bp2->f2() == 42 );
    CHECK ( bp3->f3() == 43 );
  }
}
