//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
// Copyright (c) 2019-2020 Krystian Stasiowski (sdkrystian at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/static_string
//

#ifndef BOOST_STATIC_STRING_CONFIG_HPP
#define BOOST_STATIC_STRING_CONFIG_HPP

// Are we dependent on Boost?
// #define BOOST_STATIC_STRING_STANDALONE

// Can we have deduction guides?
#if __cpp_deduction_guides >= 201703L
#define BOOST_STATIC_STRING_USE_DEDUCT
#endif

// Include <version> if we can
#ifdef __has_include 
#if __has_include(<version>)
#include <version>
#endif
#endif

// Can we use __has_builtin?
#ifdef __has_builtin
#define BOOST_STATIC_STRING_HAS_BUILTIN(arg) __has_builtin(arg)
#else
#define BOOST_STATIC_STRING_HAS_BUILTIN(arg) 0
#endif

// Can we use is_constant_evaluated?
// Standard version
#if __cpp_lib_is_constant_evaluated >= 201811L
#define BOOST_STATIC_STRING_IS_CONST_EVAL std::is_constant_evaluated()
#elif BOOST_STATIC_STRING_HAS_BUILTIN(__builtin_is_constant_evaluated)
#define BOOST_STATIC_STRING_IS_CONST_EVAL __builtin_is_constant_evaluated()
#endif

// Can we use [[nodiscard]]?
#ifdef __has_attribute
#if __has_attribute(nodiscard)
#define BOOST_STATIC_STRING_NODISCARD [[nodiscard]]
#else
#define BOOST_STATIC_STRING_NODISCARD
#endif
#else
#define BOOST_STATIC_STRING_NODISCARD
#endif

// _MSVC_LANG isn't avaliable until after VS2015
#if defined(_MSC_VER) && _MSC_VER < 1910L
// The constexpr support in this version is effectively that of
// c++11, so we treat it as such
#define BOOST_STATIC_STRING_STANDARD_VERSION 201103L
#elif defined(_MSVC_LANG)
// MSVC doesn't define __cplusplus by default
#define BOOST_STATIC_STRING_STANDARD_VERSION _MSVC_LANG
#else
#define BOOST_STATIC_STRING_STANDARD_VERSION __cplusplus
#endif

#if BOOST_STATIC_STRING_STANDARD_VERSION > 201703L
#define BOOST_STATIC_STRING_CPP20
#define BOOST_STATIC_STRING_CPP17
#define BOOST_STATIC_STRING_CPP14
#define BOOST_STATIC_STRING_CPP11
#define BOOST_STATIC_STRING_CPP20_CONSTEXPR constexpr
#define BOOST_STATIC_STRING_CPP17_CONSTEXPR constexpr
#define BOOST_STATIC_STRING_CPP14_CONSTEXPR constexpr
#define BOOST_STATIC_STRING_CPP11_CONSTEXPR constexpr
#elif BOOST_STATIC_STRING_STANDARD_VERSION >= 201703L
#define BOOST_STATIC_STRING_CPP17
#define BOOST_STATIC_STRING_CPP14
#define BOOST_STATIC_STRING_CPP11
#define BOOST_STATIC_STRING_CPP20_CONSTEXPR
#define BOOST_STATIC_STRING_CPP17_CONSTEXPR constexpr
#define BOOST_STATIC_STRING_CPP14_CONSTEXPR constexpr
#define BOOST_STATIC_STRING_CPP11_CONSTEXPR constexpr
#elif BOOST_STATIC_STRING_STANDARD_VERSION >= 201402L
#define BOOST_STATIC_STRING_CPP14
#define BOOST_STATIC_STRING_CPP11
#define BOOST_STATIC_STRING_CPP20_CONSTEXPR
#define BOOST_STATIC_STRING_CPP17_CONSTEXPR
#define BOOST_STATIC_STRING_CPP14_CONSTEXPR constexpr
#define BOOST_STATIC_STRING_CPP11_CONSTEXPR constexpr
#else
#define BOOST_STATIC_STRING_CPP11
#define BOOST_STATIC_STRING_CPP20_CONSTEXPR
#define BOOST_STATIC_STRING_CPP17_CONSTEXPR
#define BOOST_STATIC_STRING_CPP14_CONSTEXPR
#define BOOST_STATIC_STRING_CPP11_CONSTEXPR constexpr
#endif

