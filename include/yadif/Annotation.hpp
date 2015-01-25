/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_ANNOTATION_HPP
#define YADIF_ANNOTATION_HPP

namespace yadif {

  namespace detail {
    struct None {};
  }

  template<typename T = detail::None>
  struct Annotation
  {
    using type = T;
  };

  template<typename T>
  struct is_annotation : std::false_type {};

  template<typename T>
  struct is_annotation<Annotation<T>> : std::true_type {};
}

#endif//ndef YADIF_ANNOTATION_HPP
