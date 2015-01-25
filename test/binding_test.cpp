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

  struct TestSource : InstanceSource
  {
    InstancePtr get(Injector&) override { return {}; }
  };

  struct TestScope : Scope
  {
    InstancePtr get(Injector& injector) override { return sourcePtr_->get(injector); }
    void wrap(SourcePtr sourcePtr) override { sourcePtr_ = sourcePtr; }

    SourcePtr sourcePtr_;
  };
}

TEST_CASE("Binding class")
{
  SourcePtr sourcePtr = std::make_shared<TestSource>();
  ScopePtr  scopePtr  = std::make_shared<TestScope>();

  Binding binding;

  SECTION("bind to source")
  {
    binding.setSource(sourcePtr);
    binding.finish();

    CHECK( &binding.source() == sourcePtr.get() );
  }

  SECTION("bind to scoped source")
  {
    binding.setSource(sourcePtr);
    binding.setScope(scopePtr);
    binding.finish();

    CHECK( &binding.source() == scopePtr.get() );
    CHECK( dynamic_cast<TestScope&>(binding.source()).sourcePtr_ == sourcePtr );
  }


  SECTION("eager binding")
  {
    CHECK( binding.isEager() == false );

    binding.setEager(true);

    CHECK( binding.isEager() == true );
  }


  SECTION("empty binding throws")
  {
    CHECK_THROWS_AS( binding.finish(), IllegalBinding );
  }

  SECTION("binding twice throws")
  {
    CHECK_NOTHROW(   binding.setSource(sourcePtr)              );
    CHECK_THROWS_AS( binding.setSource(sourcePtr), IllegalBinding );
  }

  SECTION("scoping twice throws")
  {
    CHECK_NOTHROW(   binding.setScope(scopePtr)              );
    CHECK_THROWS_AS( binding.setScope(scopePtr), IllegalBinding );
  }
}
