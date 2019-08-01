
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
    coruja::scoped_any_connection _model_to_view_conn, _enable_conn;
    ViewToModel _view_to_model;
    std::string _sig, _slot;

    void connect_view(const char* sig, const char* slot)
    { QObject::connect(_widget, sig, &_view_to_model, slot); }
    
    void disconnect_view(const char* sig, const char* slot)
    { QObject::disconnect(_widget, sig, &_view_to_model, slot); }public:
    
public:
    
    base_widget() = default;

    template<typename ObservableObject, typename ModelToView>
    base_widget(ObservableObject& model,
                QtWidget& widget,
                std::string sig,
                std::string slot,
                ModelToView model_to_view)
        : _widget(&widget)
        , _view_to_model(model, widget)
        , _sig(std::move(sig))
        , _slot(std::move(slot))
    {
        auto& rsig = _sig;
        auto& rslot = _slot;
        _model_to_view_conn = model.for_each(
        [&, model_to_view, rsig, rslot](T v)
        {
            disconnect_view(rsig.c_str(), rslot.c_str());
            model_to_view(widget, v);
            connect_view(rsig.c_str(), rslot.c_str());
        });
        _view_to_model.conn = _model_to_view_conn.get();
        connect_view(_sig.c_str(), _slot.c_str());
    }

    ~base_widget()
    { disconnect_view(_sig.c_str(), _slot.c_str()); }
    
    const QtWidget& widget() const noexcept
    { return *_widget; }
    
    QtWidget& widget() noexcept
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
