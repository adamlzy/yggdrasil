//utf8_string_pak_serialize.cpp

#ifndef __GNUC__
#	error "this file test of gnu only !!!"
#endif // __GNUC__

//#define YGGR_USE_STL_STRING

#define YGGR_SYS_DEFAULT_STRING_CHARSET "utf-8"

#include <iostream>
#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/binary_archive_partner.hpp>
#include <cassert>


#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

typedef yggr::packet::packet<yggr::archive::archive_partner::binary_oarchive_partner> opak;
typedef yggr::packet::packet<yggr::archive::archive_partner::binary_iarchive_partner> ipak;


void test_foo1(void)
{
	//yggr::utf8_string a("100");
	boost::container::string a("100");

	std::cout << a << std::endl;
	opak o;
	o.save(a);

	std::cout << o.org_buf() << std::endl;

	ipak i(o);
	//yggr::utf8_string b;
	boost::container::string b;
	i.load(b);

	std::cout << b << std::endl;

	assert(a.size() == b.size());
}

struct A
{
	A(void)
		: a(0), f(0.0f)
	{
	}

	A(int n)
		:a(n), f(123.5f), str("世界Hello")
	{
	}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, unsigned int version)
	{
		ar & a;
		ar & f;
		ar & str;
	}

	int a;
	float f;
	yggr::charset::utf8_string str;

};

void test_foo2(void)
{
	A a(100);

	std::cout << a.str << std::endl;
	opak o;
	o.save(a);

	std::cout << o.org_buf() << std::endl;

	ipak i(o);
	A b;
	i.load(b);

	std::cout << b.str << std::endl;
	assert(b.str.size() == a.str.size());
}

int main(int argc, char* argv[])
{
	test_foo1();
	test_foo2();
	char cc = 0;
	std::cin >> cc;
	return 0;
}