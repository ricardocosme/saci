#pragma once

#include <coruja/object/any_object.hpp>
#include <coruja/support/signal/scoped_blocked_connection.hpp>
#include <QObject>

namespace saci { namespace qt { namespace detail {

class radio_btn_to_model : public QObject {
    Q_OBJECT
public:
    radio_btn_to_model() = default;
    radio_btn_to_model(coruja::any_object<bool> pmodel)
        : model(std::move(pmodel)) {}
    virtual ~radio_btn_to_model() = default;
    coruja::any_object<bool> model;
    coruja::any_connection conn;
public Q_SLOTS:
    void propagates(bool v)
    {
        auto blk = coruja::make_scoped_blocked_connection(conn);
        model = v;
    }
};

}}}
