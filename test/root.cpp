
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/core/lightweight_test.hpp>
#include <coruja/container/list.hpp>
#include <saci/tree/model/tree.hpp>
#include <saci/tree/model/tree_io.hpp>

#include <initializer_list>
#include <string>
#include <unordered_map>

using namespace saci::tree;

using name = std::string;
using onames = coruja::list<name>;

template<typename CheckPolicy>
using tree_t = root<onames, CheckPolicy, leaves<onames, UnCheckable>>;

static_assert(std::is_same<typename tree_t<Checkable>::ctx_t, void>::value, "");

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
    auto model_it = c.begin();
    for(auto& child : t.children) {
        BOOST_TEST(child.obj == &*model_it++);
        BOOST_TEST(child.parent == &t);
    }
    c.clear();
    BOOST_TEST(t.children.empty());
}

enum class color_t {
    black,
    white
};

struct persons_traits {
    std::unordered_map<name, color_t> person2color;
};

namespace std {
template<typename CharType, typename CharTrait>
inline std::basic_ostream<CharType, CharTrait>&
operator<<(std::basic_ostream<CharType, CharTrait>& out, const onames& c)
{
    return (out << "Persons");
}
}

int main() {
    //Sync with the domain model (insert/erase)
    {
        {
            onames c{"abc", "def", "ghi"};
            tree_t<UnCheckable> t(c);
            sync_with_domain_tests(t, c);
        }
    
        //After move assignment operation
        {
            onames c{"abc", "def", "ghi"};
            tree_t<UnCheckable> rhs(c);
            tree_t<UnCheckable> lhs;
            lhs = std::move(rhs);
            sync_with_domain_tests(lhs, c);
        }

        //After move ctor
        {
            onames c{"abc", "def", "ghi"};
            tree_t<UnCheckable> rhs(c);
            tree_t<UnCheckable> lhs(std::move(rhs));
            sync_with_domain_tests(lhs, c);
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
        using tree = root<onames, Checkable, leaves<onames, Checkable>>;
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
        for(auto& child : t.children)
            BOOST_TEST(child.visible == true);

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

    //Using an object as a context attached to the ObservableErasableRange
    {
        onames c{"abc", "def", "ghi"};
        using tree = root<with_ctx<onames, persons_traits>,
                          Checkable,
                          leaves<onames, Checkable>>;
        static_assert(std::is_same<tree::ctx_t, persons_traits>::value, "");
        persons_traits traits;
        traits.person2color.emplace("abc", color_t::white);
        traits.person2color.emplace("def", color_t::black);
        traits.person2color.emplace("ghi", color_t::black);
        tree t(c, traits);
        BOOST_TEST(t.ctx == &traits);
    }

    //tree_io
    {
        onames c{"abc", "def", "ghi"};
        tree_t<Checkable> t(c);
        t.expand = true;
        std::cout << t << std::endl;
    }
}
