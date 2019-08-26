#pragma once

#include <saci/tree/model/root.hpp>
#include <boost/serialization/saci/check.hpp>

#include <boost/serialization/coruja_object.hpp>

namespace boost { namespace serialization {

template<typename Archive, typename T, typename CheckPolicy, typename Child>
inline void save(Archive& ar,
                 const saci::tree::root<T, CheckPolicy, Child>& o,
                 unsigned int version)
{
    ar << o.expand;
    ar << o.children.size();
    for(auto& e : o.children) {
        ar << node_obj_id(*e.obj);
        ar << e;
    }
}

template<typename Archive, typename T, typename CheckPolicy, typename Child>
inline void load(Archive& ar,
                 saci::tree::root<T, CheckPolicy, Child>& o,
                 unsigned int version)
{
    std::cout << "[root load]" << std::endl;
    ar >> o.expand;
    std::size_t n;
    ar >> n;
    std::cout << "children="
              << n << std::endl;
    using root_t = saci::tree::root<T, CheckPolicy, Child>;
    using child_t = typename root_t::child;
    for(std::size_t i(0); i < n; ++i) {
        std::string id;
        ar >> id;
        std::cout << "[child '"
                  << id
                  << "'load]" << std::endl;
        auto it = std::find_if(o.children.begin(), o.children.end(),
                               [&id](child_t& child){ return id == node_obj_id(*child.obj); });
        if(it != o.children.end())
            ar >> *it;
        else {
            typename root_t::children_t::value_type skipped;
            ar >> skipped;
        }
    }
}

template<typename Archive, typename T, typename CheckPolicy, typename Child>
inline void serialize(Archive& ar,
                      saci::tree::root<T, CheckPolicy, Child>& o,
                      unsigned int version)
{ split_free(ar, o, version); }

}}
