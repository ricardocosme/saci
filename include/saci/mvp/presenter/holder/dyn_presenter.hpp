
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <list>
#include <memory>
#include <utility>

namespace saci { namespace mvp { namespace holder {

template<typename Presenter>
class dyn_presenter {
    std::unique_ptr<Presenter> _presenter;
public:
    dyn_presenter() = default;
    
    Presenter* get() const noexcept
    { return _presenter; }

    Presenter* operator->() const noexcept
    { return _presenter.get(); }
    
    template<typename... Args>
    Presenter& instance(Args&&... args) {
        if(!_presenter) {
            std::unique_ptr<Presenter> o(
                new Presenter(std::forward<Args>(args)...));
            o->onClose([this]{ _presenter.release(); });
            _presenter = std::move(o);
        }
        return *_presenter;
    }
};

}}}
