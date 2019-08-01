#pragma once

#include <coruja/object/any_object_view.hpp>
#include <coruja/object/object.hpp>

namespace saci { namespace tree {

struct UnCheckable {};
struct Checkable {};

namespace detail {

template<typename Derived,
         typename CheckPolicy,
         typename Parent = void>
struct visibility;

template<typename Derived>
struct visibility<Derived, UnCheckable, void> {    
private:
    coruja::object<bool> check{true};
public:    
    visibility() : visible(check) {}
    
    visibility(visibility&& rhs)
        : check(std::move(rhs.check))
        , visible(check)
    {}
    
    visibility& operator=(visibility&& rhs) {
        check = std::move(rhs.check);
        visible = check;
        return *this;
    }
    
    //TODO: This doesn't make sense because visible is always true
    //here. We need to compose this with the check attribute of a
    //child to achieve the visible of that child. Actually, coruja
    //doesn't support compositions between observables and observeds. 
    coruja::any_object_view<bool> visible;
};

template<typename Derived>
struct visibility<Derived, Checkable, void> {
    visibility() : visible(check) {}

    visibility(visibility&& rhs)
        : check(std::move(rhs.check))
        , visible(check)
    {}
    
    visibility& operator=(visibility&& rhs) {
        check = std::move(rhs.check);
        visible = check;
        return *this;
    }
    
    coruja::object<bool> check{false};
    coruja::any_object_view<bool> visible;
};

template<typename Derived, typename Parent>
struct visibility<Derived, UnCheckable, Parent> {
    visibility() = default;
    
    visibility(Parent& p) : visible(p.visible) {}
    
    void update_parent_ptr(Parent& p)
    { visible = p.visible; }
    
    coruja::any_object_view<bool> visible;
};

template<typename Derived, typename Parent>
struct visibility<Derived, Checkable, Parent> {
    
    visibility() : check(false) {}
    visibility(Parent& p)
        : check(false)
        , visible(check && p.visible) {}

    visibility(visibility&& rhs) = default;

    visibility& operator=(visibility&& rhs) {
        check = std::move(rhs.check);
        visible = check;
        return *this;
    }
    
    void update_parent_ptr(Parent& p)
    { visible = check && p.visible; }
    
    coruja::object<bool> check;
    coruja::any_object_view<bool> visible;
};

}}}
