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
  // --------------------------------------------------------------------
  // providers - functors, functions & lambdas
  // --------------------------------------------------------------------

  struct FunctorD
  {
    int* operator()();
  };

  struct FunctorI
  {
    int* operator()(Injector&);
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
  // types for IsConvertible test
  // --------------------------------------------------------------------

  struct Base {};
  struct Derived : public Base {};

  // --------------------------------------------------------------------
  // helper function that detect type of provider object
  // --------------------------------------------------------------------

//  template <typename F>
//  bool
//  isDefaultCallable(F)
//  {
//    return IsDefaultCallable<F>();
//  }
//
//  template <typename F>
//  bool
//  isCallableWithInjector(F)
//  {
//    return IsCallableWithInjector<F>();
//  }
//
  template <typename F>
  bool
  isDefaultCallable(F)
  {
    return is_default_callable<F>::value;
  }

  template <typename F>
  bool
  isCallableWithInjector(F)
  {
    return is_callable_with_injector<F>::value;
  }

}

// ----------------------------------------------------------------------
// test cases
// ----------------------------------------------------------------------

TEST_CASE("type aliases")
{
  CHECK(( std::is_same<int*, result_of_t<FunctorD()>>::value          ));
  CHECK(( std::is_same<int*, result_of_t<FunctorI(Injector&)>>::value ));

  CHECK(( std::is_same<FunctorD, conditional_t<true,  FunctorD, FunctorI>>::value ));
  CHECK(( std::is_same<FunctorI, conditional_t<false, FunctorD, FunctorI>>::value ));
}

TEST_CASE("provider interface predicates")
{
  SECTION("test is_default_callable<> template")
  {
    CHECK( isDefaultCallable(functorD)   );
    CHECK( isDefaultCallable(functionD)  );
    CHECK( isDefaultCallable(lambdaD)    );

    CHECK( !isDefaultCallable(functorI)  );
    CHECK( !isDefaultCallable(functionI) );
    CHECK( !isDefaultCallable(lambdaI)   );

    CHECK( !isDefaultCallable(functorE)  );
    CHECK( !isDefaultCallable(functionE) );
    CHECK( !isDefaultCallable(lambdaE)   );
    CHECK( !isDefaultCallable(structE)   );
  }

  SECTION("test iscallable_with_injector<> template")
  {
    CHECK( isCallableWithInjector(functorI)  );
    CHECK( isCallableWithInjector(functionI) );
    CHECK( isCallableWithInjector(lambdaI)   );

    CHECK( !isCallableWithInjector(functorD)  );
    CHECK( !isCallableWithInjector(functionD) );
    CHECK( !isCallableWithInjector(lambdaD)   );

    CHECK( !isCallableWithInjector(functorE)  );
    CHECK( !isCallableWithInjector(functionE) );
    CHECK( !isCallableWithInjector(lambdaE)   );
    CHECK( !isCallableWithInjector(structE)   );
  }
}
