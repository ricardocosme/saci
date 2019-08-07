#pragma once

#include <saci/tree/model/node_base.hpp>
#include <boost/serialization/saci/check.hpp>

#include <boost/fusion/include/size.hpp>
#include <boost/serialization/coruja_object.hpp>

namespace boost { namespace serialization {

template<typename Archive,
         typename Parent,
         typename CheckPolicy,
         typename ExpandPolicy,
         typename T>
inline void save(Archive& ar,
                 const saci::tree::node_base<
                 T, CheckPolicy, ExpandPolicy, Parent>& o,
                 unsigned int version)
{
    using branch_t = saci::tree::node_base<
        T, CheckPolicy, ExpandPolicy, Parent>;
    save_expand(ar, o, typename branch_t::expand_t{});
    save_check(ar, o, typename branch_t::check_t{});
}

template<typename Archive,
         typename Parent,
         typename CheckPolicy,
         typename ExpandPolicy,
         typename T>
inline void load(Archive& ar,
                 saci::tree::node_base<
                 T, CheckPolicy, ExpandPolicy, Parent>& o,
                 unsigned int version)
{
    using branch_t = saci::tree::node_base<
        T, CheckPolicy, ExpandPolicy, Parent>;
    load_expand(ar, o, typename branch_t::expand_t{});
    load_check(ar, o, typename branch_t::check_t{});
}

template<typename Archive,
         typename Parent,
         typename CheckPolicy,
         typename ExpandPolicy,
         typename T>
inline void serialize(Archive& ar,
                 saci::tree::node_base<
                 T, CheckPolicy, ExpandPolicy, Parent>& o,
                 unsigned int version)
{ split_free(ar, o, version); }

}}
