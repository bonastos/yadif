/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_BINDING_HPP
#define YADIF_BINDING_HPP

#include "InstanceSource.hpp"
#include "Scope.hpp"
#include "yadif_exception.hpp"

#include <cassert>

namespace yadif {
  namespace detail {
    class Binding;

    class Binding
    {
    public:

      void
      setSource(SourcePtr sourcePtr)
      {
        if (sourcePtr_)
          throw IllegalBinding("binding interface twice");

        sourcePtr_ = std::move(sourcePtr);
      }

      void
      setScope(ScopePtr scopePtr)
      {
        if (scopePtr_)
          throw IllegalBinding("binding scope twice");

        scopePtr_ = std::move(scopePtr);
      }

      void
      setEager(bool value)
      {
        isEager_ = value;
      }

      void
      finish()
      {
        if (!sourcePtr_)
          throw IllegalBinding("interface not bound");

        if (scopePtr_)
        {
          scopePtr_->wrap(std::move(sourcePtr_));
          sourcePtr_ = scopePtr_;
        }
      }

      InstanceSource&
      source()
      {
        assert(sourcePtr_);
        return *sourcePtr_;
      }

      bool
      isEager() const
      {
        return isEager_;
      }

    private:
      SourcePtr sourcePtr_;
      ScopePtr  scopePtr_;
      bool      isEager_ = false;
    };

  }
}

#endif//ndef YADIF_BINDING_HPP
