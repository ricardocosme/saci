
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <coruja/object/object.hpp>
#include <coruja/support/signal/scoped_blocked_connection.hpp>
#include <QObject>
#include <QRadioButton>

namespace saci { namespace qt { namespace detail {

class radio_btn_to_model : public QObject {
    Q_OBJECT
public:
    radio_btn_to_model() = default;
    radio_btn_to_model(coruja::object<bool>& pmodel, QRadioButton&)
        : model(pmodel) {}
    virtual ~radio_btn_to_model() = default;
    coruja::object<bool>& model;
    coruja::any_connection conn;
public Q_SLOTS:
    void propagates(bool v)
    {
        auto blk = coruja::make_scoped_blocked_connection(conn);
        model = v;
    }
};

}}}
