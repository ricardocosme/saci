
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "saci/qt/push_btn/detail/clicked.hpp"

#include <coruja/support/signal/any_connection.hpp>
#include <QPushButton>

#include <utility>

namespace saci { namespace qt { inline namespace experimental {

class push_btn
{
    QPushButton* _widget{nullptr};
    coruja::scoped_any_connection _enable_conn;

    detail::clicked _clicked;
    
public:
    
    push_btn() = default;

    push_btn(std::string text, QPushButton& widget)
        : _widget(&widget)
    {
        _widget->setText(text.c_str());
        QObject::connect(_widget, SIGNAL(clicked(bool)),
                         &_clicked, SLOT(reaction(bool)));
    }

    ~push_btn() {
        QObject::disconnect(_widget, SIGNAL(clicked(bool)),
                            &_clicked, SLOT(reaction(bool)));
    }
    
    const QPushButton& widget() const noexcept
    { return *_widget; }
    
    QPushButton& widget() noexcept
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

    template<typename F>
    coruja::any_connection clicked(F&& f)
    { return _clicked.cbk.connect(std::forward<F>(f)); }
};
}}}
