#pragma once

#include <coruja/object/object.hpp>
#include <saci/tree/view/qt/detail/item2obool.hpp>
#include <saci/tree/view/qt/detail/node2item.hpp>
#include <saci/tree/view/qt/detail/node2conn.hpp>

#include <memory>
#include <string>

namespace saci { namespace tree { namespace view { namespace qt {
namespace detail {

inline void handle_node_label(
    QTreeWidgetItem& item,
    std::string s)
{
    item.setToolTip(0, s.c_str());
    item.setText(0, s.c_str());
}

template<typename Node>
inline void handle_node_label(
    Node&,
    QTreeWidgetItem& item,
    detail::node2conn&,
    std::string s)
{ handle_node_label(item, s); }

template<typename Node>
inline void handle_node_label(
    Node& node,
    QTreeWidgetItem& item,
    detail::node2conn& conns,
    coruja::object<std::string>& o)
{
    conns.emplace(
        &node,
        o.for_each([&](std::string s){ handle_node_label(item, s); }));
}

template<typename Parent>
struct insert_node_base {
    insert_node_base(
        Parent& p,
        detail::item2obool_t& i2c,
        detail::item2obool_t& i2e,
        detail::node2item_t& n2i,
        detail::node2conn& vc,
        detail::node2conn& c)
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
            node.check.for_each([&](bool v)
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
        handle_node_label(node, *item, conns, node_label(*(node.obj)));
        return *item;
    }

    Parent& parent;
    detail::item2obool_t &item2obool, &item2expand;
    detail::node2item_t& node2item;
    detail::node2conn& blockable_conns;
    detail::node2conn& conns;
};

template<typename Parent>
struct insert_node;

template<>
struct insert_node<QTreeWidgetItem> : insert_node_base<QTreeWidgetItem> {
    using base = insert_node_base<QTreeWidgetItem>;
    using base::base;

    template<typename Node>
    void handle_parent_visible(QTreeWidgetItem& item, Node& node) {
        conns.emplace(
            &node,
            node.parent->visible.for_each(
            [&](bool v)
            {
                item.setFlags(v ? item.flags() | Qt::ItemIsEnabled
                                : item.flags() ^ Qt::ItemIsEnabled);
            }));
    }
    
    template<typename Node>
    QTreeWidgetItem& operator()(Node& node)  {
        auto& item = base::operator()(node);
        handle_parent_visible(item, node);
        return item;
    }
};

template<>
struct insert_node<QTreeWidget> : insert_node_base<QTreeWidget> {
    using base = insert_node_base<QTreeWidget>;
    using base::base;

    template<typename Node>
    QTreeWidgetItem& operator()(Node& node)
    { return base::operator()(node); }
};

}}}}}
