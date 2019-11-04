
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <coruja/support/signal/any_connection.hpp>
#include <coruja/support/signal/scoped_connection.hpp>
#include <QLabel>

#include <string>

namespace saci { namespace qt { 

class label
{
    QLabel* _widget{nullptr};
    coruja::scoped_any_connection _enable_conn;
public:    
    label() = default;

    template<typename ObservableObject>
    label(ObservableObject& model, QLabel& widget)
        : _widget(&widget)
    {
        model.for_each([&](std::string v){ widget.setText(v.c_str()); });
    }
    
    const QLabel& widget() const noexcept
    { return *_widget; }
    
    QLabel& widget() noexcept
    { return *_widget; }

    ///The boolean state of an ObservableObjectView defines if the
    ///widget is enabled.
    ///
    ///ObservableObjectView must have an observed_t equal to bool
    ///
    ///!Attention! It's not supported to call widget().setEnabled(b)
    ///when using this method.
    template<typename ObservableObjectView>
    void enable(ObservableObjectView&& o) {
        auto& w = widget();
        _enable_conn = o.for_each([&](bool v){ w.setEnabled(v); });
    }
};

}}
