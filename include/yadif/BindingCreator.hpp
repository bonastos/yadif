/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_BINDING_CREATOR_HPP
#define YADIF_BINDING_CREATOR_HPP

namespace yadif {
  namespace detail {
    class BindingStore;

    struct BindingCreator
    {
      virtual void createBindings(BindingStore& bindings) const = 0;
    };

  }
}

#endif//ndef YADIF_BINDING_CREATOR_HPP
