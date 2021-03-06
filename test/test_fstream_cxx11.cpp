//
//  Copyright (c) 2019 Alexander Grund
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/nowide/config.hpp>

#if BOOST_NOWIDE_CXX11

#include "test.hpp"
#include <boost/nowide/cstdio.hpp>
#include <boost/nowide/fstream.hpp>
#include <iostream>
#include <iterator>
#include <utility>

namespace nw = boost::nowide;

void create_file(const std::string& filename, const std::string& contents)
{
    nw::ofstream f(filename, std::ios::trunc);
    TEST(f << contents);
}

template<typename T>
std::string get_file_contents(T& stream)
{
    TEST(stream);
    std::string s((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    return s;
}

std::string get_file_contents(const std::string& filename)
{
    nw::ifstream f(filename);
    return get_file_contents(f);
}

nw::ifstream make_ifstream(const std::string& filename)
{
    nw::ifstream f(filename);
    TEST(f);
    std::string s;
    TEST(f >> s);
    TEST(s == "Hello");
    return f;
}

void test_ifstream(const std::string& filename)
{
    create_file(filename, "Hello\nWorld");
    // Move construct
    {
        nw::ifstream f = make_ifstream(filename);
        TEST(f);
        std::string s;
        TEST(f >> s);
        TEST(s == "World");
    }
    // Move assign
    {
        nw::ifstream f;
        {
            nw::ifstream f2 = make_ifstream(filename);
            f = std::move(f2);
        }
        TEST(f);
        std::string s;
        TEST(f >> s);
        TEST(s == "World");
    }
    // Swap
    {
        nw::ifstream f;
        {
            nw::ifstream f2 = make_ifstream(filename);
            swap(f, f2);
            TEST(!f2.is_open());
        }
        TEST(f);
        std::string s;
        TEST(f >> s);
        TEST(s == "World");
    }
    nw::remove(filename.c_str());
}

nw::ofstream make_ofstream(const std::string& filename)
{
    nw::ofstream f(filename);
    TEST(f);
    TEST(f << "Hello");
    return f;
}

void test_ofstream(const std::string& filename)
{
    // Move construct
    {
        nw::ofstream f = make_ofstream(filename);
        TEST(f);
        TEST(f << " world");
        f.close();
        TEST(get_file_contents(filename) == "Hello world");
    }
    // Move assign
    {
        nw::ofstream f;
        {
            nw::ofstream f2 = make_ofstream(filename);
            f = std::move(f2);
        }
        TEST(f);
        TEST(f << " world");
        f.close();
        TEST(get_file_contents(filename) == "Hello world");
    }
    // Swap
    {
        nw::ofstream f;
        {
            nw::ofstream f2 = make_ofstream(filename);
            swap(f, f2);
            TEST(!f2.is_open());
        }
        TEST(f);
        TEST(f << " world");
        f.close();
        TEST(get_file_contents(filename) == "Hello world");
    }
    nw::remove(filename.c_str());
}

nw::fstream make_fstream(const std::string& filename)
{
    create_file(filename, "");
    nw::fstream f(filename);
    TEST(f << "Hello");
    return f;
}

void test_fstream(const std::string& filename)
{
    // Move construct
    {
        nw::fstream f = make_fstream(filename);
        TEST(f);
        TEST(f << " world");
        f.seekg(0);
        TEST(get_file_contents(f) == "Hello world");
    }
    // Move assign
    {
        nw::fstream f;
        {
            nw::fstream f2 = make_fstream(filename);
            f = std::move(f2);
        }
        TEST(f);
        TEST(f << " world");
        f.seekg(0);
        TEST(get_file_contents(f) == "Hello world");
    }
    // Swap
    {
        nw::fstream f;
        {
            nw::fstream f2 = make_fstream(filename);
            swap(f, f2);
            TEST(!f2.is_open());
        }
        TEST(f);
        TEST(f << " world");
        f.seekg(0);
        TEST(get_file_contents(f) == "Hello world");
    }
    nw::remove(filename.c_str());
}

int main(int, char** argv)
{
    const std::string exampleFilename = std::string(argv[0]) + "-\xd7\xa9-\xd0\xbc-\xce\xbd.txt";
    try
    {
        test_ifstream(exampleFilename);
        test_ofstream(exampleFilename);
        test_fstream(exampleFilename);
    } catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    std::cout << "Ok" << std::endl;
    return 0;
}

#else

#include <iostream>

int main()
{
    std::cout << "Test skipped as there is no C++11 support by the compiler" << std::endl;
    return 0;
}

#endif
