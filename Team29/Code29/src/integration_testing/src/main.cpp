#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
////#include <string>
////#include <iostream>
////#include "parser.h"
////#include "PKB.h"
////
////TEST_CASE("Test integration between parser and PKB") {
////	std::string s =
////		"procedure main{\
////	x = x + y;\
////	y = y + z;\
////	print x;\
////	read y;\
////	if (z == 0) then            {\
////		if ((y == 1) && (x != (y + z))) then     {\
////			k = x + p;\
////			while ((k != (p + y)) || (z == (k * k)) && (l == (p + p)))) {\
////				k = y * y;\
////				read k;\
////				print zz;\
////				while ((k == (k)) && (pop != pop)) {\
////					zz = pq * zk + kz - pz / pl;\
////				}\
////			}\
////		}\
//// else {\
////			read k;\
////			z = (z + x) * (x + y) / p;\
////		}\
////	}\
//// else {\
////		print z;\
////		p = lp + pq;\
////	}\
//// print myName;\
////}";
////
////	REQUIRE(PKB::isParentStar(5, 13));
////	REQUIRE(PKB::IsFollows(7, 13));
////}