/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_MODULE_HPP
#define YADIF_MODULE_HPP

#include "Binder.hpp"
#include "BindingCreator.hpp"
#include "BindingStore.hpp"

#include <vector>

namespace yadif {

  class Module : public detail::BindingCreator
  {
  private:
    virtual void configure() const = 0;

  public:

    void
    createBindings(detail::BindingStore& bindings) const override
    {
      cache_.clear();
      configure();

      for (auto& e : cache_)
      {
        e.binding.finish();
        bindings.add(e.key, e.binding);
      }
    }

  protected:

    template <typename Interface, typename A = Annotation<>>
    detail::Binder<Interface> bind() const
    {
      static_assert(is_annotation<A>::value, "second bind() parameter must be an annotation");

      cache_.emplace_back(detail::key<Interface, A>());
      return detail::Binder<Interface>{cache_.back().binding};
    }

  private:
    struct CachedBinding
    {
      CachedBinding(detail::Key k)
        : key(k)
      {}

      detail::Key key;
      detail::Binding binding;
    };

    mutable std::vector<CachedBinding> cache_;
  };

}

#endif//ndef YADIF_MODULE_HPP
