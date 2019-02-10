
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <coruja/support/signal/any_connection.hpp>
#include <coruja/support/signal/signal.hpp>

#include <utility>

namespace saci { namespace mvp { namespace holder {

template<typename Presenter>
class presenter {
    Presenter _presenter;
    coruja::signal<void()> _before_close;
    
    void register_onclose()
    {
        _presenter.onclose
            ([this]{
                 _before_close();
                 _presenter = Presenter();
                 return false;
             });
    }
    
public:
    
    presenter() = default;
    
    presenter(presenter&& rhs)
        : _presenter(std::move(rhs._presenter))
    { register_onclose(); }
    
    presenter& operator=(presenter&& rhs) {
        _presenter = std::move(rhs._presenter);
        register_onclose();
        
        return *this;
    }
    
    Presenter& get() const noexcept
    { return _presenter; }

    Presenter* operator->() const noexcept
    { return &const_cast<Presenter&>(_presenter); }

    bool operator!() const noexcept
    { return _presenter == Presenter(); }

    template<typename F>
    coruja::any_connection before_close(F&& f)
    { return _before_close.connect(std::forward<F>(f)); }
    
    template<typename... Args>
    Presenter& build(Args&&... args) {
        _presenter = Presenter(std::forward<Args>(args)...);
        register_onclose();
        return _presenter;
    }
};

#define SACI_SINGLE_OF(presenter, ...) \
    if(!presenter) presenter.build(__VA_ARGS__);

}}}
