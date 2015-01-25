/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef	YADIF_KEY_HPP
#define	YADIF_KEY_HPP

#include "Annotation.hpp"

#include <typeindex>
#include <string>
#include <utility>

namespace yadif {
  namespace detail {

    template <typename T>
    std::type_index
    typeIndex()
    {
      return std::type_index(typeid(T));
    }

    class Key
    {
    public:
      Key(std::type_index typeId, std::type_index annotation)
        : key_{typeId, annotation}
      {}

      bool
      operator<(const Key& rhs) const
      {
        return key_ < rhs.key_;
      }

      std::string
      name() const
      {
        auto n = std::string("<") + key_.first.name();
        if (key_.second != typeIndex<Annotation<>>())
          n += std::string(", ") + key_.second.name();
        n += ">";
        return n;
      }

    private:
      // first contains typId, second annotation.
      std::pair<std::type_index, std::type_index> key_;
    };

    template <typename T>
    struct TypeWrap {};

    template <typename T, typename A = Annotation<>>
    Key key()
    {
      static_assert(is_annotation<A>::value, "second key() parameter must be an annotation");
      return Key(typeIndex<TypeWrap<T>>(), typeIndex<A>());
    }
  }
}

#endif//ndef YADIF_KEY_HPP
