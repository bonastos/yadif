/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_PROVIDING_SOURCE_HPP
#define YADIF_PROVIDING_SOURCE_HPP

#include "Injector.hpp"
#include "InstanceSource.hpp"
#include "type_traits.hpp"

#include <memory>

namespace yadif {
  namespace detail {

    // --------------------------------------------------------------------------
    // helper template alias ProviderTag<F>
    // --------------------------------------------------------------------------

    namespace tag {
      struct DefaultCallable {};
      struct CallableWithInjector {};
      struct Illegal {};
    }

    template <typename F>
    using ProviderTag = conditional_t<is_default_callable<F>::value,
                                      tag::DefaultCallable,
                                      conditional_t<is_callable_with_injector<F>::value,
                                                    tag::CallableWithInjector,
                                                    tag::Illegal>>;
    // --------------------------------------------------------------------------
    // helper template alias ProviderResult<F>
    // --------------------------------------------------------------------------

    template <typename F, typename Tag>
    struct ProviderResultImpl
    {};

    template <typename F>
    struct ProviderResultImpl<F, tag::DefaultCallable>
    {
      using type = result_of_t<F()>;
    };

    template <typename F>
    struct ProviderResultImpl<F, tag::CallableWithInjector>
    {
      using type = result_of_t<F(Injector&)>;
    };

    template <typename F>
    using ProviderResult = typename ProviderResultImpl<F, ProviderTag<F>>::type;

    // --------------------------------------------------------------------------
    // template ProvidingSource<F>
    // --------------------------------------------------------------------------

    template <typename Interface, typename F>
    struct ProvidingSource : public InstanceSource
    {
      using SharedPtr = std::shared_ptr<Interface>;

      ProvidingSource(F&& f)
        : f_(std::forward<F>(f))
      {
        static_assert(!std::is_same<ProviderTag<F>, tag::Illegal>::value, "provider not callable");
        static_assert(std::is_constructible<std::shared_ptr<Interface>, ProviderResult<F>>::value,
                      "illegal provider return type");
      }

      template <typename FF>
      SharedPtr
      getInstance(FF f, Injector& injector, tag::DefaultCallable)
      {
        return SharedPtr{f()};
      }

      template <typename FF>
      SharedPtr
      getInstance(FF f, Injector& injector, tag::CallableWithInjector)
      {
        return SharedPtr{f(injector)};
      }

      InstancePtr
      get(Injector& injector) override
      {
        return getInstance(f_, injector, ProviderTag<F>{});
      }

      F f_;
  };

  }
}

#endif//ndef YADIF_PROVIDING_SOURCE_HPP
