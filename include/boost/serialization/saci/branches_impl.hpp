#pragma once

#include <saci/tree/model/branches_impl.hpp>
#include <boost/serialization/coruja_list.hpp>

namespace boost { namespace serialization {

template<typename Archive,
         typename T,
         typename CheckPolicy,
         typename Children,
         typename Parent>
inline void serialize(Archive& ar,
                      saci::tree::branches_impl<
                          T, CheckPolicy, Children, Parent>& o,
                      unsigned int version)
{
    serialize(ar, static_cast<typename saci::tree::branches_impl<
              T, CheckPolicy, Children, Parent>::base&>(o), version);
}

}}
