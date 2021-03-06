#pragma once

#include "saci/tree/model/branches_impl.hpp"
#include "saci/tree/model/leaves_impl.hpp"

namespace saci { namespace tree { namespace detail {

template<typename Parent>
struct update_parent_ptr {
    template<typename T>
    void operator()(T& o) const
    { o.update_parent_ptr(parent); }
    
    template<typename T, typename CheckPolicy, typename P>
    void operator()(leaves_impl<T, CheckPolicy, P>& o) const
    { for(auto& e : o) e.update_parent_ptr(parent); }
    
    template<typename T, typename CheckPolicy, typename C, typename P>
    void operator()(branches_impl<T, CheckPolicy, C, P>& o) const
    { for(auto& e : o) e.update_parent_ptr(parent); }
    
    Parent& parent;
};

}}}
