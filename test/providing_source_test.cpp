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
  // --------------------------------------------------------------------
  // providers - functors, functions & lambdas
  // --------------------------------------------------------------------

  struct FunctorD
  {
    int* operator()() {return new int{42}; }
  };

  struct FunctorI
  {
    int* operator()(Injector&) {return new int{42}; }
  };

  FunctorD functorD;
  FunctorI functorI;
  int* functionD() {return new int{42}; }
  int* functionI(yadif::Injector&) {return new int{42}; }
  auto lambdaD = [] () -> int* {return new int{42}; };
  auto lambdaI = [] (yadif::Injector&) -> int* {return new int{42}; };

  // --------------------------------------------------------------------
  // non providers - wrong interface
  // --------------------------------------------------------------------

  struct E {};

  struct FunctorE
  {
    void operator()(int);
  };

  FunctorE functorE;
  int* functionE(int) {return new int{42}; }
  auto lambdaE = [] (int) -> E { return {}; };
  E structE;

  // --------------------------------------------------------------------
  // helper functions that detect type of Provider object
  // --------------------------------------------------------------------

  template <typename Tag, typename F>
  bool
  hasProviderTag(F)
  {
    return std::is_same<Tag, ProviderTag<F>>::value;
  }

  template <typename Result, typename F>
  bool
  hasResultType(F)
  {
    return std::is_same<Result, ProviderResult<F>>::value;
  }

  template <typename T, typename F>
  ProvidingSource<T, F>
  make_providing_source(F&& f)
  {
    return {std::forward<F>(f)};
  }

}

// ----------------------------------------------------------------------
// test cases
// ----------------------------------------------------------------------

TEST_CASE("ProvidingSource Generation")
{
  SECTION("template alias ProviderTag<F>")
  {
    CHECK( hasProviderTag<tag::DefaultCallable>(functorD)  );
    CHECK( hasProviderTag<tag::DefaultCallable>(functionD) );
    CHECK( hasProviderTag<tag::DefaultCallable>(lambdaD)   );

    CHECK( hasProviderTag<tag::CallableWithInjector>(functorI)  );
    CHECK( hasProviderTag<tag::CallableWithInjector>(functionI) );
    CHECK( hasProviderTag<tag::CallableWithInjector>(lambdaI)   );

    CHECK( hasProviderTag<tag::Illegal>(functorE)  );
    CHECK( hasProviderTag<tag::Illegal>(functionE) );
    CHECK( hasProviderTag<tag::Illegal>(lambdaE)   );
    CHECK( hasProviderTag<tag::Illegal>(structE)   );
  }

  SECTION("template alias ProviderResult<F>")
  {
    CHECK(( hasResultType<int*>(functorD) ));
    CHECK(( hasResultType<std::shared_ptr<int>>([]() -> std::shared_ptr<int> { return {}; }) ));
    CHECK(( hasResultType<std::unique_ptr<int>>([]() -> std::unique_ptr<int> { return {}; }) ));
  }

  SECTION("check that ProvidingSource instantiation works")
  {
    auto p1 = make_providing_source<int>(functorD);
    auto p2 = make_providing_source<int>(functorI);
    auto p3 = make_providing_source<int>(functionD);
    auto p4 = make_providing_source<int>(functionI);
    auto p5 = make_providing_source<int>(lambdaD);
    auto p6 = make_providing_source<int>(lambdaI);
    auto p7 = make_providing_source<int>([] () -> int* { return nullptr; });
    auto p8 = make_providing_source<int>(FunctorD{});
    auto p9 = make_providing_source<int>(FunctorI{});
  }
}
