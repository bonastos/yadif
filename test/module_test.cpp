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

namespace  {

  struct A {};

  struct TestModule : public Module
  {
    void configure() const override
    {
      bind<A>().toProvider([] () -> A* { return nullptr; });
    }
};

}

TEST_CASE("class Module test")
{
  BindingStore bindings;
  TestModule m;
  m.createBindings(bindings);

  CHECK_NOTHROW( bindings.source(key<A>()) );
}
