#pragma once

#include <saci/tree/model/branch.hpp>
#include <saci/tree/model/leaves_impl.hpp>
#include <boost/serialization/saci/check.hpp>

#include <boost/serialization/coruja_object.hpp>

namespace boost { namespace serialization {

template<typename Archive,
         typename Parent,
         typename CheckPolicy,
         typename GetContainer,
         typename Children>
inline void save(Archive& ar,
                 const saci::tree::collection_branch_node_impl<
                 Parent, CheckPolicy, GetContainer, Children>& o,
                 unsigned int version)
{
    using branch_t = saci::tree::collection_branch_node_impl<
                 Parent, CheckPolicy, GetContainer, Children>;
    ar << o.expand;
    save_check(ar, o, typename branch_t::check_t{});
    ar << o.children.size();
    for(auto& e : o.children) {
        ar << node_obj_id(*e.obj);
        ar << e;
    }
}

template<typename Archive,
         typename Parent,
         typename CheckPolicy,
         typename GetContainer,
         typename Children>
inline void load(Archive& ar,
                 saci::tree::collection_branch_node_impl<
                 Parent, CheckPolicy, GetContainer, Children>& o,
                 unsigned int version)
{
    using branch_t = saci::tree::collection_branch_node_impl<
                 Parent, CheckPolicy, GetContainer, Children>;
    ar >> o.expand;
    load_check(ar, o, typename branch_t::check_t{});
    std::size_t n;
    ar >> n;
    using child_t = typename branch_t::child;
    for(std::size_t i(0); i < n; ++i) {
        std::string id;
        ar >> id;
        auto it = std::find_if(o.children.begin(), o.children.end(),
                               [&id](child_t& child){ return id == node_obj_id(*child.obj); });
        if(it != o.children.end())
            ar >> *it;
        else {
            typename branch_t::children_t::value_type skipped;
            ar >> skipped;
        }
    }
}

template<typename Archive,
         typename Parent,
         typename CheckPolicy,
         typename GetContainer,
         typename Children>
inline void serialize(Archive& ar,
                      saci::tree::collection_branch_node_impl<
                      Parent, CheckPolicy, GetContainer, Children>& o,
                      unsigned int version)
{ split_free(ar, o, version); }

template<typename Archive>
struct _save_child_branch_node {
    template<typename T>
    void operator()(const coruja::list<T>& o) const
    {
        ar << o.size();
        for(auto& e : o) {
            ar << node_obj_id(*e.obj);
            ar << e;
        }
    }

    template<typename T, typename CheckPolicy, typename Parent>
    void operator()(const saci::tree::leaves_impl<T, CheckPolicy, Parent>& o) const
    {
        ar << o.size();
        for(auto& e : o) {
            ar << node_obj_id(*e.obj);
            ar << e;
        }
    }
    
    template<typename T>
    void operator()(T& o) const
    {
        ar << o;
    }
    Archive& ar;
};

template<typename Archive>
struct _load_child_branch_node {
    template<typename T>
    void operator()(coruja::list<T>& o) const
    {
        std::size_t n;
        ar >> n;
        for(std::size_t i(0); i < n; ++i) {
            std::string id;
            ar >> id;
            auto it = std::find_if
                (o.begin(), o.end(),
                 [&id](T& child){ return id == node_obj_id(*child.obj); });
            if(it != o.end())
                ar >> *it;
            else {
                T skipped;
                ar >> skipped;
            }
        }
    }
    
    template<typename T, typename CheckPolicy, typename Parent>
    void operator()(saci::tree::leaves_impl<T, CheckPolicy, Parent>& o) const
    {
        std::size_t n;
        ar >> n;
        for(std::size_t i(0); i < n; ++i) {
            std::string id;
            ar >> id;
            auto it = std::find_if
                (o.begin(), o.end(),
                 [&id](typename saci::tree::leaves_impl<T, CheckPolicy, Parent>::value_type& child){ return id == node_obj_id(*child.obj); });
            if(it != o.end())
                ar >> *it;
            else {
                typename saci::tree::leaves_impl<T, CheckPolicy, Parent>::value_type skipped;
                ar >> skipped;
            }
        }
    }
    
    template<typename T>
    void operator()(T& o) const
    {
        ar >> o;
    }
    Archive& ar;
};

template<typename Archive,
         typename Parent,
         typename CheckPolicy,
         typename GetContainer,
         typename Children>
inline void save(Archive& ar,
                 const saci::tree::collection_branch_node_children_impl<
                 Parent, CheckPolicy, GetContainer, Children>& o,
                 unsigned int version)
{
    using branch_t = saci::tree::collection_branch_node_children_impl<
                 Parent, CheckPolicy, GetContainer, Children>;
    ar << o.expand;
    save_check(ar, o, typename branch_t::check_t{});
    ar << static_cast<std::size_t>(boost::fusion::size(o.children));
    boost::fusion::for_each(o.children, _save_child_branch_node<Archive>{ar});
}

template<typename Archive,
         typename Parent,
         typename CheckPolicy,
         typename GetContainer,
         typename Children>
inline void load(Archive& ar,
                 saci::tree::collection_branch_node_children_impl<
                 Parent, CheckPolicy, GetContainer, Children>& o,
                 unsigned int version)
{
    using branch_t = saci::tree::collection_branch_node_children_impl<
                 Parent, CheckPolicy, GetContainer, Children>;
    ar >> o.expand;
    load_check(ar, o, typename branch_t::check_t{});
    std::size_t n;
    ar >> n;
    boost::fusion::for_each(o.children, _load_child_branch_node<Archive>{ar});
}

template<typename Archive,
         typename Parent,
         typename CheckPolicy,
         typename GetContainer,
         typename Children>
inline void serialize(Archive& ar,
                      saci::tree::collection_branch_node_children_impl<
                      Parent, CheckPolicy, GetContainer, Children>& o,
                      unsigned int version)
{ split_free(ar, o, version); }

template<typename Archive,
         typename Parent,
         typename CheckPolicy,
         typename GetContainer,
         typename Children>
inline void serialize(Archive& ar,
                      saci::tree::collection_branch_node<
                      Parent, CheckPolicy, GetContainer, Children>& o,
                      unsigned int version)
{
    using node = saci::tree::collection_branch_node<
        Parent, CheckPolicy, GetContainer, Children>;
    ar & static_cast<typename node::base&>(o);
}

}}
