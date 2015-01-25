/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <catch.hpp>

#include <yadif.hpp>

#include <memory>

using namespace yadif;
using namespace yadif::detail;

namespace {

  struct Test1 {};
  struct Test2 {};
  
  struct TestSource : public InstanceSource
  {
    InstancePtr
    get(Injector&) override
    {
      called = true;
      return {};
    }

    bool called = false;
  };

}

TEST_CASE("Bindings container")
{
  BindingStore bindings;

  Key key1 = key<Test1>();
  Key key2 = key<Test2>();
  
  SourcePtr sourcePtr1 = std::make_shared<TestSource>();
  SourcePtr sourcePtr2 = std::make_shared<TestSource>();
  
  Binding binding1;
  binding1.setSource(sourcePtr1);

  Binding binding2;
  binding2.setSource(sourcePtr2);
  binding2.setEager(true);

  SECTION("add provider")
  {
    bindings.add(key1, binding1);
    bindings.add(key2, binding2);

    CHECK( &bindings.source(key1) == sourcePtr1.get() );
    CHECK( &bindings.source(key2) == sourcePtr2.get() );
  }

  SECTION("bindEagerSources")
  {
    bindings.add(key1, binding1);
    bindings.add(key2, binding2);

    Injector injector;
    bindings.bindEagerSources(injector);

    CHECK( std::dynamic_pointer_cast<TestSource>(sourcePtr1)->called == false );
    CHECK( std::dynamic_pointer_cast<TestSource>(sourcePtr2)->called == true  );
  }

  SECTION("add duplicate provider")
  {
    bindings.add(key1, binding1);

    CHECK_THROWS_AS( bindings.add(key1, binding2), DuplicateBinding );
  }

  SECTION("get nonexisting binding")
  {
    CHECK_THROWS_AS( bindings.source(key1), NoBindingFound );
  }
}
