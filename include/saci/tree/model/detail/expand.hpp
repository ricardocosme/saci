#pragma once

#include <coruja/object/object.hpp>

namespace saci { namespace tree { namespace detail {

struct UnExpandable {};
struct Expandable {};

template<typename Policy>
struct expand {};

template<>
struct expand<Expandable>
{ coruja::object<bool> expand; };

}}}
