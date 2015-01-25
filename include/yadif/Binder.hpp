/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_BINDER_HPP
#define YADIF_BINDER_HPP

#include "Binding.hpp"
#include "ConvertingSource.hpp"
#include "ProvidingSource.hpp"
#include "SingletonScope.hpp"

#include <memory>

namespace yadif {
  namespace detail {

    template <typename Interface>
    class Binder {
    public:
      Binder(Binding& binding)
        : binding_{binding}
      {}

      template <typename Implementation, typename A = Annotation<>>
      Binder&
      to()
      {
        static_assert(is_annotation<A>::value, "second to() parameter must be an annotation");
        binding_.setSource(std::make_shared<ConvertingSource<Interface, Implementation, A>>());
        return *this;
      }

      template <typename TypeProvider>
      Binder&
      toProvider(TypeProvider&& provider)
      {
        binding_.setSource(std::make_shared<ProvidingSource<Interface, TypeProvider>>(std::forward<TypeProvider>(provider)));
        return *this;
      }

      template <typename ScopeType>
      Binder&
      in(ScopeType&& scope)
      {
        setScope(std::forward<ScopeType>(scope), EagerBinding<ScopeType>::value);
        return *this;
      }

      Binder&
      asSingleton()
      {
        return in(SingletonScope{});
      }

      Binder&
      asEagerSingleton()
      {
        setScope(SingletonScope{}, true);
        return *this;
      }

    private:

      template <typename ScopeType>
      void
      setScope(ScopeType&& scope, bool eager)
      {
        binding_.setEager(eager);
        binding_.setScope(std::make_shared<ScopeType>(std::forward<ScopeType>(scope)));
      }

      Binding& binding_;
    };
  }
}

#endif//ndef YADIF_BINDER_HPP
