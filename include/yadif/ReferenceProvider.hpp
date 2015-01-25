/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_REFERENCE_PROVIDER_HPP
#define YADIF_REFERENCE_PROVIDER_HPP

#include <memory>

namespace yadif {

  namespace detail {
    struct NoDeleter
    {
      void operator () (void* p) const {}
    };
  }
  
  template <typename T>
  class ReferenceProvider
  {
  public:
    
    ReferenceProvider(T& ref)
      : instancePtr_{&ref, detail::NoDeleter{}}
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
  ReferenceProvider<T>
  makeReferenceProvider(T& ref)
  {
    return ReferenceProvider<T>(ref);
  }

}

#endif//ndef YADIF_REFERENCE_PROVIDER_HPP
