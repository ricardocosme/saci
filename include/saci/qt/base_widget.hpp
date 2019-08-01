
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <coruja/support/signal/any_connection.hpp>
#include <coruja/support/signal/scoped_connection.hpp>

namespace saci { namespace qt { 

template<typename QtWidget,
         typename T,
         typename ViewToModel>
class base_widget
{
    QtWidget* _widget{nullptr};
    coruja::scoped_any_connection _conn;
    ViewToModel _view_to_model;
    std::string _sig, _slot;
public:
    base_widget() = default;

    template<typename ObservableObject, typename UpdateModel>
    base_widget(ObservableObject& model,
                QtWidget& widget,
                std::string sig,
                std::string slot,
                UpdateModel update_model)
        : _widget(&widget)
        , _view_to_model(model)
        , _sig(std::move(sig))
        , _slot(std::move(slot))
    {
        _conn = model.for_each([&widget, update_model](T v)
        { update_model(widget, v); });
        _view_to_model.conn = _conn.get();
        QObject::connect(_widget, _sig.c_str(), &_view_to_model, _slot.c_str());
    }

    ~base_widget() {
        QObject::disconnect(_widget, _sig.c_str(),
                            &_view_to_model, _slot.c_str());
    }
    
    const QtWidget& widget() const noexcept
    { return *_widget; }
    
    QtWidget& widget() noexcept
    { return *_widget; }
};

}}
