/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_TYPE_TRAITS_HPP
#define YADIF_TYPE_TRAITS_HPP

#include <type_traits>

namespace yadif {
  class Injector;

  namespace detail {

    // --------------------------------------------------------------------------
    // Some type alias for std::traits (Actually available in C++14)
    // --------------------------------------------------------------------------

    template <typename T>
    using result_of_t = typename std::result_of<T>::type;

    template <bool Condition, typename TrueType, typename FalseType>
    using conditional_t = typename std::conditional<Condition, TrueType, FalseType>::type;

    // --------------------------------------------------------------------------
    // Nifty trick taken from n3911
    // --------------------------------------------------------------------------

    template< class... > struct voider { using type = void; };
    template< class... Ts> using void_t = typename voider<Ts...>::type;

    // --------------------------------------------------------------------------
    // Predicates that detect if a provider is callable with a specific interface
    // --------------------------------------------------------------------------

    template <typename F, typename Check = void>
    struct is_default_callable : std::false_type {};

    template <typename F>
    struct is_default_callable<F, void_t<result_of_t<F()>>> : std::true_type {};


    template <typename F, typename  = void>
    struct is_callable_with_injector : std::false_type {};

    template <typename F>
    struct is_callable_with_injector<F, void_t<result_of_t<F(Injector&)>>> : std::true_type {};
  }
}

#endif//ndef YADIF_TYPE_TRAITS_HPP
