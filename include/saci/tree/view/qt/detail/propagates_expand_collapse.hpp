#pragma once

namespace saci { namespace tree { namespace view { namespace qt {
namespace detail {

template<typename F>
inline void expand_collapse(QTreeWidgetItem* item,
                            node2conn& conns,
                            item2obool_t &item2obool,
                            F&& f)
{
    //TODO: RAII?
    for(auto& conn : conns) conn.second.get().block();
    auto it = item2obool.find(item);
    if(it != item2obool.end()) f(*it->second);
    for(auto& conn : conns) conn.second.get().unblock();
}

}}}}}
