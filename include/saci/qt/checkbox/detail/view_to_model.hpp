#pragma once

#include <coruja/object/any_object.hpp>
#include <coruja/support/signal/scoped_blocked_connection.hpp>
#include <QObject>

#include <utility>

namespace saci { namespace qt {  namespace detail {

class checkbox_to_model : public QObject {
    Q_OBJECT
public:
    checkbox_to_model() = default;
    checkbox_to_model(coruja::any_object<bool> pmodel)
        : model(std::move(pmodel)) {}
    virtual ~checkbox_to_model() = default;
    coruja::any_object<bool> model;
    coruja::any_connection conn;
public Q_SLOTS:
    void propagates(int v)
    {
        auto blk = coruja::make_scoped_blocked_connection(conn);
        model = v == Qt::Checked ? true : false;
    }
};

}}}
