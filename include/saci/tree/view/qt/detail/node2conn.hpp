#pragma once

#include <coruja/support/signal/any_connection.hpp>
#include <unordered_map>

namespace saci { namespace tree { namespace view { namespace qt {
namespace detail {

using node_ptr = void*;
using node2conn = std::unordered_map<node_ptr, coruja::scoped_any_connection>;

}}}}}
