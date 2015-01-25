/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_PROVIDER_HPP
#define YADIF_PROVIDER_HPP

#include "Annotation.hpp"
#include "Injector.hpp"
#include "index_sequence.hpp"
#include "type_traits.hpp"

#include <memory>
#include <tuple>
#include <utility>

namespace yadif {
  namespace detail {

    // -------------------------------------------------------------------
    // tie lifetime of injected objects by embedding them in deleter
    // -------------------------------------------------------------------

    template <typename T, typename Others>
    class TieDeleter {
    public:
      TieDeleter(Others& others) : others_(others) {}

      void operator () (T* p) const { delete p; }
    private:
      Others others_;
    };

  }

  // ---------------------------------------------------------------------
  // helper templates representing argument bindings
  // ---------------------------------------------------------------------

  template<typename T, typename A = Annotation<>>
  struct Ref
  {
    static_assert(is_annotation<A>::value, "second Ref parameter must be an annotation");

    using ValueType      = T;
    using AnnotationType = A;
    using ParamType      = T&;

    static ParamType transform(std::shared_ptr<T>& sp) { return *sp; }
  };

  template<typename T, typename A = Annotation<>>
  struct Ptr
  {
    static_assert(is_annotation<A>::value, "second Ptr parameter must be an annotation");

    using ValueType      = T;
    using AnnotationType = A;
    using ParamType      = T*;

    static ParamType transform(std::shared_ptr<T>& sp) { return sp.get(); }
  };

  template<typename T, typename A = Annotation<>>
  struct SharedPtr
  {
    static_assert(is_annotation<A>::value, "second SharedPtr parameter must be an annotation");

    using ValueType      = T;
    using AnnotationType = A;
    using ParamType      = std::shared_ptr<T>;

    static ParamType transform(std::shared_ptr<T>& sp) { return sp; }
  };

  // ---------------------------------------------------------------------
  // provider generator template
  // ---------------------------------------------------------------------

  template <typename T, typename... Args>
  struct Provider
  {
    using Instances = std::tuple<std::shared_ptr<typename Args::ValueType>...>;
    using Parameters = std::tuple<typename Args::ParamType...>;
    using Deleter = detail::TieDeleter<T, Instances>;
    using Indices = detail::make_index_sequence<sizeof...(Args)>;

    static_assert(std::is_constructible<T, typename Args::ParamType...>::value, "no matching ctor found");

    Instances
    getInstances(Injector& injector)
    {
      return Instances(injector.get<typename Args::ValueType, typename Args::AnnotationType>()...);
    }

    template <size_t... I>
    Parameters
    getParametersImpl(Instances& instances, detail::index_sequence<I...>)
    {
      return Parameters(Args::transform(std::get<I>(instances))...);
    }

    Parameters
    getParameters(Instances& instances)
    {
      return getParametersImpl(instances, Indices{});
    }

    template <size_t... I>
    T*
    constructImpl(const Parameters& p, detail::index_sequence<I...>)
    {
      return new T(std::get<I>(p)...);
    }

    T*
    construct(const Parameters& p)
    {
      return constructImpl(p, Indices{});
    }

    std::shared_ptr<T>
    operator() (Injector& injector)
    {
      auto instances = getInstances(injector);
      return std::shared_ptr<T>(construct(getParameters(instances)),
                                Deleter(instances));
    }
  };

}

#endif//ndef YADIF_PROVIDER_HPP
