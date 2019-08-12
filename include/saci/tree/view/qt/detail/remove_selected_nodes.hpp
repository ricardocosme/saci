#pragma once

#include <algorithm>
#include <vector>

namespace saci { namespace tree { namespace view { namespace qt { namespace detail {

template<typename Children>
struct remove_selected_nodes {
    template<typename Node>
    void operator()(Node&& node) const {
        if(node.selected()) {
            its.push_back(
                std::find_if(
                    children.begin(), children.end(),
                    [&](typename Children::value_type& o)
                    { return &o == node.model().obj; }));
        }
    }
    Children& children;
    mutable std::vector<typename Children::iterator> its;
};

}}}}}
