#pragma once

#include "saci/tree/model/branches_impl.hpp"
#include "saci/tree/model/leaves_impl.hpp"
#include "saci/tree/model/detail/get_collection.hpp"
#include "saci/tree/model/detail/get_object.hpp"

#include <boost/fusion/include/find.hpp>
#include <boost/fusion/include/vector.hpp>
#include <coruja/container/list.hpp>
#include <coruja/support/macro.hpp>

namespace saci { namespace tree { namespace detail {

template<typename Children, typename T>
inline coruja::list<T>& get_children(coruja::list<T>& children)
{ return children; }

template<typename Children, typename... T>
inline Children& get_children(boost::fusion::vector<T...>& children)
{ return *boost::fusion::find<Children>(children); }

template<typename Children, typename Parent, typename Model>
inline void sync_with_domain(Parent& parent, Model& model)
{
    //Constructs a child node for each element of the model.
    parent.observe_for_each
        (model, [](Parent& self, typename Model::value_type& model)
        { get_children<Children>(self.children).emplace_back(model, self); });
        
    //Removes the child node related to the element of the model that
    //will be removed.
    parent.observe_before_erase
        (model, [](Parent& self, typename Model::value_type& model) {
            get_children<Children>(self.children).remove_if(
                [&model](typename Children::value_type& child)
                { return &model == child.obj; });
        });
}

template<typename Node>
struct sync_with_domain_t
{
    template<typename T>
    void operator()(coruja::list<T>& o) const {
        sync_with_domain<coruja::list<T>>(self, detail::get_object(self, o));
    }
    
    template<typename T, typename CheckPolicy, typename Parent>
    void operator()(leaves_impl<T, CheckPolicy, Parent>& o) const {
        sync_with_domain<leaves_impl<T, CheckPolicy, Parent>>(
            self,
            detail::get_collection<leaves_impl<T, CheckPolicy, Parent>>(self));
    }

    template<typename T, typename CheckPolicy, typename Children, typename Parent>
    void operator()(branches_impl<T, CheckPolicy, Children, Parent>& o) const {
        sync_with_domain<branches_impl<T, CheckPolicy, Children, Parent>>(
            self,
            detail::get_collection<branches_impl<T, CheckPolicy, Children, Parent>>(self));
    }
    
    template<typename T>
    void operator()(T& o) const {
        auto&& lvalue = detail::get_object(self, o);
        o = T(lvalue, self);
    }
    Node& self;
};

}}}

