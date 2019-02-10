#pragma once

#include <coruja/object/object.hpp>

namespace saci { namespace tree {

struct UnExpandable {};
struct Expandable {};

namespace detail {

template<typename Policy>
struct expand {};

template<>
struct expand<Expandable>
{ coruja::object<bool> expand; };

}}}
