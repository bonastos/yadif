/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_EXCEPTION_HPP
#define YADIF_EXCEPTION_HPP

#include "Key.hpp"

#include <stdexcept>

namespace yadif {

  struct Exception : public std::logic_error
  {
    using std::logic_error::logic_error;
  };

  struct NoBindingFound : public Exception
  {
    NoBindingFound(detail::Key key)
      : Exception("no binding found for " + key.name())
    {}
  };

  struct DuplicateBinding : public Exception
  {
    DuplicateBinding(detail::Key key)
      : Exception("duplicate binding for " + key.name())
    {}
  };

  struct IllegalBinding : public Exception
  {
    IllegalBinding(const std::string& msg)
      : Exception(msg)
    {}
  };
}

#endif//ndef YADIF_EXCEPTION_HPP
