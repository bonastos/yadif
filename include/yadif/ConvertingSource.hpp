/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_CONVERTING_SOURCE_HPP
#define YADIF_CONVERTING_SOURCE_HPP

#include "InstanceSource.hpp"
#include "Injector.hpp"
#include "type_traits.hpp"

#include <memory>

namespace yadif {
  namespace detail {

    template<typename Interface, typename Implementation, typename A>
    struct ConvertingSource : public InstanceSource
    {
      static_assert(std::is_convertible<Implementation*, Interface*>::value, "illegal binding");

      InstancePtr get(Injector& injector_) override
      {
        return std::shared_ptr<Interface>{injector_.get<Implementation, A>()};
      }
    };

  }
}

#endif//ndef YADIF_CONVERTING_SOURCE_HPP
