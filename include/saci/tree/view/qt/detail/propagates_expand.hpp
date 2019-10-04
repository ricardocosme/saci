#pragma once

#include "saci/tree/view/qt/detail/propagates_expand_collapse.hpp"

#include <coruja/support/signal/any_connection.hpp>
#include <coruja/support/signal/scoped_connection.hpp>
#include <QObject>

#include <vector>

namespace saci { namespace tree { namespace view { namespace qt {
namespace detail {

class propagates_expand : public QObject {
    Q_OBJECT
public:
    propagates_expand(
        item2obool_t& n2e,
        node2conn& c)
        : item2expand(n2e)
        , conns(c)
    {}
    virtual ~propagates_expand() = default;
    item2obool_t &item2expand;
    node2conn &conns;
public Q_SLOTS:
    void reaction(QTreeWidgetItem* item) {
        expand_collapse(item, conns, item2expand,
        [](coruja::object<bool>& expand) {
            //TODO: Remove the conditional when for_each and
            //after_change emit a signal only for new values.
            if(!expand.observed()) expand = true;
        });
    }
};

//TODO [REPLACE]: Link the moc in the build process
#include <saci/tree/view/qt/detail/propagates_expand.moc>

}}}}}
