
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace saci { namespace tree { namespace detail {

template<typename LeavesImpl, typename Self>
auto get_collection_ctx_aux(Self& self, std::false_type)
CORUJA_DECLTYPE_AUTO_RETURN
( typename LeavesImpl::get_collection{}(*self.obj, self.parent->ctx) )
    
template<typename LeavesImpl, typename Self>
auto get_collection_ctx_aux(Self& self, std::true_type)
CORUJA_DECLTYPE_AUTO_RETURN
( typename LeavesImpl::get_collection{}(*self.obj) )
    
template<typename LeavesImpl, typename Self>
auto get_collection_aux(Self& self, std::true_type)
CORUJA_DECLTYPE_AUTO_RETURN (
    get_collection_ctx_aux<LeavesImpl>(
        self,
        typename std::is_same<typename Self::parent_t::ctx_t, void>::type{})
)
    
template<typename LeavesImpl, typename Self>
auto get_collection_aux(Self& self, std::false_type)
CORUJA_DECLTYPE_AUTO_RETURN
( *self.obj )

template<typename LeavesImpl, typename Self>
auto get_collection(Self& self)
CORUJA_DECLTYPE_AUTO_RETURN (
    get_collection_aux<LeavesImpl>(
        self,
        typename detail::is_T_function_obj<
            typename LeavesImpl::type, typename LeavesImpl::parent_t>::type{})
)
    
}}}
