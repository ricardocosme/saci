#pragma once

#include <coruja/object/object.hpp>
#include <saci/tree/view/qt/detail/node2conn.hpp>
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
        //TODO: Obj2Item
        auto items = tree.findItems(strip_node_label(node_label(*(node.obj))).c_str(),
                                    Qt::MatchExactly);
        delete items[0];

        visible_conns.erase(&node);
    }
    QTreeWidget& tree;
    detail::node2conn& visible_conns;
};

}}}}}
