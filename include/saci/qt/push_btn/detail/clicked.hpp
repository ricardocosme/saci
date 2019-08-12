
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <coruja/support/signal/signal.hpp>
#include <QObject>

namespace saci { namespace qt { namespace detail {

class clicked : public QObject {
    Q_OBJECT
public:
    clicked() = default;
    virtual ~clicked() = default;
    coruja::signal<void()> cbk;
public Q_SLOTS:
    void reaction(bool)
    { cbk(); }
};

}}}
