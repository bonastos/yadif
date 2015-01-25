/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_SINGLETON_SCOPE_HPP
#define YADIF_SINGLETON_SCOPE_HPP

#include "Scope.hpp"
#include "type_traits.hpp"

namespace yadif {

  struct SingletonScope;

  namespace detail {

    template <typename T>
    struct EagerBinding
    {
      static constexpr bool value = std::is_same<T, SingletonScope>::value ?
#ifdef YADIF_EAGER_SINGLETON
        true
#else
        false
#endif
        : false;
    };

  }

  struct SingletonScope : public Scope
  {
    void
    wrap(SourcePtr sourcePtr) override
    {
      sourcePtr_ = sourcePtr;
    }

    InstancePtr
    get(Injector& injector) override
    {
      if (!instancePtr_)
        instancePtr_ = sourcePtr_->get(injector);

      return instancePtr_;
    }

    SourcePtr   sourcePtr_;
    InstancePtr instancePtr_;
  };

}

#endif//ndef YADIF_SINGLETON_SCOPE_HPP
