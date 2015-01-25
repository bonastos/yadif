/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_INJECTOR_HPP
#define YADIF_INJECTOR_HPP

#include "BindingCreator.hpp"
#include "BindingStore.hpp"
#include "Key.hpp"

namespace yadif {

  class Injector {
  public:
    template <typename... Modules>
    Injector(const Modules&... modules)
    {
      createBindings(modules...);
      bindings_.bindEagerSources(*this);
    }

    template <typename... Modules>
    Injector(const Injector& other, Modules... modules)
      : bindings_{other.bindings_}
    {
      createBindings(modules...);
      bindings_.bindEagerSources(*this);
    }

    template<typename T, typename A = Annotation<>>
    std::shared_ptr<T>
    get()
    {
      auto key = detail::key<T, A>();
      return std::static_pointer_cast<T>(bindings_.source(key).get(*this));
    }

  private:
    template <typename Module, typename... Modules>
    void
    createBindings(const Module& module, const Modules&... modules)
    {
      module.createBindings(bindings_);
      createBindings(modules...);
    }

    void
    createBindings()
    {}

  public:
    detail::BindingStore bindings_;
  };

}

#endif//ndef YADIF_INJECTOR_HPP
