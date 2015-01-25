/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_INSTANCE_PROVIDER_HPP
#define YADIF_INSTANCE_PROVIDER_HPP

#include <memory>

namespace yadif {

  template <typename T>
  class InstanceProvider
  {
  public:
    InstanceProvider(T val)
      : instancePtr_{std::make_shared<T>(val)}
    {}

    std::shared_ptr<T>
    operator() ()
    {
      return instancePtr_;
    }
    
  private:
    std::shared_ptr<T> instancePtr_;
  };

  template <typename T>
  InstanceProvider<T>
  makeInstanceProvider(T val)
  {
    return InstanceProvider<T>(std::move(val));
  }
}

#endif//ndef YADIF_INSTANCE_PROVIDER_HPP
