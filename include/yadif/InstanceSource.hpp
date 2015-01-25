/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_INSTANCE_SOURCE_HPP
#define YADIF_INSTANCE_SOURCE_HPP

#include <memory>

namespace yadif {

  class Injector;
  
  namespace detail {

    using InstancePtr = std::shared_ptr<void>;
  
    struct InstanceSource
    {
      virtual InstancePtr get(Injector& injector) = 0;

      virtual ~InstanceSource() = default;
    };

    using SourcePtr = std::shared_ptr<InstanceSource>;
  }
}

#endif//ndef YADIF_INSTANCE_SOURCE_HPP
