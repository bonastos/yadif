/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_SCOPE_HPP
#define YADIF_SCOPE_HPP

#include "InstanceSource.hpp"

namespace yadif {

  struct Scope : public detail::InstanceSource
  {
    using SourcePtr   = detail::SourcePtr;
    using InstancePtr = detail::InstancePtr;
    
    virtual void wrap(SourcePtr sourcePtr) = 0;
  };
  
  using ScopePtr = std::shared_ptr<Scope>;
}

#endif//ndef YADIF_SCOPE_HPP
