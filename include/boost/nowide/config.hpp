//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//  Copyright (c) 2019 Alexander Grund
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_CONFIG_HPP_INCLUDED
#define BOOST_NOWIDE_CONFIG_HPP_INCLUDED

/// @file

#include <boost/config.hpp>
#include <boost/nowide/replacement.hpp>
#include <boost/version.hpp>

#if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_NOWIDE_DYN_LINK)
#ifdef BOOST_NOWIDE_SOURCE
#define BOOST_NOWIDE_DECL BOOST_SYMBOL_EXPORT
#else
#define BOOST_NOWIDE_DECL BOOST_SYMBOL_IMPORT
#endif // BOOST_NOWIDE_SOURCE
#else
#define BOOST_NOWIDE_DECL
#endif // DYN_LINK

//
// Automatically link to the correct build variant where possible.
//
#if !defined(BOOST_ALL_NO_LIB) && !defined(BOOST_NOWIDE_NO_LIB) && !defined(BOOST_NOWIDE_SOURCE)
//
// Set the name of our library, this will get undef'ed by auto_link.hpp
// once it's done with it:
//
#define BOOST_LIB_NAME boost_nowide
//
// If we're importing code from a dll, then tell auto_link.hpp about it:
//
#if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_NOWIDE_DYN_LINK)
#define BOOST_DYN_LINK
#endif
//
// And include the header that does the work:
//
#include <boost/config/auto_link.hpp>
#endif // auto-linking disabled

/// @def BOOST_NOWIDE_USE_WCHAR_OVERLOADS
/// @brief Whether to use the wchar_t* overloads in fstream/filebuf
/// Enabled on Windows and Cygwin as the latter may use wchar_t in filesystem::path
#if defined(BOOST_WINDOWS) || defined(__CYGWIN__)
#define BOOST_NOWIDE_USE_WCHAR_OVERLOADS 1
#else
#define BOOST_NOWIDE_USE_WCHAR_OVERLOADS 0
#endif

/// @def BOOST_NOWIDE_USE_FILEBUF_REPLACEMENT
/// @brief Define to 1 to use internal class from filebuf.hpp
///
/// - On Non-Windows platforms: Define to 1 to use the same class from header <filebuf.hpp>
///   that is used on Windows.
/// - On Windows: No effect, always overwritten to 1
///
/// Affects boost::nowide::basic_filebuf,
/// boost::nowide::basic_ofstream, boost::nowide::basic_ifstream, boost::nowide::basic_fstream
#if defined(BOOST_WINDOWS) || BOOST_NOWIDE_USE_WCHAR_OVERLOADS
#ifdef BOOST_NOWIDE_USE_FILEBUF_REPLACEMENT
#undef BOOST_NOWIDE_USE_FILEBUF_REPLACEMENT
#endif
#define BOOST_NOWIDE_USE_FILEBUF_REPLACEMENT 1
#elif !defined(BOOST_NOWIDE_USE_FILEBUF_REPLACEMENT)
#define BOOST_NOWIDE_USE_FILEBUF_REPLACEMENT 0
#endif

#if BOOST_VERSION < 106500 && defined(BOOST_GCC) && __GNUC__ >= 7
#define BOOST_NOWIDE_FALLTHROUGH __attribute__((fallthrough))
#else
#define BOOST_NOWIDE_FALLTHROUGH BOOST_FALLTHROUGH
#endif

// MSVC 2015 (1900) has reasonable C++11 support (especially auto-generated move ctors)
// libstdc++ < 5 does not support movable streams
#if(__cplusplus >= 201103L || (defined(BOOST_MSVC) && BOOST_MSVC >= 1900)) \
  && (!defined(BOOST_LIBSTDCXX_VERSION) || BOOST_LIBSTDCXX_VERSION >= 50000)
#define BOOST_NOWIDE_CXX11 1
#else
#define BOOST_NOWIDE_CXX11 0
#endif

namespace boost {
///
/// \brief This namespace includes implementations of the standard library functions and
/// classes such that they accept UTF-8 strings on Windows.
/// On other platforms (i.e. not on Windows) those functions and classes are just aliases
/// of the corresponding ones from the std namespace or behave like them.
///
namespace nowide {}
} // namespace boost

#endif // boost/nowide/config.hpp
