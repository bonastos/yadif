/*
 *  Copyright 2015 Dirk Bonekamper
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef YADIF_INDEX_SEQUENCE_HPP
#define YADIF_INDEX_SEQUENCE_HPP

#include <cstddef>

namespace yadif {
  namespace detail {
    template<std::size_t ...I>
    struct index_sequence
    {
      static constexpr std::size_t size() {
        return sizeof...(I);
      }
    };

    template<std::size_t N, std::size_t ...S>
    struct gen_index_sequence : gen_index_sequence<N - 1, N - 1, S...>
    {};

    template<std::size_t ...S>
    struct gen_index_sequence<0, S...>
    {
      using type = index_sequence<S...>;
    };

    template<std::size_t N>
    using make_index_sequence = typename gen_index_sequence<N>::type;
  }
}

#endif//ndef YADIF_INDEX_SEQUENCE_HPP
