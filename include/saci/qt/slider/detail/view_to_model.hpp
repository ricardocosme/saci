
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <coruja/object/object.hpp>
#include <coruja/support/signal/scoped_blocked_connection.hpp>
#include <QObject>
#include <QSlider>

namespace saci { namespace qt { namespace detail {

class slider_to_model : public QObject {
    Q_OBJECT
public:
    slider_to_model() = default;
    slider_to_model(coruja::object<double>& pmodel, QSlider&)
        : model(pmodel) {}
    virtual ~slider_to_model() = default;
    coruja::object<double>& model;
    coruja::any_connection conn;
    double step;                               
public Q_SLOTS:
    void propagates(int v)
    {
        auto blk = coruja::make_scoped_blocked_connection(conn);
        model = v * step;
    }
};

}}}
