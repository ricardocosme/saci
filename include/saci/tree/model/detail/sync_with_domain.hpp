#pragma once

#include "saci/tree/model/leaves_impl.hpp"
#include "saci/tree/model/detail/get_collection.hpp"
#include "saci/tree/model/detail/get_object.hpp"

#include <boost/fusion/include/find.hpp>
#include <boost/fusion/include/vector.hpp>
#include <coruja/container/list.hpp>
#include <coruja/support/macro.hpp>

namespace saci { namespace tree { namespace detail {

//Reações responsáveis por sincronizar a hierarquia com o modelo de
//domínio da aplicação. Inserções e remoções na hierarquia específica
//da aplicação devem ser refletidas na hierarquia em árvore.
template<typename Node, typename T>
inline void sync_with_domain(Node& node, T& obj)
{
    node.observe_for_each
        (obj, [](Node& self, typename T::value_type& o)
        { self.children.emplace_back(o, self); });
        
    node.observe_before_erase
        (obj, [](Node& self, typename T::value_type& o) {
            self.children.remove_if([&o](typename Node::children_t::value_type& node)
            { return &o == node.obj; });
        });
}

template<typename Nodes, typename T>
coruja::list<T>& get_nodes(coruja::list<T>& children)
{ return children; }

template<typename Nodes, typename... T>
Nodes& get_nodes(boost::fusion::vector<T...>& children)
{ return *boost::fusion::find<Nodes>(children); }

template<typename Node, typename Nodes, typename T>
inline void sync_with_domain(Node& node, Nodes&, T& obj)
{
    node.observe_for_each
        (obj, [](Node& self, typename T::value_type& o)
        {
            auto& nodes = get_nodes<Nodes>(self.children);
            nodes.emplace_back(o, self);
        });
        
    node.observe_before_erase
        (obj, [](Node& self, typename T::value_type& o) {
            auto& nodes = get_nodes<Nodes>(self.children);
            nodes.remove_if([&o](typename Nodes::value_type& node)
            { return &o == node.obj; });
        });
}

template<typename Self>
struct sync_with_domain_t
{
    template<typename T>
    void operator()(coruja::list<T>& o) const {
        sync_with_domain(self, o, detail::get_object(self, o));
    }
    
    template<typename T, typename CheckPolicy, typename P>
    void operator()(leaves_impl<T, CheckPolicy, P>& o) const {
        sync_with_domain(
            self, o,
            detail::get_collection<leaves_impl<T, CheckPolicy, P>>(self));
    }
    template<typename T>
    void operator()(T& o) const {
        auto&& lvalue = detail::get_object(self, o);
        o = T(lvalue, self);
    }
    Self& self;
};

}}}

