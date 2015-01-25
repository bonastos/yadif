/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_BINDING_STORE_HPP
#define YADIF_BINDING_STORE_HPP

#include "Binding.hpp"
#include "Key.hpp"
#include "yadif_exception.hpp"

#include <map>

namespace yadif {
  namespace detail {

    class BindingStore
    {
    public:

      void
      add(Key key, Binding binding)
      {
        if (bindings_.count(key) != 0)
          throw DuplicateBinding(key);

        bindings_.insert(std::make_pair(key, binding));
      }

      void
      bindEagerSources(Injector& injector)
      {
        for (auto& e : bindings_)
        {
          auto& binding = e.second;

          if (binding.isEager())
            binding.source().get(injector);
        } 
      }

      InstanceSource&
      source(Key key)
      {
        if (bindings_.count(key) == 0)
          throw NoBindingFound(key);

        return bindings_.at(key).source();
      }

    private:
      std::map<Key, Binding> bindings_;
    };

  }
}

#endif//ndef YADIF_BINDING_STORE_HPP
