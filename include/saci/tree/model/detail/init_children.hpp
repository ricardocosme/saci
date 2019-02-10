#pragma once

#include "saci/tree/model/detail/tag.hpp"

namespace saci { namespace tree { namespace detail {

template<typename Self, typename Enable = void>
struct init_children;

template<typename Self>
struct init_children<Self,
                     typename std::enable_if<
                         !std::is_same<typename Self::parent_t::ctx_t, void>::value>::type>
{
    template<typename T>
    void operator()(T& o) const
    { o = T(self, typename T::get_container{}(*self.obj, self.parent->ctx)); }
    Self& self;
};

template<typename Self>
struct init_children<Self,
                     typename std::enable_if<
                         std::is_same<typename Self::parent_t::ctx_t, void>::value>::type>
{
    template<typename T>
    void operator()(T& o) const
    { o = T(self, typename T::get_container{}(*self.obj)); }
    Self& self;
};

}}}
