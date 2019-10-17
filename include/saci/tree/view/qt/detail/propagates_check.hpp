#pragma once

#include <coruja/support/signal/any_connection.hpp>
#include <coruja/support/signal/scoped_connection.hpp>
#include <QObject>

#include <vector>

namespace saci { namespace tree { namespace view { namespace qt {
namespace detail {

class propagates_check : public QObject {
    Q_OBJECT
public:
    propagates_check(
        item2obool_t& n2i,
        node2conn& c)
        : item2obool(n2i)
        , conns(c)
    {}
    virtual ~propagates_check() = default;
    item2obool_t &item2obool;
    node2conn& conns;
public Q_SLOTS:
    void reaction(QTreeWidgetItem* item, int col = 0) {
        auto& check = *item2obool[item];
        //TODO: RAII?
        for(auto& conn : conns) conn.second.get().block();
        auto check_state = item->checkState(col) == 2 ? true : false;
        //TODO: This conditional should be removed in the future when
        //`after_change` going to be fixed.
        if(check != check_state)
            check = check_state;
        for(auto& conn : conns) conn.second.get().unblock();
    }
};

//TODO [REPLACE]: Link the moc in the build process
#include <saci/tree/view/qt/detail/propagates_check.moc>

}}}}}
