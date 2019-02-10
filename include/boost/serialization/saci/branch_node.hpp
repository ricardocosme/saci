#pragma once

#include <saci/tree/model/branch.hpp>
#include <boost/serialization/saci/check.hpp>

#include <boost/fusion/include/size.hpp>
#include <boost/serialization/coruja_object.hpp>

namespace boost { namespace serialization {

template<typename Archive>
struct save_child_branch_node {
    template<typename T>
    void operator()(T& o) const
    { ar << o; }
    Archive& ar;
};

template<typename Archive>
struct load_child_branch_node {
    template<typename T>
    void operator()(T& o) const
    { ar >> o; }
    Archive& ar;
};

template<typename Archive,
         typename Parent,
         typename CheckPolicy,
         typename T,
         typename Children>
inline void save(Archive& ar,
                 const saci::tree::branch_node<
                 Parent, CheckPolicy, T, Children>& o,
                 unsigned int version)
{
    using branch_t = saci::tree::branch_node<
        Parent, CheckPolicy, T, Children>;
    ar << o.expand;
    save_check(ar, o, typename branch_t::check_t{});
    ar << static_cast<std::size_t>(boost::fusion::size(o.children));
    boost::fusion::for_each(o.children, save_child_branch_node<Archive>{ar});
}

template<typename Archive,
         typename Parent,
         typename CheckPolicy,
         typename T,
         typename Children>
inline void load(Archive& ar,
                 saci::tree::branch_node<
                 Parent, CheckPolicy, T, Children>& o,
                 unsigned int version)
{
    using branch_t = saci::tree::branch_node<
        Parent, CheckPolicy, T, Children>;
    ar >> o.expand;
    load_check(ar, o, typename branch_t::check_t{});
    std::size_t n;
    ar >> n;
    boost::fusion::for_each(o.children, load_child_branch_node<Archive>{ar});
}

template<typename Archive,
         typename Parent,
         typename CheckPolicy,
         typename T,
         typename Children>
inline void serialize(Archive& ar,
                      saci::tree::branch_node<
                      Parent, CheckPolicy, T, Children>& o,
                      unsigned int version)
{ split_free(ar, o, version); }

}}
