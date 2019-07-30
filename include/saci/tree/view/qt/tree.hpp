#pragma once

#include <QTreeWidget>

#include <saci/tree/view/qt/detail/erase_node.hpp>
#include <saci/tree/view/qt/detail/insert_node.hpp>
#include <saci/tree/view/qt/detail/item2check.hpp>
#include <saci/tree/view/qt/detail/node2conn.hpp>
#include <saci/tree/view/qt/detail/propagates_check.hpp>

#include <memory>
#include <vector>

namespace saci { namespace tree { namespace view { namespace qt {

class tree {
public:
    tree() = default;
    
    template<typename Model>
    tree(Model& model, QTreeWidget& tree)
        : _tree(&tree)
        , _propagates_check(new detail::propagates_check(_item2check, _visible_conns))
    {
        _conns.push_back(
            model.children.for_each(
                detail::insert_node(*_tree, _item2check, _visible_conns, _conns)));
        
        _conns.emplace_back(
            model.children.before_erase(detail::erase_node{*_tree, _visible_conns}));
            
        QObject::connect
            (_tree, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
             _propagates_check.get(), SLOT(reaction(QTreeWidgetItem*, int)));
    }

    const QTreeWidget& widget() const
    { return *_tree; }

    QTreeWidget& widget() 
    { return *_tree; }
    
    QTreeWidget* _tree{nullptr};

    detail::item2check_t _item2check;
    detail::node2conn _visible_conns;
    std::vector<coruja::scoped_any_connection> _conns;
    
    std::unique_ptr<detail::propagates_check> _propagates_check;
};

}}}}
