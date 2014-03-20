//-------------------- empty_base_optimization.test.cpp ------------------------
/*
    This file is part of rippled: https://github.com/ripple/rippled
    Copyright (c) 2014 Ripple Labs Inc.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#if BEAST_INCLUDE_BEASTCONFIG
#include "../../../BeastConfig.h"
#endif

#include "../empty_base_optimization.h"

#include "../../../modules/beast_core/beast_core.h" // for UnitTest

namespace beast {

class empty_base_optimization_Tests
    : public UnitTest
{
public:
    template <class T>
    class test1
        : private empty_base_optimization<T>
    {
        typedef empty_base_optimization<T> Base;
        void* m_p;
    public:
        explicit test1 (T const& t)
            : Base (t)
        {}

        T&       member()       {return Base::member();}
        T const& member() const {return Base::member();}
    };

    template <class T>
    class test2
    {
        void* m_p;
        T m_t;
    public:
        explicit test2 (T const& t)
            : m_t (t)
        {}

        T&       member()       {return m_t;}
        T const& member() const {return m_t;}
    };

    struct Empty
    {
        operator bool() {return true;}
    };

    static
    bool
    test_one ()
    {
        test1<int> t1(1);
        test2<int> t2(2);
        static_assert(sizeof(t1) == sizeof(t2), "don't optimize for int");
        if (t1.member() != 1)
            return false;
        if (t2.member() != 2)
            return false;
        return true;
    }

    static
    bool
    test_two ()
    {
        test1<Empty> t1((Empty()));
        test2<Empty> t2((Empty()));
        static_assert(sizeof(t1) < sizeof(t2), "do optimize for Empty");
        if (t1.member() != true)
            return false;
        if (t2.member() != true)
            return false;
        return true;
    }

    void
    runTest ()
    {
        beginTestCase ("empty_base_optimization");
        expect (test_one());
        expect (test_two());
        pass ();
    }

    empty_base_optimization_Tests()
        : UnitTest ("empty_base_optimization", "beast")
    {
    }
};

static empty_base_optimization_Tests empty_base_optimization_tests;

}  // beast
