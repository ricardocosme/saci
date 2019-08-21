
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/core/lightweight_test.hpp>
#include <coruja/container/list.hpp>
#include <saci/tree/model/tree.hpp>
#include <iostream>
#include <initializer_list>
#include <string>

struct person_t {
    std::string name;
    coruja::list<std::string> skills;
};

using persons_t = coruja::list<person_t>;

struct Name {
    std::string& operator()(person_t& o)
    { return o.name; }
};

struct Skills {
    coruja::list<std::string>& operator()(person_t& o)
    { return o.skills; }
};

template<typename Node>
std::string& name(Node& o)
{ return *boost::fusion::at_c<0>(o.children).obj; }

template<typename Node>
coruja::list<std::string>& skills(Node& o)
{ return *boost::fusion::at_c<1>(o.children).obj; }

persons_t build_persons() {
    persons_t persons;
    persons.emplace_back(person_t{"joao", {"woodworking", "cooking"}});
    persons.emplace_back(person_t{"maria", {"cooking"}});
    persons.emplace_back(person_t{"alberto"});
    return persons;
}

int main() {
    using namespace saci::tree;

    {
        auto persons = build_persons();

        using tree_t =
            root<persons_t, UnCheckable,
                 leaves<persons_t, Checkable>
                 >;
        tree_t t(persons);
    }

    {
        auto persons = build_persons();

        using tree_t =
            root<persons_t, UnCheckable,
                 branches<persons_t, Checkable,
                          leaves<Skills, Checkable> >
                 >;
        tree_t t(persons);
    }
}
