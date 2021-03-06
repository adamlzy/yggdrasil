//time.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __YGGR_TIME_TIME_HPP__
#define __YGGR_TIME_TIME_HPP__


#include <yggr/support/max_min_undef.ipp>

#include <ctime>
#include <algorithm>

#include <boost/thread/xtime.hpp>
#include <boost/serialization/access.hpp>

#include <yggr/move/move.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/serialization/nvp.hpp>
#include <yggr/mplex/strict_sizeof.hpp>

namespace yggr
{
namespace time
{

class time : public boost::xtime
{
public:
	enum
	{
		E_S_TO_MS = 1000,
		E_NS_TO_MS = 1000000,
		E_NS_TO_S = 1000000000,
		E_compile_u32 = 0xffffffff
	};

	typedef boost::system_time system_type;

	typedef boost::xtime base_type;
	typedef base_type::xtime_sec_t xtime_sec_t; // sys32 32bit sys64 64bit
	typedef base_type::xtime_nsec_t xtime_nsec_t; // win 32bit linux 64bit

private:
	typedef time this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	time(void);
	explicit time(const xtime_sec_t& sec);
	time(const xtime_sec_t& sec, const xtime_nsec_t& nsec);

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	time(BOOST_RV_REF(base_type) right)
	{
		base_type::sec = boost::forward<xtime_sec_t>(right.sec);
		base_type::nsec = boost::forward<xtime_nsec_t>(right.nsec);
	}
#else
	time(BOOST_RV_REF(base_type) right)
	{
	    base_type& right_ref = right;
	    std::swap(base_type::sec, right_ref.sec);
        std::swap(base_type::nsec, right_ref.nsec);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	time(const base_type& right);

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	time(BOOST_RV_REF(this_type) right)
	{
		base_type::sec = boost::forward<xtime_sec_t>(right.sec);
		base_type::nsec = boost::forward<xtime_nsec_t>(right.nsec);
	}
#else
	time(BOOST_RV_REF(this_type) right)
	{
	    this_type& right_ref = right;
	    std::swap(base_type::sec, right_ref.sec);
        std::swap(base_type::nsec, right_ref.nsec);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	time(const this_type& right);
	~time(void);

	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::sec = boost::forward<xtime_sec_t>(right.sec);
		base_type::nsec = boost::forward<xtime_nsec_t>(right.nsec);
#else
	    base_type& right_ref = right;
	    std::swap(base_type::sec, right_ref.sec);
        std::swap(base_type::nsec, right_ref.nsec);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
        return *this;
	}

	this_type& operator=(const base_type& right);

	inline this_type& operator= (BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::sec = boost::forward<xtime_sec_t>(right.sec);
		base_type::nsec = boost::forward<xtime_nsec_t>(right.nsec);
#else
	    this_type& right_ref = right;
	    std::swap(base_type::sec, right_ref.sec);
        std::swap(base_type::nsec, right_ref.nsec);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
        return *this;
	}

	this_type& operator=(const this_type& right);

	inline void swap(BOOST_RV_REF(base_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		std::swap(base_type::sec, right.sec);
		std::swap(base_type::nsec, right.nsec);
#else
		base_type& right_ref = right;
		std::swap(base_type::sec, right_ref.sec);
        std::swap(base_type::nsec, right_ref.nsec);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(base_type& right);

	inline void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		std::swap(base_type::sec, right.sec);
		std::swap(base_type::nsec, right.nsec);
#else
	    this_type& right_ref = right;
	    std::swap(base_type::sec, right_ref.sec);
	    std::swap(base_type::nsec, right_ref.nsec);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right);

	bool operator== (const xtime_sec_t& sec) const;
	bool operator== (const base_type& right) const;
	bool operator== (const this_type& right) const;

	bool operator!= (const xtime_sec_t& sec) const;
	bool operator!= (const base_type& right) const;
	bool operator!= (const this_type& right) const;

	bool operator< (const xtime_sec_t& sec) const;
	bool operator< (const base_type& right) const;
	bool operator< (const this_type& right) const;

	bool operator<= (const xtime_sec_t& sec) const;
	bool operator<= (const base_type& right) const;
	bool operator<= (const this_type& right) const;

	bool operator> (const xtime_sec_t& sec) const;
	bool operator> (const base_type& right) const;
	bool operator> (const this_type& right) const;

	bool operator>= (const xtime_sec_t& sec) const;
	bool operator>= (const base_type& right) const;
	bool operator>= (const this_type& right) const;

	this_type operator-(const xtime_sec_t& sec) const;
	this_type operator-(const this_type& right) const;

	this_type& operator-=(const xtime_sec_t& sec);
	this_type& operator-=(const this_type& right);

	this_type operator+(const xtime_sec_t& sec) const;
	this_type operator+(const this_type& right) const;

	this_type& operator+=(const xtime_sec_t& sec);
	this_type& operator+=(const this_type& right);

	template<typename T>
	const T ex_to_second(void) const
	{
		typedef T ret_type;
		return static_cast<ret_type>(sec) + static_cast<ret_type>(nsec) / E_NS_TO_S;
	}

	template<typename T>
	const T ex_to_millisecond(void) const
	{
		typedef T ret_type;
		return static_cast<ret_type>(sec) * E_S_TO_MS + (static_cast<ret_type>(nsec) + 500) / E_NS_TO_MS;
	}

	operator bool(void) const;
	void reset(void);
	void update(void);
	operator base_type&(void);
	operator const base_type&(void);

	std::tm to_tm(void) const;
	std::tm to_local_tm(void) const;

	std::string to_string(void) const;
	std::string to_local_string(void) const;

	std::string to_string(const std::string& format) const;
	std::string to_local_string(const std::string& format) const;

	template<typename T>
	static const T s_time_second(void)
	{
		this_type tmp;
		return tmp.ex_to_second<T>();
	}

	template<typename T>
	static const T s_time_millisecond(void)
	{
		this_type tmp;
		return tmp.ex_to_millisecond<T>();
	}

	static const this_type max_time(void);
	static const this_type min_time(void);

	std::size_t hash(void) const;

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		typedef Archive archive_type;
		yggr::s64 tsec = 0;
		yggr::s64 tnsec = 0;
		if(typename archive_type::is_saving())
		{
			tsec = base_type::sec; // sys32 32bit sys64 64bit
			tnsec = base_type::nsec; // win 32bit linux 64bit
		}
		ar & YGGR_SERIALIZE_NAME_NVP("sec", tsec);
		ar & YGGR_SERIALIZE_NAME_NVP("nsec",tnsec);
		if(typename archive_type::is_loading())
		{
			base_type::sec = tsec;
			base_type::nsec = tnsec;
		}
	}
};

} //namespace time
} //namespace yggr

#define _YGGR_TMP_PP_TIME_SWAP_DEF() \
    void swap(yggr::time::time::base_type& left, yggr::time::time& right); \
	void swap(yggr::time::time& left, yggr::time::time::base_type& right); \
	void swap(yggr::time::time& left, yggr::time::time& right); \
	\
	inline void swap(BOOST_RV_REF(yggr::time::time::base_type) left, yggr::time::time& right) { \
		typedef yggr::time::time::base_type now_base_type; \
		right.swap(left); } \
	\
	inline void swap(yggr::time::time::base_type& left, BOOST_RV_REF(yggr::time::time) right) { \
		std::swap(right.sec, left.sec); \
		std::swap(right.nsec, left.nsec); } \
	\
	inline void swap(BOOST_RV_REF(yggr::time::time) left, yggr::time::time::base_type& right) { \
		std::swap(right.sec, left.sec); \
		std::swap(right.nsec, left.nsec); } \
	\
	inline void swap(yggr::time::time& left, BOOST_RV_REF(yggr::time::time::base_type) right) { \
		typedef yggr::time::time::base_type now_base_type; \
		left.swap(right); } \
	\
	inline void swap(BOOST_RV_REF(yggr::time::time) left, yggr::time::time& right) { \
		typedef yggr::time::time now_this_type; \
		right.swap(left); } \
	\
	inline void swap(yggr::time::time& left, BOOST_RV_REF(yggr::time::time) right) { \
		typedef yggr::time::time now_this_type; \
		left.swap(right); }

namespace std
{
	_YGGR_TMP_PP_TIME_SWAP_DEF()
} // namespace std

namespace boost
{
	_YGGR_TMP_PP_TIME_SWAP_DEF()
} // namesapce boost

#undef _YGGR_TMP_PP_TIME_SWAP_DEF

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace time
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

std::size_t hash_value(const yggr::time::time& tm);

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} //namespace boost
#else
} // namespace time
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP


#endif //_YGGR_TIME_TIME_HPP__
