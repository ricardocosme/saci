#pragma once

#include <QTreeWidget>

#include <saci/tree/view/qt/detail/erase_node.hpp>
#include <saci/tree/view/qt/detail/insert_node.hpp>
#include <saci/tree/view/qt/detail/item2obool.hpp>
#include <saci/tree/view/qt/detail/node2conn.hpp>
#include <saci/tree/view/qt/detail/node2item.hpp>
#include <saci/tree/view/qt/detail/propagates_check.hpp>
#include <saci/tree/view/qt/detail/propagates_collapse.hpp>
#include <saci/tree/view/qt/detail/propagates_expand.hpp>
#include <saci/tree/view/qt/detail/remove_selected_nodes.hpp>

#include <memory>
#include <vector>

namespace saci { namespace tree { namespace view { namespace qt {

template<typename Model>
class tree {
public:
    template<typename Node>
    class node {
        QTreeWidgetItem* _item{nullptr};
        Node* _model{nullptr};
    public:
        node() = default;
        explicit node(Node& model, QTreeWidgetItem& item)
            : _item(&item)
            , _model(&model)
        {}
        
        bool selected() const noexcept
        { return _item->isSelected(); }
        
        Node& model() noexcept
        { return *_model; }
        
        const Node& model() const noexcept
        { return *_model; }

        QTreeWidgetItem& item() noexcept
        { return *_item; }
        
        const QTreeWidgetItem& item() const noexcept
        { return *_item; }
    };
    
    tree() = default;
    
    tree(Model& model, QTreeWidget& tree)
        : _model(&model)
        , _tree(&tree)
        , _propagates_check(new detail::propagates_check(_item2obool, _blockable_conns))
        , _propagates_expand(new detail::propagates_expand(_item2expand, _blockable_conns))
        , _propagates_collapse(new detail::propagates_collapse(_item2expand, _blockable_conns))
    {
        auto& rootitem = detail::insert_node<QTreeWidget>
            (*_tree, _item2obool, _item2expand, _node2item, _blockable_conns, _node2conns)(model);

        _conns.push_back(
            model.children.for_each(
                detail::insert_node<QTreeWidgetItem>
                (rootitem, _item2obool, _item2expand, _node2item, _blockable_conns, _node2conns)));
        
        _conns.emplace_back(
            model.children.before_erase(
                detail::erase_node{*_tree, _blockable_conns, _node2conns, _node2item}));
            
        QObject::connect
            (_tree, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
             _propagates_check.get(), SLOT(reaction(QTreeWidgetItem*, int)));
        QObject::connect
            (_tree, SIGNAL(itemExpanded(QTreeWidgetItem*)),
             _propagates_expand.get(), SLOT(reaction(QTreeWidgetItem*)));
        QObject::connect
            (_tree, SIGNAL(itemCollapsed(QTreeWidgetItem*)),
             _propagates_collapse.get(), SLOT(reaction(QTreeWidgetItem*)));
    }

    ///Visit each node with a function F
    ///
    ///The function 'f' receives the visited node with the type `tree::node`.
    template<typename F>
    void for_each(F&& f) {
        //TODO: This method must be able to handle a tree with more levels.
        for(auto it = _model->children.begin(); it != _model->children.end();) {
            auto next = std::next(it);
            f(node<typename std::remove_reference<decltype(*it)>::type>
              {*it, *_node2item[&*it]});
            it = next;
        }
    }

    ///Removes the dynamic nodes that are selected
    void remove_selected_nodes() {
        //TODO: This method must be able to handle a tree with more levels.
        detail::remove_selected_nodes<typename Model::type>
            action{*_model->obj};
        for_each(action);
        for(auto it : action.its)
            _model->obj->erase(it);
    }

    const QTreeWidget& widget() const
    { return *_tree; }

    QTreeWidget& widget() 
    { return *_tree; }

    Model* _model{nullptr};
    QTreeWidget* _tree{nullptr};

    detail::item2obool_t _item2obool, _item2expand;
    detail::node2item_t _node2item;
    detail::node2conn _blockable_conns, _node2conns;
    std::vector<coruja::scoped_any_connection> _conns;
    
    std::unique_ptr<detail::propagates_check> _propagates_check;
    std::unique_ptr<detail::propagates_expand> _propagates_expand;
    std::unique_ptr<detail::propagates_collapse> _propagates_collapse;
};

}}}}
