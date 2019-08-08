
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/core/lightweight_test.hpp>
#include <coruja/container/list.hpp>
#include <saci/tree/model/tree.hpp>

#include <string>

using namespace saci::tree;

struct L2 {
};
struct L1 {
    using l2list_t = coruja::list<L2>;
    coruja::list<L2> l2list;
};

using l1list_t = coruja::list<L1>;

struct GetL2 {
    L1::l2list_t& operator()(L1& o) const {
        return o.l2list;
    }
};

int main() {
    //root UnCheckable with move assignment operator
    {
        l1list_t l1list;
        l1list.push_back(L1{{L2{}, L2{}}});
        
        using tree_t =
            root<l1list_t, UnCheckable,
                 leaves<l1list_t, UnCheckable>
                 >;
        tree_t lhs;
        lhs = tree_t(l1list);
        bool expected{false};
        lhs.visible.for_each
            ([&expected](bool v){ expected = v; });
        BOOST_TEST(expected);
        for(auto& l2 : lhs.children) {
            BOOST_TEST(l2.parent == &lhs);
        }
        expected = false;
        lhs.children.front().visible.for_each
            ([&expected](bool v){ expected = v; });
        BOOST_TEST(expected);
    }
    
    //root UnCheckable with move ctor
    {
        l1list_t l1list;
        l1list.push_back(L1{{L2{}, L2{}}});
        
        using tree_t =
            root<l1list_t, UnCheckable,
                 leaves<l1list_t, UnCheckable>
                 >;
        tree_t rhs(l1list);    
        auto lhs = std::move(rhs);
        bool expected{false};
        lhs.visible.for_each
            ([&expected](bool v){ expected = v; });
        BOOST_TEST(expected);
        for(auto& l2 : lhs.children) {
            BOOST_TEST(l2.parent == &lhs);
        }
        expected = false;
        lhs.children.front().visible.for_each
            ([&expected](bool v){ expected = v; });
        BOOST_TEST(expected);
    }

   //root Checkable with move assignment operator
    {
        l1list_t l1list;
        l1list.push_back(L1{{L2{}, L2{}}});
        
        using tree_t =
            root<l1list_t, Checkable,
                 leaves<l1list_t, UnCheckable>
                 >;
        tree_t lhs;
        lhs = tree_t(l1list);
        bool expected{false};
        lhs.visible.for_each
            ([&expected](bool v){ expected = v; });
        lhs.check = true;
        BOOST_TEST(expected);
        for(auto& l2 : lhs.children) {
            BOOST_TEST(l2.parent == &lhs);
        }
        expected = false;
        lhs.children.front().visible.for_each
            ([&expected](bool v){ expected = v; });
        BOOST_TEST(expected);
    }
    
   //root Checkable with move ctor
    {
        l1list_t l1list;
        l1list.push_back(L1{{L2{}, L2{}}});
        
        using tree_t =
            root<l1list_t, Checkable,
                 leaves<l1list_t, UnCheckable>
                 >;
        tree_t rhs(l1list);    
        auto lhs = std::move(rhs);
        bool expected{false};
        lhs.visible.for_each
            ([&expected](bool v){ expected = v; });
        lhs.check = true;
        BOOST_TEST(expected);
        for(auto& l2 : lhs.children) {
            BOOST_TEST(l2.parent == &lhs);
        }
        expected = false;
        lhs.children.front().visible.for_each
            ([&expected](bool v){ expected = v; });
        BOOST_TEST(expected);
    }

    //root Checkable and child Checkable with move assignment operator
    {
        l1list_t l1list;
        l1list.push_back(L1{{L2{}, L2{}}});
        
        using tree_t =
            root<l1list_t, Checkable,
                 leaves<l1list_t, Checkable>
                 >;
        tree_t lhs;
        lhs = tree_t(l1list);
        lhs.check = true;
        bool expected{false};
        lhs.children.front().visible.for_each
            ([&expected](bool v){ expected = v; });
        lhs.children.front().check = true;
        BOOST_TEST(expected);
    }
    
    //root Checkable and child Checkable with move ctor
    {
        l1list_t l1list;
        l1list.push_back(L1{{L2{}, L2{}}});
        
        using tree_t =
            root<l1list_t, Checkable,
                 leaves<l1list_t, Checkable>
                 >;
        tree_t rhs(l1list);    
        auto lhs = std::move(rhs);
        lhs.check = true;
        bool expected{false};
        lhs.children.front().visible.for_each
            ([&expected](bool v){ expected = v; });
        lhs.children.front().check = true;
        BOOST_TEST(expected);
    }

    //with_ctx

    //root UnCheckable with move assignment operator
    {
        int i{10};
        l1list_t l1list;
        l1list.push_back(L1{{L2{}, L2{}}});
        
        using tree_t =
            root<with_ctx<l1list_t, int>, UnCheckable,
                 leaves<l1list_t, UnCheckable>
                 >;
        tree_t lhs;
        lhs = tree_t(l1list, i);
        bool expected{false};
        lhs.visible.for_each
            ([&expected](bool v){ expected = v; });
        BOOST_TEST(expected);
        for(auto& l2 : lhs.children) {
            BOOST_TEST(l2.parent == &lhs);
        }
        expected = false;
        lhs.children.front().visible.for_each
            ([&expected](bool v){ expected = v; });
        BOOST_TEST(expected);
    }
    
    //root UnCheckable with move ctor
    {
        int i{10};
        l1list_t l1list;
        l1list.push_back(L1{{L2{}, L2{}}});
        
        using tree_t =
            root<with_ctx<l1list_t, int>, UnCheckable,
                 leaves<l1list_t, UnCheckable>
                 >;
        tree_t rhs(l1list, i);    
        auto lhs = std::move(rhs);
        bool expected{false};
        lhs.visible.for_each
            ([&expected](bool v){ expected = v; });
        BOOST_TEST(expected);
        for(auto& l2 : lhs.children) {
            BOOST_TEST(l2.parent == &lhs);
        }
        expected = false;
        lhs.children.front().visible.for_each
            ([&expected](bool v){ expected = v; });
        BOOST_TEST(expected);
    }

   //root Checkable with move assignment operator
    {
        int i{10};
        l1list_t l1list;
        l1list.push_back(L1{{L2{}, L2{}}});
        
        using tree_t =
            root<with_ctx<l1list_t, int>, Checkable,
                 leaves<l1list_t, UnCheckable>
                 >;
        tree_t lhs;
        lhs = tree_t(l1list, i);
        bool expected{false};
        lhs.visible.for_each
            ([&expected](bool v){ expected = v; });
        lhs.check = true;
        BOOST_TEST(expected);
        for(auto& l2 : lhs.children) {
            BOOST_TEST(l2.parent == &lhs);
        }
        expected = false;
        lhs.children.front().visible.for_each
            ([&expected](bool v){ expected = v; });
        BOOST_TEST(expected);
    }
    
   //root Checkable with move ctor
    {
        int i{10};
        l1list_t l1list;
        l1list.push_back(L1{{L2{}, L2{}}});
        
        using tree_t =
            root<with_ctx<l1list_t, int>, Checkable,
                 leaves<l1list_t, UnCheckable>
                 >;
        tree_t rhs(l1list, i); 
        auto lhs = std::move(rhs);
        bool expected{false};
        lhs.visible.for_each
            ([&expected](bool v){ expected = v; });
        lhs.check = true;
        BOOST_TEST(expected);
        for(auto& l2 : lhs.children) {
            BOOST_TEST(l2.parent == &lhs);
        }
        expected = false;
        lhs.children.front().visible.for_each
            ([&expected](bool v){ expected = v; });
        BOOST_TEST(expected);
    }

    //root Checkable and child Checkable with move assignment operator
    {
        int i{10};
        l1list_t l1list;
        l1list.push_back(L1{{L2{}, L2{}}});
        
        using tree_t =
            root<with_ctx<l1list_t, int>, Checkable,
                 leaves<l1list_t, Checkable>
                 >;
        tree_t lhs;
        lhs = tree_t(l1list, i);
        lhs.check = true;
        bool expected{false};
        lhs.children.front().visible.for_each
            ([&expected](bool v){ expected = v; });
        lhs.children.front().check = true;
        BOOST_TEST(expected);
    }
    
    //root Checkable and child Checkable with move ctor
    {
        int i{10};
        l1list_t l1list;
        l1list.push_back(L1{{L2{}, L2{}}});
        
        using tree_t =
            root<with_ctx<l1list_t, int>, Checkable,
                 leaves<l1list_t, Checkable>
                 >;
        tree_t rhs(l1list, i);    
        auto lhs = std::move(rhs);
        lhs.check = true;
        bool expected{false};
        lhs.children.front().visible.for_each
            ([&expected](bool v){ expected = v; });
        lhs.children.front().check = true;
        BOOST_TEST(expected);
    }
    
}
