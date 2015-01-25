/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <catch.hpp>

#include <yadif.hpp>

using namespace yadif;

// --------------------------------------------------------

TEST_CASE("request non existing binding")
{
  Injector injector;

  CHECK_THROWS_AS( injector.get<int>(), NoBindingFound );
}

// --------------------------------------------------------

namespace {

  struct A {};
  struct B {};
  struct C {};
  struct D {};

  struct ModuleA : Module
  {
    void configure() const override { bind<A>().toProvider([]() -> A* { return nullptr; }); wasCalled = true; }

    static bool wasCalled;
  };

  bool ModuleA::wasCalled = false;

  struct ModuleB : Module
  {
    void configure() const override { bind<B>().toProvider([]() -> B* { return nullptr; }); }
  };

  struct ModuleC : Module
  {
    void configure() const override { bind<C>().toProvider([]() -> C* { return nullptr; }); }
  };

  struct ModuleD : Module
  {
    void configure() const override { bind<D>().toProvider([]() -> D* { return nullptr; }); }
  };

}

TEST_CASE("injector construction")
{
  SECTION("construct from modules")
  {
    Injector injector{ModuleA{}, ModuleB{}};

    CHECK( ModuleA::wasCalled );

    CHECK_NOTHROW( injector.get<A>() );
    CHECK_NOTHROW( injector.get<B>() );
  }

  SECTION("construct from existing injector & modules")
  {
    Injector injector0{ModuleA{}, ModuleB{}};

    Injector injector{injector0, ModuleC{}, ModuleD{}};

    CHECK_NOTHROW( injector.get<A>() );
    CHECK_NOTHROW( injector.get<B>() );
    CHECK_NOTHROW( injector.get<C>() );
    CHECK_NOTHROW( injector.get<D>() );
  }
}