// Boost and non-Boost versions of utilities
#ifndef BOOST_STATIC_STRING_STANDALONE
#ifndef BOOST_STATIC_STRING_THROW_IF
#define BOOST_STATIC_STRING_THROW_IF(cond, ex) if (cond) BOOST_THROW_EXCEPTION(ex)
#endif
#ifndef BOOST_STATIC_STRING_THROW
#define BOOST_STATIC_STRING_THROW(ex) BOOST_THROW_EXCEPTION(ex)
#endif
#ifndef BOOST_STATIC_STRING_STATIC_ASSERT
#define BOOST_STATIC_STRING_STATIC_ASSERT(cond, msg) BOOST_STATIC_ASSERT_MSG(cond, msg)
#endif
#ifndef BOOST_STATIC_STRING_ASSERT
#define BOOST_STATIC_STRING_ASSERT(cond) BOOST_ASSERT(cond)
#endif
#else
#ifndef BOOST_STATIC_STRING_THROW_IF
#define BOOST_STATIC_STRING_THROW_IF(cond, ex) if (cond) throw ex
#endif
#ifndef BOOST_STATIC_STRING_THROW
#define BOOST_STATIC_STRING_THROW(ex) throw ex
#endif
#ifndef BOOST_STATIC_STRING_STATIC_ASSERT
#define BOOST_STATIC_STRING_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#endif
#ifndef BOOST_STATIC_STRING_ASSERT
#define BOOST_STATIC_STRING_ASSERT(cond) assert(cond)
#endif
#endif

#ifndef BOOST_STATIC_STRING_STANDALONE
#include <boost/assert.hpp>
#include <boost/container_hash/hash.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/throw_exception.hpp>
#else
#include <cassert>
#include <stdexcept>
#include <string_view>
#endif

// Compiler bug prevents constexpr from working with clang 4.x and 5.x
// if it is detected, we disable constexpr.
#if (BOOST_STATIC_STRING_STANDARD_VERSION >= 201402L && \
BOOST_STATIC_STRING_STANDARD_VERSION < 201703L) && \
defined(__clang__) && \
((__clang_major__ == 4) || (__clang_major__ == 5))
// This directive works on clang
#warning "C++14 constexpr is not supported in clang 4.x and 5.x due to a compiler bug."
#ifdef BOOST_STATIC_STRING_CPP14
#undef BOOST_STATIC_STRING_CPP14
#endif
#undef BOOST_STATIC_STRING_CPP14_CONSTEXPR
#define BOOST_STATIC_STRING_CPP14_CONSTEXPR
#endif

// This is for compiler/library configurations
// that cannot use the library comparison function
// objects at all in constant expresssions. In these
// cases, we use whatever will make more constexpr work.
#if defined(__clang__) && \
((defined(__GLIBCXX__) &&__GLIBCXX__ >= 20180726L \
&& __GLIBCXX__ <= 20191102L) || defined(_MSC_VER))
#define BOOST_STATIC_STRING_NO_PTR_COMP_FUNCTIONS
#endif

namespace boost {
namespace static_string {

/// The type of `basic_string_view` used by the library
template<typename CharT, typename Traits>
using basic_string_view =
#ifndef BOOST_STATIC_STRING_STANDALONE
  boost::basic_string_view<CharT, Traits>;
#else
  std::basic_string_view<CharT, Traits>;
#endif
} // static_string
} // boost
#endif