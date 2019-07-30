
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "coruja/support/signal/any_connection.hpp"
#include "coruja/support/signal/any_signal_id_t.hpp"
#include "coruja/support/type_traits.hpp"

#include <functional>
#include <memory>
#include <utility>

namespace coruja { inline namespace experimental {

template<typename T>
class any_object
{
    struct any_object_iface
    {
        virtual ~any_object_iface() = default;
        virtual any_object_iface* clone() const = 0;
        virtual void assign(T) = 0;
        virtual T get() const noexcept = 0;
        virtual T observed() const noexcept = 0;
        virtual any_connection after_change(std::function<void(const T&)>) = 0;
        virtual any_connection for_each(std::function<void(const T&)>) = 0;
        virtual any_signal_id_t after_change_id() const noexcept = 0;
    };

    template<typename ObservableObject>
    struct model_t : any_object_iface
    {
        model_t() = default;
        
        template<typename U>
        model_t(U&& obj) : _obj(std::forward<U>(obj)) {}
        
        virtual ~model_t() = default;
        
        any_object_iface* clone() const override
        { return new model_t(_obj); }
        
        void assign(T o) override
        { _obj = std::move(o); }
        
        T get() const noexcept override
        { return _obj.get(); }
        
        T observed() const noexcept override
        { return get(); }
        
        any_connection after_change(std::function<void(const T&)> f) override
        { return _obj.after_change(std::move(f)); }
        
        any_connection for_each(std::function<void(const T&)> f) override
        { return _obj.for_each(std::move(f)); }
        
        any_signal_id_t after_change_id() const noexcept override
        { return _obj.after_change_id(); }
        
        ObservableObject _obj;
    };
    
    std::unique_ptr<any_object_iface> _model;
    
public:
    
    using observed_t = T;
    using value_type = observed_t;
    using after_change_connection_t = any_connection;
    using signal_id_t = ::coruja::any_signal_id_t;
    
    any_object() = default;
    
    template<typename ObservableObject,
             typename Enable = enable_if_t<
                 !std::is_same<
                     typename std::decay<ObservableObject>::type,
                     any_object>::value
                 >>
    any_object(ObservableObject&& o)
        : _model(new model_t<ObservableObject>
                 (std::forward<ObservableObject>(o)))
    {}

    any_object(const any_object& rhs)
        : _model(rhs._model ? rhs._model->clone() : nullptr)
    {}
    
    any_object& operator=(const any_object& rhs)
    {
        _model.reset(rhs._model ? rhs._model->clone() : nullptr);
        return *this;
    }
    
    template<typename U>
    enable_if_t<
        !std::is_same<remove_reference_t<U>, any_object>::value,
        any_object&>
    operator=(U&& rhs)
    {
        _model->assign(std::forward<U>(rhs));
        return *this;
    }
    
    any_object(any_object&& rhs) = default;
    any_object& operator=(any_object&&) = default;

    T get() const noexcept
    { return _model->get(); }

    T observed() const noexcept
    { return get(); }
    
    template<typename F>
    after_change_connection_t after_change(F&& f)
    { return _model->after_change(std::forward<F>(f)); }

    //Experimental
    template<typename F>
    after_change_connection_t for_each(F&& f)
    { return _model->for_each(std::forward<F>(f)); }

    //Experimental
    signal_id_t after_change_id() const noexcept
    { return _model->after_change_id(); }
};
        
}}
