#pragma once

#include <type_traits>

namespace saci { namespace tree {

template<typename ValueType>
struct is_collection_leaf {
    template<typename Child>
    struct apply {
        using type = std::is_same<ValueType, typename Child::type>;
    };
};

}}
