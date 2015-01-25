/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <catch.hpp>

#define YADIF_EAGER_SINGLETON

#include <yadif.hpp>

using namespace yadif;

namespace {

  struct A {};
  struct B {};
  struct C {};

  template <typename T>
  struct TestProvider
  {
    T* operator()() { ++nCalled; return new T; }

    static unsigned nCalled;
  };

  template <typename T>
  unsigned TestProvider<T>::nCalled = 0;

  using AProvider = TestProvider<A>;
  using BProvider = TestProvider<B>;
  using CProvider = TestProvider<C>;

  struct TestModule : public Module
  {
    void configure() const override
    {
      bind<A>().toProvider(AProvider{}).in(SingletonScope{});
      bind<B>().toProvider(BProvider{}).asSingleton();
      bind<C>().toProvider(CProvider{}).asEagerSingleton();
    }
  };

}

TEST_CASE("SingletonScope with global eager binding")
{
  Injector injector{TestModule{}};
  CHECK ( AProvider::nCalled == 1 );
  CHECK ( BProvider::nCalled == 1 );
  CHECK ( CProvider::nCalled == 1 );

  std::shared_ptr<A> ap1 = injector.get<A>();
  std::shared_ptr<A> ap2 = injector.get<A>();
  std::shared_ptr<A> ap3 = injector.get<A>();
  CHECK ( ap1 == ap2 );
  CHECK ( ap2 == ap3 );
  CHECK ( AProvider::nCalled == 1 );

  std::shared_ptr<B> bp1 = injector.get<B>();
  std::shared_ptr<B> bp2 = injector.get<B>();
  std::shared_ptr<B> bp3 = injector.get<B>();
  CHECK ( bp1 == bp2 );
  CHECK ( bp2 == bp3 );
  CHECK ( BProvider::nCalled == 1 );

  std::shared_ptr<C> cp1 = injector.get<C>();
  std::shared_ptr<C> cp2 = injector.get<C>();
  std::shared_ptr<C> cp3 = injector.get<C>();
  CHECK ( cp1 == cp2 );
  CHECK ( cp2 == cp3 );
  CHECK ( CProvider::nCalled == 1 );
}
