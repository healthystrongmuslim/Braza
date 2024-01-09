// C++ includes used for precompiling -*- C++ -*-

// Copyright (C) 2003-2013 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file stdc++.h
 *  This is an implementation file for a precompiled header.
 */

// 17.4.1.2 Headers

// C
#ifndef _GLIBCXX_NO_ASSERT
#include <cassert>
#endif
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#if __cplusplus >= 201103L
#include <ccomplex>
#include <cfenv>
#include <cinttypes>
#include <cstdbool>
#include <cstdint>
#include <ctgmath>
#include <cwchar>
#include <cwctype>
#endif

// C++
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>

#if __cplusplus >= 201103L
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#endif
template <class moddingclass,typename t>
inline t mod(t n,moddingclass k){return (k>0)? n= n%k + k*((n%k)<0): 0;}

template <class moddingclass>
void mods(moddingclass m){}

template <class moddingclass,class T,typename... Args>
void mods(moddingclass m, T& b, Args&... a){
				//printf("\e[91;1mmodding %i by %i\n\e[0m",b,m);
				b=mod(b,m);
				mods(m,a...);
}

const char endcl[]="\33[0m\n";
const char* endc(){std::cout<<endcl;return endcl;}
inline void cls(){cout << "\e[2J\e[1;1H";}
inline void plswait(long double k){//float seconds value as argument
	// sleep_for(), sleep() and all those other functions don't work on mac terminal so i made this
				struct timespec remaining, request = { static_cast<long>(k),
								static_cast<long>(long((k-(long)k)*1000000000)%1000000000)};
				nanosleep(&request, &remaining);
}
/*
	Name            FG  BG
	Black           30  40
	Red             31  41
	Green           32  42
	Yellow          33  43
	Blue            34  44
	Magenta         35  45
	Cyan            36  46
	White           37  47
	Bright Black    90  100
	Bright Red      91  101
	Bright Green    92  102
	Bright Yellow   93  103
	Bright Blue     94  104
	Bright Magenta  95  105
	Bright Cyan     96  106
	Bright White    97  107
	*/
template <class rangingclass,typename t>
t rangein(t & n,rangingclass end,rangingclass start=0){//given a number n, the range from start to end is sized up, the number is modded by the size. start is added as the starting point. if start > end, range count is backwards.
				(end!=start)? (end>start)? mod(n,end-start+1): (mod(n,start-end+1),n*=-1):n=0;n+=start;
				return n;
}
template <class rangingclass>
void rangeins(rangingclass start, rangingclass end){}
template <class rangingclass,class T,typename... Args>
void rangeins(rangingclass start,rangingclass end, T& b, Args&... a){
				//printf("\e[91;1msetting %i in range:%i—%i\n\e[0m",b,start,end);
				rangein(b,end,start);
				rangeins(start,end,a...);
}
