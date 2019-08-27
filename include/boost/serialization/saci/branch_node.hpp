#pragma once

#include <saci/tree/model/branch.hpp>
#include <boost/serialization/saci/check.hpp>
#include <boost/serialization/version.hpp>

#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/serialization/coruja_object.hpp>

namespace boost { namespace serialization {

template<typename Archive>
struct save_child_branch_node {
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
    { ar << o; }
    Archive& ar;
};

template<typename Archive, typename BranchNode>
inline void save_impl(Archive& ar, BranchNode& o, unsigned int version,
                      boost::mpl::true_)
{
    ar << static_cast<std::size_t>(boost::fusion::size(o.children));
    boost::fusion::for_each(o.children, save_child_branch_node<Archive>{ar});
}

template<typename Archive, typename BranchNode>
inline void save_impl(Archive& ar, BranchNode& o, unsigned int version,
                      boost::mpl::false_)
{
    save_child_branch_node<Archive>{ar}(o.children);
}

template<typename Archive,
         typename T,
         typename CheckPolicy,
         typename Children,
         typename Parent>
inline void save(Archive& ar,
                 const saci::tree::branch_node<
                 T, CheckPolicy, Children, Parent>& o,
                 unsigned int version)
{
    using branch_t = saci::tree::branch_node<
        T, CheckPolicy, Children, Parent>;
    ar << o.expand;
    save_check(ar, o, typename branch_t::check_t{});
    save_impl(ar, o, version,
              boost::fusion::traits::is_sequence<typename branch_t::children_t>{});
}

template<typename Archive>
struct load_child_branch_node {
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
    { ar >> o; }
    Archive& ar;
};

template<typename Archive, typename BranchNode>
inline void load_impl(Archive& ar, BranchNode& o, unsigned int version,
                      boost::mpl::true_)
{
    std::size_t n;
    ar >> n;
    boost::fusion::for_each(o.children, load_child_branch_node<Archive>{ar});
}

template<typename Archive, typename BranchNode>
inline void load_impl(Archive& ar, BranchNode& o, unsigned int version,
                      boost::mpl::false_)
{
    load_child_branch_node<Archive>{ar}(o.children);
}

template<typename Archive,
         typename T,
         typename CheckPolicy,
         typename Children,
         typename Parent>
inline void load(Archive& ar,
                 saci::tree::branch_node<
                 T, CheckPolicy, Children, Parent>& o,
                 unsigned int version)
{
    using branch_t = saci::tree::branch_node<
        T, CheckPolicy, Children, Parent>;
    ar >> o.expand;
    load_check(ar, o, typename branch_t::check_t{});
    load_impl(ar, o, version,
              boost::fusion::traits::is_sequence<typename branch_t::children_t>{});
}

template<typename Archive,
         typename T,
         typename CheckPolicy,
         typename Children,
         typename Parent>
inline void serialize(Archive& ar,
                      saci::tree::branch_node<
                      T, CheckPolicy, Children, Parent>& o,
                      unsigned int version)
{
    split_free(ar, o, version);
}

template<typename Archive,
         typename T,
         typename CheckPolicy,
         typename Children,
         typename Parent>
inline void serialize(Archive& ar,
                      saci::tree::detail::legacy::branch_node_base<
                      T, CheckPolicy, Children, Parent>& o,
                      unsigned int version)
{
    ar & static_cast<typename saci::tree::detail::legacy::branch_node_base<
        T, CheckPolicy, Children, Parent>::base&>(o); 
}

}}
