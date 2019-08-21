#pragma once

#include "saci/tree/model/detail/sync_with_domain.hpp"
#include "saci/tree/model/detail/tag.hpp"

#include "saci/tree/model/leaves_impl.hpp"

namespace saci { namespace tree { namespace detail {

template<typename Self, typename Enable = void>
struct init_children;

template<typename Self>
struct init_children<Self,
                     typename std::enable_if<
                         !std::is_same<typename Self::parent_t::ctx_t, void>::value>::type>
{
    template<typename T>
    void operator()(coruja::list<T>& o) const
    {  }
    
    template<typename T, typename CheckPolicy, typename Parent>
    void operator()(leaves_impl<T, CheckPolicy, Parent>& o) const {
    }
    
    template<typename T>
    void operator()(T& o) const
        { o = T(typename T::get_object{}(*self.obj, self.parent->ctx), self); }
    Self& self;
};

template<typename Self>
struct init_children<Self,
                     typename std::enable_if<
                         std::is_same<typename Self::parent_t::ctx_t, void>::value>::type>
{
    template<typename T>
    void operator()(coruja::list<T>& o) const {
        sync_with_domain(self, o, typename T::get_object{}(*self.obj));
    }
    
    template<typename T, typename CheckPolicy, typename Parent>
    void operator()(leaves_impl<T, CheckPolicy, Parent>& o) const {
        sync_with_domain(self, o, typename leaves_impl<T, CheckPolicy, Parent>
                         ::get_collection{}(*self.obj));
    }
    
    template<typename T>
    void operator()(T& o) const
    { o = T(typename T::get_object{}(*self.obj), self); }
    Self& self;
};

}}}
