#pragma once

#include <coruja/object/object.hpp>

#include <unordered_map>

namespace saci { namespace tree { namespace view { namespace qt {
namespace detail {

using node_ptr = void*;
using item2obool_t = std::unordered_map<QTreeWidgetItem*,
                                        coruja::object<bool>*>;

}}}}}
