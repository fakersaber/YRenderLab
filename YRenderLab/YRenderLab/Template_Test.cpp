#include <Public/Basic/FunctionTraits.h>

struct A {

};

struct B {
	int member;
	
	void memberFunc() {};
};



void test() {
	static_assert(CheckMember::has_member<B>::value);
	//static_assert(CheckMember::has_member<A>::value);

	static_assert(CheckMemberFunc::has_memberFunc<B>::value);
}