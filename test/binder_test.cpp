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

  struct Ifc {};
  struct Impl : public Ifc {};
  
  struct Test{};
  
  auto testProvider = [] () -> Impl* { return nullptr; };

  struct TestScope : Scope
  {
    InstancePtr get(Injector& injector) override { return sourcePtr_->get(injector); }
    void wrap(SourcePtr sourcePtr) override { sourcePtr_ = sourcePtr; }

    SourcePtr sourcePtr_;
  };
}

TEST_CASE("basic Binder syntax")
{
  Binding binding;

  SECTION("bind to implementation")
  {
    {
      Binder<Ifc>{binding}.to<Impl>();
    }
    CHECK_NOTHROW(binding.finish());
    CHECK( binding.isEager() == false );
  }

  SECTION("bind to implementation with annotation")
  {
    {
      Binder<Ifc>{binding}.to<Impl, Annotation<Test>>();
    }
    CHECK_NOTHROW(binding.finish());
    CHECK( binding.isEager() == false );
  }

  SECTION("bind to provider")
  {
    {
      Binder<Ifc>{binding}.toProvider(testProvider);
    }
    CHECK_NOTHROW(binding.finish());
    CHECK( binding.isEager() == false );
  }

  SECTION("bind in generic scope")
  {
    {
      Binder<Ifc>{binding}.to<Impl>().in(TestScope{});
    }
    CHECK_NOTHROW(binding.finish());
    CHECK( binding.isEager() == false );
  }
  
  SECTION("bind in SingletonScope")
  {
    {
      Binder<Ifc>{binding}.to<Impl>().in(SingletonScope{});
    }
    CHECK_NOTHROW(binding.finish());
    CHECK( binding.isEager() == false );
  }
  
  SECTION("bind as singleton")
  {
    {
      Binder<Ifc>{binding}.to<Impl>().asSingleton();
    }
    CHECK_NOTHROW(binding.finish());
    CHECK( binding.isEager() == false );
  }

  SECTION("bind as eager singleton")
  {
    {
      Binder<Ifc>{binding}.to<Impl>().asEagerSingleton();
    }
    CHECK_NOTHROW(binding.finish());
    CHECK( binding.isEager() == true );
  }
}
