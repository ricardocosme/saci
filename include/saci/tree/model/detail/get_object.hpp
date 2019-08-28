
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace saci { namespace tree { namespace detail {

template<typename Node, typename Self>
auto get_object_ctx_aux(Self& self, std::false_type)
CORUJA_DECLTYPE_AUTO_RETURN
( typename Node::get_object{}(*self.obj, self.parent->ctx) )
    
template<typename Node, typename Self>
auto get_object_ctx_aux(Self& self, std::true_type)
CORUJA_DECLTYPE_AUTO_RETURN
( typename Node::get_object{}(*self.obj) )
    
template<typename Self, typename Node>
auto get_object_aux(Self& self, Node&, std::true_type)
CORUJA_DECLTYPE_AUTO_RETURN (
    get_object_ctx_aux<Node>(
        self,
        typename std::is_same<typename Self::parent_t::ctx_t, void>::type{})
)
    
template<typename Self, typename Node>
auto get_object_aux(Self& self, Node& node, std::false_type)
CORUJA_DECLTYPE_AUTO_RETURN
( *node.obj )

template<typename Self, typename Node>
auto get_object(Self& self, Node& node)
CORUJA_DECLTYPE_AUTO_RETURN (
    get_object_aux(
        self,
        node,
        typename detail::is_T_function_obj<
            typename Node::T, Self>::type{})
)
    
}}}
