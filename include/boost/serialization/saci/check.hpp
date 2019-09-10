#pragma once

#include <saci/tree/model/detail/expand.hpp>
#include <saci/tree/model/detail/visibility.hpp>

namespace boost { namespace serialization {

template<typename Archive, typename Node>
inline void save_check(Archive& ar,
                        Node& node,
                        saci::tree::Checkable)
{ ar << node.check; }

template<typename Archive, typename Node>
inline void save_check(Archive& ar,
                        Node& node,
                        saci::tree::UnCheckable)
{}

template<typename Archive, typename Node>
inline void load_check(Archive& ar,
                        Node& node,
                        saci::tree::Checkable)
{ ar >> node.check; }

template<typename Archive, typename Node>
inline void load_check(Archive& ar,
                        Node& node,
                        saci::tree::UnCheckable)
{}

template<typename Archive, typename Node>
inline void save_expand(Archive& ar,
                        Node& node,
                        saci::tree::detail::Expandable)
{ ar << node.expand; }

template<typename Archive, typename Node>
inline void save_expand(Archive& ar,
                        Node& node,
                        saci::tree::detail::UnExpandable)
{}

template<typename Archive, typename Node>
inline void load_expand(Archive& ar,
                        Node& node,
                        saci::tree::detail::Expandable)
{ ar >> node.expand; }

template<typename Archive, typename Node>
inline void load_expand(Archive& ar,
                        Node& node,
                        saci::tree::detail::UnExpandable)
{}

template<typename Node>
inline void assign_expand(Node& lhs,
                          Node& rhs,
                          saci::tree::detail::Expandable)
{ lhs.expand = rhs.expand.observed(); }

template<typename Node>
inline void assign_expand(Node& lhs,
                          Node& rhs,
                          saci::tree::detail::UnExpandable)
{}

template<typename Node>
inline void assign_check(Node& lhs,
                          Node& rhs,
                          saci::tree::Checkable)
{ lhs.check = rhs.check.observed(); }

template<typename Node>
inline void assign_check(Node& lhs,
                          Node& rhs,
                          saci::tree::UnCheckable)
{}

}}
