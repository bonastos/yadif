/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <catch.hpp>

#include <yadif.hpp>

#include <map>

using namespace yadif;

namespace {


  struct DestructionRegistry
  {
    void addPtr(void* p) {counters[p] = next++; }

    unsigned destructionCounter(void* p) const { return counters.at(p); }

    std::map<void*, unsigned> counters;
    unsigned next = 1;
  };

  DestructionRegistry registry;


  struct A
  {
    ~A() { registry.addPtr(this); }
  };

  struct B
  {
    ~B() { registry.addPtr(this); }
  };

  struct C
  {
    ~C() { registry.addPtr(this); }
  };

  struct D
  {
    D(A& a, B* bp, std::shared_ptr<C> cp) : a_{a}, bp_{bp}, cp_{cp} {}

    ~D() { registry.addPtr(this); }

    A& a_;
    B* bp_;
    std::shared_ptr<C> cp_;
  };

  using AProvider = Provider<A>;
  using BProvider = Provider<B>;
  using CProvider = Provider<C>;
  using DProvider = Provider<D, Ref<A>, Ptr<B>, SharedPtr<C>>;

  struct TestModule : yadif::Module
  {
    void configure() const override
    {
      bind<A>().toProvider(AProvider{});
      bind<B>().toProvider(BProvider{});
      bind<C>().toProvider(CProvider{});
      bind<D>().toProvider(DProvider{});
    }
  };

}

TEST_CASE("Provider template")
{
  Injector injector{TestModule{}};

  A* ap = nullptr;
  B* bp = nullptr;
  C* cp = nullptr;
  D* dp = nullptr;

  {
    std::shared_ptr<D> spd = injector.get<D>();

    dp = spd.get();
    ap = &dp->a_;
    bp = dp->bp_;
    cp = dp->cp_.get();
  }

  CHECK( registry.destructionCounter(dp) < registry.destructionCounter(ap) );
  CHECK( registry.destructionCounter(dp) < registry.destructionCounter(bp) );
  CHECK( registry.destructionCounter(dp) < registry.destructionCounter(cp) );

}
