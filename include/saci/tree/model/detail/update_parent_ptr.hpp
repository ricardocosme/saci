#pragma once

namespace saci { namespace tree { namespace detail {

template<typename Parent>
struct update_parent_ptr {
    template<typename T>
    void operator()(T& o) const
    { o.update_parent_ptr(parent); }
    
    template<typename T>
    void operator()(coruja::list<T>& o) const
    { for(auto& e : o) e.update_parent_ptr(parent); }
    
    Parent& parent;
};

}}}
