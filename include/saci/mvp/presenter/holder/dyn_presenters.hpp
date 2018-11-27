
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <coruja/support/signal/any_connection.hpp>
#include <coruja/support/signal/scoped_connection.hpp>

#include <list>
#include <memory>
#include <utility>
#include <vector>

namespace saci { namespace mvp { namespace holder {

template<typename Presenter>
class dyn_presenters {
    using presenters_t = std::list<std::unique_ptr<Presenter>>;
    
    presenters_t _presenters;
    std::vector<coruja::scoped_any_connection> _conns;

    void register_onclose(typename presenters_t::iterator it)
    {
        _conns.emplace_back
            (it->get()->onClose([this, it]{ _presenters.erase(it); }));
    }

    void register_all_onclose()
    {
        for(auto it(_presenters.begin()); it != _presenters.end(); ++it)
            register_onclose(it);
    }
    
    template<typename>
    friend bool operator==(const dyn_presenters&, const dyn_presenters&);
    
public:
    
    dyn_presenters() = default;

    //TODO: get() -> std::list<Presenter*> (range view?)

    dyn_presenters(dyn_presenters&& rhs)
        : _presenters(std::move(rhs._presenters))
        , _conns(std::move(rhs._conns))
    { register_all_onclose(); }
    
    dyn_presenters& operator=(dyn_presenters&& rhs)
    {
        _presenters = std::move(rhs._presenters);
        _conns = std::move(rhs._conns);
        register_all_onclose();
        
        return *this;
    }
    
    template<typename... Args>
    Presenter& instance(Args&&... args) {
        auto it = _presenters.emplace
            (_presenters.end(), new Presenter(std::forward<Args>(args)...));
        register_onclose(it);
        return **it;
    }
};

template<typename Presenter>
inline bool operator==(const dyn_presenters<Presenter>& lhs,
                       const dyn_presenters<Presenter>& rhs)
{ return lhs._presenters == rhs._presenters; }

template<typename Presenter>
inline bool operator!=(const dyn_presenters<Presenter>& lhs,
                       const dyn_presenters<Presenter>& rhs)
{ return !(lhs == rhs); }

}}}
