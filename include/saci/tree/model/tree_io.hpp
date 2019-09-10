#pragma once

#include <ostream>

namespace saci { namespace tree {

template<typename OStream, typename Node>
inline OStream& out_check_dispatch(OStream& out, const Node& o, Checkable)
{
    out << (o.check.observed() ? "[x]" : "[ ]");
    return out;
}

template<typename OStream, typename Node>
inline OStream& out_check_dispatch(OStream& out, const Node& o, UnCheckable)
{ return out; }

template<typename OStream, typename Node>
inline OStream& out_check(OStream& out, const Node& o)
{ return out_check_dispatch(out, o, typename Node::check_t{}); }

template<typename OStream, typename Node>
inline OStream& out_expand_dispatch(OStream& out, const Node& o, detail::Expandable)
{
    out << (o.expand.observed() ? '-' : '+');
    return out;
}

template<typename OStream, typename Node>
inline OStream& out_expand_dispatch(OStream& out, const Node& o, detail::UnExpandable)
{
    out << '-';
    return out;
}

template<typename OStream, typename Node>
inline OStream& out_expand(OStream& out, const Node& o)
{ return out_expand_dispatch(out, o, typename Node::expand_t{}); }

template<typename OStream>
struct out_child_of_fusion {
    template<typename T>
    void operator()(const coruja::list<T>& children) const {
        out_children(out, children, level);
    }
    
    template<typename T>
    void operator()(T& child) const {
        for(std::size_t i(0); i < level; ++i)
            out << ' ';
        out_expand(out, child);
        out_check(out, child);
        out << *child.obj << std::endl;
        handle_children(out, child, level + 2);
    }
    OStream& out;
    std::size_t level;
};

template<typename OStream, typename... Children>
inline OStream& out_children(OStream& out, const boost::fusion::vector<Children...>& children,
                           std::size_t level)
{
    boost::fusion::for_each(children, out_child_of_fusion<OStream>{out, level});
    return out;
}

template<typename OStream, typename Child>
inline OStream& out_children(OStream& out, const coruja::list<Child>& children, std::size_t level)
{
    for(auto& child : children) {
        for(std::size_t i(0); i < level; ++i)
            out << ' ';
        out_expand(out, child);
        out_check(out, child);
        out << *child.obj << std::endl;
        handle_children(out, child, level + 2);
    }
    return out;
}

template<typename OStream, typename Node>
inline OStream& handle_children_dispatch(OStream& out, const Node& node, detail::Expandable,
                                       std::size_t level)
{
    if(node.expand.observed()) 
        out_children(out, node.children, level);
    return out;
}

template<typename OStream, typename Node>
inline OStream& handle_children_dispatch(OStream& out, const Node& node, detail::UnExpandable,
                                       std::size_t level)
{
    out_children(out, node.children, level);
    return out;
}

template<typename OStream, typename Parent, typename T, typename CheckPolicy>
inline OStream& handle_children_dispatch(OStream& out,
                                       const leaf_impl<Parent, CheckPolicy, T>& node,
                                       detail::UnExpandable,
                                       std::size_t level)
{ return out; }

template<typename OStream, typename Node>
inline OStream& handle_children(OStream& out, const Node& o, std::size_t level = 2)
{ return handle_children_dispatch(out, o, typename Node::expand_t{}, level); }

template<typename CharType, typename CharTrait,
         typename T, typename CheckPolicy, typename Child>
inline std::basic_ostream<CharType, CharTrait>&
operator<<(std::basic_ostream<CharType, CharTrait>& out,
           const root<T, CheckPolicy, Child>& o)
{
  if (out.good()) {
      out_expand_dispatch(out, o, detail::Expandable{});
      out_check(out, o);
      out<< *o.obj << std::endl;
      handle_children(out, o);      
  }
   
  return out;
}

}}
