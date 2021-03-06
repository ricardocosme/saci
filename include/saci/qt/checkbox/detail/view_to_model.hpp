
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <coruja/object/object.hpp>
#include <coruja/support/signal/scoped_blocked_connection.hpp>
#include <QCheckBox>
#include <QObject>

#include <utility>

namespace saci { namespace qt {  namespace detail {

class checkbox_to_model : public QObject {
    Q_OBJECT
public:
    checkbox_to_model() = default;
    checkbox_to_model(coruja::object<bool>& pmodel, QCheckBox&)
        : model(pmodel) {}
    virtual ~checkbox_to_model() = default;
    coruja::object<bool>& model;
    coruja::any_connection conn;
public Q_SLOTS:
    void propagates(int v)
    {
        auto blk = coruja::make_scoped_blocked_connection(conn);
        model = v == Qt::Checked ? true : false;
    }
};

}}}
