#include "base.hpp"

namespace test {
static Atomic<i32> total = 0;
static Atomic<i32> failed = 0;
}
static inline
void expect_ex(bool pred, cstring msg, Source_Location loc){
	test::total.fetch_add(1);
	if(!pred){
		std::fprintf(stderr, "(%s:%d) FAIL: %s\n", loc.filename, loc.line, msg);
		test::failed.fetch_add(1);
	}
}
#define expect(Expr) expect_ex((Expr), #Expr, this_location())

void test_slice(){
	auto allocator = mem::heap_allocator();
	auto s0 = allocator.make<int>(16);
	auto s1 = allocator.make<int>(16);
	defer(allocator.destroy(s0));
	defer(allocator.destroy(s1));

	for(isize i = 0; i < s0.size(); i++){
		s0[i] = i;
		s1[i] = i;
	}

	expect(s0.equals(s1));
	s1[5] = 69;
	expect(!s0.equals(s1));

	expect(s0.slice(0, 5).equals(s1.slice(0, 5)));
	expect(s0.slice(6, s0.size()).equals(s1.slice(6)));
}

void test_string(){
	{
		String s = "   Heλλo, 世界 .  \n";
		expect(s.trim_leading(" \t\n") ==  "Heλλo, 世界 .  \n");
		expect(s.trim_trailing(" \t\n") ==  "   Heλλo, 世界 .");
		expect(s.trim(" \t\n") == "Heλλo, 世界 .");
	}
	{
		String s = "  世λHeλλo, 世界 世世λ世世";
		expect(s.trim_leading(" 世λ") ==  "Heλλo, 世界 世世λ世世");
		expect(s.trim_trailing(" 世λ") ==  "  世λHeλλo, 世界");
		expect(s.trim(" 世λ") == "Heλλo, 世界");
		expect(s.trim(" 世λ").trim(" 世λ") == "Heλλo, 世界");
	}
	{
		String s = "界";
		expect(s.trim_leading("界") ==  "");
		expect(s.trim_trailing("界") ==  "");
		expect(s.trim("界") ==  "");
	}
	{
		String s = "";
		expect(s.trim_leading("") ==  "");
		expect(s.trim_trailing("") ==  "");
		expect(s.trim("") ==  "");
	}
	{
		String s = "Heλλo, 世界.";
		expect(s.size() == 16);
		expect(s.rune_count() == 10);
	}
	{
		String s = "";
		expect(s.size() == 0);
		expect(s.rune_count() == 0);
	}
}

