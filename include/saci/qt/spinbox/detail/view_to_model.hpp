#pragma once

#include <coruja/object/any_object.hpp>
#include <coruja/support/signal/scoped_blocked_connection.hpp>
#include <QObject>

namespace saci { namespace qt { namespace detail {

class spinbox_to_model : public QObject {
    Q_OBJECT
public:
    spinbox_to_model() = default;
    spinbox_to_model(coruja::any_object<double> pmodel)
        : model(std::move(pmodel)) {}
    virtual ~spinbox_to_model() = default;
    coruja::any_object<double> model;
    coruja::any_connection conn;
public Q_SLOTS:
    void propagates(double v)
    {
        auto blk = coruja::make_scoped_blocked_connection(conn);
        model = v;
    }
};

}}}
