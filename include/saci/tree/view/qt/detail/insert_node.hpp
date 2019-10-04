#pragma once

#include <coruja/object/object.hpp>
#include <saci/tree/view/qt/detail/item2obool.hpp>
#include <saci/tree/view/qt/detail/node2item.hpp>
#include <saci/tree/view/qt/detail/node2conn.hpp>

#include <memory>
#include <string>
#include <vector>

namespace saci { namespace tree { namespace view { namespace qt {
namespace detail {

inline void handle_node_label(
    QTreeWidgetItem& item,
    std::string s)
{
    item.setToolTip(0, s.c_str());
    item.setText(0, s.c_str());
}

inline void handle_node_label(
    QTreeWidgetItem& item,
    std::vector<coruja::scoped_any_connection>&,
    std::string s)
{ handle_node_label(item, s); }

inline void handle_node_label(
    QTreeWidgetItem& item,
    std::vector<coruja::scoped_any_connection>& conns,
    coruja::object<std::string>& o)
{
    return conns.emplace_back(
        o.for_each([&](std::string s){ handle_node_label(item, s); }));
}

template<typename Parent>
struct insert_node {
    insert_node(
        Parent& p,
        detail::item2obool_t& i2c,
        detail::item2obool_t& i2e,
        detail::node2item_t& n2i,
        detail::node2conn& vc,
        std::vector<coruja::scoped_any_connection>& c)
        : parent(p)
        , item2obool(i2c)
        , item2expand(i2e)
        , node2item(n2i)
        , blockable_conns(vc)
        , conns(c)
    {}


    template<typename Node>
    void handle_check(QTreeWidgetItem& item, Node& node, Checkable) {
        item2obool[&item] = &node.check;

        blockable_conns.emplace(&node,
            node.visible.for_each([&](bool v)
            {item.setCheckState(0, v ? Qt::Checked : Qt::Unchecked);}));
     }

    template<typename Node>
    void handle_check(QTreeWidgetItem& item, Node& node, UnCheckable)
    { /*do nothing*/ }
    
    template<typename Node>
    void handle_expand(QTreeWidgetItem& item, Node& node, saci::tree::detail::Expandable) {
        item2expand[&item] = &node.expand;
        blockable_conns.emplace(
            &node,
            node.expand.for_each([&](bool v){ item.setExpanded(v); }));
     }

    template<typename Node>
    void handle_expand(QTreeWidgetItem& item, Node& node, saci::tree::detail::UnExpandable)
    { /*do nothing*/ }
    
    template<typename Node>
    QTreeWidgetItem& operator()(Node& node)  {
        //parent has the ownership
        auto item = new QTreeWidgetItem{&parent};
        handle_check(*item, node, typename Node::check_t{});
        handle_expand(*item, node, typename Node::expand_t{});
        node2item[&node] = item;
        handle_node_label(*item, conns, node_label(*(node.obj)));

        return *item;
    }

    Parent& parent;
    detail::item2obool_t &item2obool, &item2expand;
    detail::node2item_t& node2item;
    detail::node2conn& blockable_conns;
    std::vector<coruja::scoped_any_connection>& conns;
};

}}}}}
