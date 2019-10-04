#pragma once

#include <coruja/object/object.hpp>
#include <saci/tree/view/qt/detail/node2conn.hpp>
#include <saci/tree/view/qt/detail/node2item.hpp>
#include <string>

namespace saci { namespace tree { namespace view { namespace qt {
namespace detail {

inline std::string strip_node_label(coruja::object<std::string>& o)
{ return o.observed(); }

inline std::string strip_node_label(std::string o)
{ return o; }

struct erase_node {
    template<typename Node>
    void operator()(Node& node) const {
        delete node2item[&node];
        blockable_conns.erase(&node);
    }
    QTreeWidget& tree;
    detail::node2conn& blockable_conns;
    detail::node2item_t& node2item;
};

}}}}}
