
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

struct P {
    using type = person_t;
};

int main() {
    using namespace saci::tree;

    {
        auto persons = build_persons();

        using tree_t =
            root<persons_t, UnCheckable,
                 branches<persons_t, Checkable,
                          leaf<Name, UnCheckable>,
                          leaf<Skills, UnCheckable>
                          >
                 >;
        tree_t root(persons);
    
        auto& persons_nodes = root.children;
        
        BOOST_TEST(name(persons_nodes.front()) == "joao");
        BOOST_TEST(name(*std::next(persons_nodes.begin())) == "maria");
        BOOST_TEST(name(persons_nodes.back()) == "alberto");

        BOOST_TEST(skills(persons_nodes.front()).size() == 2);
        BOOST_TEST(skills(persons_nodes.front()).front() == "woodworking");
        BOOST_TEST(skills(persons_nodes.front()).back() == "cooking");
        BOOST_TEST(skills(persons_nodes.back()).empty());
    }

    {
        auto persons = build_persons();
    
        using tree_t =
            root<persons_t, UnCheckable,
                 branches<persons_t, Checkable,
                          leaves<Skills, UnCheckable>
                     >
                 >;
        tree_t root(persons);

        auto& persons_nodes = root.children;
        
        BOOST_TEST(persons_nodes.front().obj->name == "joao");
        BOOST_TEST(std::next(persons_nodes.begin())->obj->name == "maria");
        BOOST_TEST(persons_nodes.back().obj->name == "alberto");

        auto& skills_nodes = persons_nodes.front().children;
        BOOST_TEST(skills_nodes.size() == 2);
        BOOST_TEST(*skills_nodes.front().obj == "woodworking");
        BOOST_TEST(*skills_nodes.back().obj == "cooking");
    }
    
    {
        // auto persons = build_persons();
    
        // using tree_t =
        //     root<persons_t, UnCheckable,
        //          branches<persons_t, Checkable,
        //                   branch<Skills, UnCheckable,
        //                          leaves<std::string, UnCheckable>
        //                          >
        //                   >
        //          >;
        // tree_t root(persons);

        // auto& persons_nodes = root.children;
        
        // BOOST_TEST(persons_nodes.front().obj->name == "joao");
        // BOOST_TEST(std::next(persons_nodes.begin())->obj->name == "maria");
        // BOOST_TEST(persons_nodes.back().obj->name == "alberto");

        // auto& skills_node = persons_nodes.front().children;
        // auto& skills_nodes = skills_node.children;
        
        // BOOST_TEST(skills_nodes.size() == 2);
        // BOOST_TEST(*skills_nodes.front().obj == "woodworking");
        // BOOST_TEST(*skills_nodes.back().obj == "cooking");
    }
    
}
