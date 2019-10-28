#pragma once

#include <coruja/support/signal/any_connection.hpp>
#include <boost/container/flat_map.hpp>

namespace saci { namespace tree { namespace view { namespace qt {
namespace detail {

using node_ptr = void*;
using node2conn = boost::container::flat_multimap<
    node_ptr, coruja::scoped_any_connection>;

}}}}}
