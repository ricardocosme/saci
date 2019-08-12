#pragma once

#include <coruja/object/object.hpp>
#include <saci/tree/view/qt/detail/item2check.hpp>
#include <saci/tree/view/qt/detail/node2item.hpp>
#include <saci/tree/view/qt/detail/node2conn.hpp>

#include <memory>
#include <string>
#include <vector>

namespace saci { namespace tree { namespace view { namespace qt {
namespace detail {

inline void handle_node_label(
    QTreeWidgetItem& item,
    std::vector<coruja::scoped_any_connection>& conns,
    coruja::object<std::string>& o)
{
    return conns.emplace_back(
        o.for_each([&item](std::string s)
        {
            item.setToolTip(0, s.c_str());
            item.setText(0, s.c_str());
        }));
}

inline void handle_node_label(
    QTreeWidgetItem&,
    std::vector<coruja::scoped_any_connection>&,
    std::string)
{}

struct insert_node {
    insert_node(
        QTreeWidget& w,
        detail::item2check_t& i2c,
        detail::node2item_t& n2i,
        detail::node2conn& vc,
        std::vector<coruja::scoped_any_connection>& c)
        : tree(w)
        , item2check(i2c)
        , node2item(n2i)
        , visible_conns(vc)
        , conns(c)
    {}
    
    insert_node(const insert_node& rhs)
        : tree(rhs.tree)
        , item2check(rhs.item2check)
        , node2item(rhs.node2item)
        , visible_conns(rhs.visible_conns)
        , conns(rhs.conns)
    {
    }
    
    template<typename Node>
    void operator()(Node& node)  {
        std::unique_ptr<QTreeWidgetItem> item(new QTreeWidgetItem);
        item2check[item.get()] = &node.check;
        node2item[&node] = item.get();
        handle_node_label(*item, conns, node_label(*(node.obj)));
        
        auto& ritem = *item;
        visible_conns.emplace(&node,
            node.visible.for_each([&ritem](bool v)
            {ritem.setCheckState(0, v ? Qt::Checked : Qt::Unchecked);}));

        //TODO: node expansion
        // node.expand.for_each([&ritem](bool v){ritem.setExpanded(v);});
        
        tree.addTopLevelItem(item.release());
    }

    QTreeWidget& tree;
    detail::item2check_t& item2check;
    detail::node2item_t& node2item;
    detail::node2conn& visible_conns;
    std::vector<coruja::scoped_any_connection>& conns;
};

}}}}}
