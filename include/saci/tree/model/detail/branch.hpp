#pragma once

#include "saci/tree/model/branch.hpp"
#include "saci/tree/model/branches.hpp"
#include "saci/tree/model/branches_impl.hpp"
#include "saci/tree/model/detail/get_branch_impl.hpp"
#include "saci/tree/model/detail/node_impl_fwd.hpp"

namespace saci { namespace tree { namespace detail {

template<typename Parent, typename Tag>
struct node_impl<Parent, Tag, typename std::enable_if<
                      std::is_base_of<tag_branch, Tag>::value>::type> {
    using type = typename legacy::get_branch_impl<
        typename Tag::type,
        typename Tag::check_t,
        typename Tag::children,
        Parent
        >::type;
};

template<typename Parent, typename Tag>
struct node_impl<Parent, Tag, typename std::enable_if<
                      std::is_base_of<tag_branches, Tag>::value>::type> {
    using type = branches_impl<
        typename Tag::type,
        typename Tag::check_t,
        typename Tag::children,
        Parent>;
};

}}}
