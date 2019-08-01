
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/core/lightweight_test.hpp>
#include <coruja/container/list.hpp>
#include <saci/tree/model/tree.hpp>

#include <initializer_list>
#include <string>

using namespace saci::tree;

using name = std::string;
using onames = coruja::list<name>;

template<typename CheckPolicy>
using tree_t = root<onames, CheckPolicy, leaves<name, UnCheckable>>;

template<typename Root>
void equals_to(Root& r, std::initializer_list<name> expected) {
    BOOST_TEST(r.children.size() == expected.size());
    auto it = expected.begin();
    for(auto& node : r.children) {
        BOOST_TEST(*node.obj == *it++);
    }
}

template<typename Root, typename Domain>
void sync_with_domain_tests(Root& t, Domain& c) {
    equals_to(t, {"abc", "def", "ghi"});
    c.emplace_back("jkl");
    equals_to(t, {"abc", "def", "ghi", "jkl"});
    c.erase(c.begin());
    equals_to(t, {"def", "ghi", "jkl"});
    c.clear();
    BOOST_TEST(t.children.empty());
}

int main() {
    //Sync with the domain model (insert/erase)
    {
        {
            onames c{"abc", "def", "ghi"};
            tree_t<UnCheckable> t(c);
            sync_with_domain_tests(t, c);
        }
    
        //After move assignment operator
        {
            onames c{"abc", "def", "ghi"};
            tree_t<UnCheckable> rhs(c);
            auto t = std::move(rhs);
            sync_with_domain_tests(t, c);
        }

        //After move ctor
        {
            onames c{"abc", "def", "ghi"};
            tree_t<UnCheckable> rhs(c);
            tree_t<UnCheckable> t(std::move(rhs));
            sync_with_domain_tests(t, c);
        }
    }

    //Uncheckable - visible
    {
        onames c{"abc", "def", "ghi"};
        tree_t<UnCheckable> t(c);

        BOOST_TEST(t.visible == true);
        for(auto& child : t.children)
            BOOST_TEST(child.visible == true);
    }

    //Checkable - visible
    {
        onames c{"abc", "def", "ghi"};
        using tree = root<onames, Checkable, leaves<name, Checkable>>;
        // root<onames, Checkable, leaves<name, Checkable>> t(c);
        tree t(c);

        BOOST_TEST(t.check == false);
        BOOST_TEST(t.visible == false);
        for(auto& child : t.children) {
            BOOST_TEST(child.check == false);
            BOOST_TEST(child.visible == false);
        }
        for(auto& child : t.children) {
            child.check = true;
            BOOST_TEST(child.visible == false);
        }
        t.check = true;
        BOOST_TEST(t.visible == true);
        for(auto& child : t.children) {
            BOOST_TEST(child.visible == true);
        }

        //After move assignment operator
        tree l;
        l = std::move(t);
        l.check = false;
        for(auto& child : l.children) {
            BOOST_TEST(child.check == true);
            BOOST_TEST(child.visible == false);
        }        

        //After move ctor
        tree l2(std::move(l));
        l2.check = true;
        for(auto& child : l2.children) {
            BOOST_TEST(child.check == true);
            BOOST_TEST(child.visible == true);
        }        
    }
}
