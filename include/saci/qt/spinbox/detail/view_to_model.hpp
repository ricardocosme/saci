
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <coruja/object/object.hpp>
#include <coruja/support/signal/scoped_blocked_connection.hpp>
#include <QObject>
#include <QDoubleSpinBox>

namespace saci { namespace qt { namespace detail {

class spinbox_to_model : public QObject {
    Q_OBJECT
public:
    spinbox_to_model() = default;
    spinbox_to_model(coruja::object<double>& pmodel, QDoubleSpinBox&)
        : model(pmodel) {}
    virtual ~spinbox_to_model() = default;
    coruja::object<double>& model;
    coruja::any_connection conn;
public Q_SLOTS:
    void propagates(double v)
    {
        auto blk = coruja::make_scoped_blocked_connection(conn);
        model = v;
    }
};

}}}
