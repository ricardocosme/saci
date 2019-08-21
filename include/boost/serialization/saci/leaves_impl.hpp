#pragma once

#include <saci/tree/model/leaves_impl.hpp>
#include <boost/serialization/coruja_list.hpp>

namespace boost { namespace serialization {

template<typename Archive,
         typename T,
         typename CheckPolicy,
         typename Parent>
inline void serialize(Archive& ar,
                 saci::tree::leaves_impl<
                 T, CheckPolicy, Parent>& o,
                 unsigned int version)
{
    serialize(ar, static_cast<typename saci::tree::leaves_impl<
            T, CheckPolicy, Parent>::base&>(o), version);
}

}}
