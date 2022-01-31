// Tests Execute Functions
#include "fncBaseTest.h"

class fncExecTest : public fncBaseTest
{
	// Execution base - Tests basic fnc objects
};


TEST_F(fncExecTest, Initialization)
{
	CalString str;

	EXPECT_STREQ(str.c_str(), "");
}



TEST_F(fncExecTest, VariblePi)
{
	// Automatic conversion of 'pi' into radians
	Num result = testEquation("2pi/180", "0.034906585rad");
	EXPECT_TRUE(result.isDouble());
	EXPECT_TRUE(result.unit().isRad());

	// Rad to deg conversion
	result = testEquation("pi/180::deg", "1deg");
	EXPECT_TRUE(result.isDouble());
	EXPECT_FALSE(result.unit().isRad());

	result = testEquation("2pi::deg", "360deg"); 
	EXPECT_TRUE(result.isDouble());
	EXPECT_FALSE(result.unit().isRad());
}

TEST_F(fncExecTest, VariableSetting)
{

	Num result = runTest("y=3.2");
	std::string testVar{"y"};
	Num varValue;
	EXPECT_TRUE(Num::isVariable(testVar, varValue));
	EXPECT_STREQ(varValue.asString().c_str(), "3.2");

	result = testEquation("y=3.2 2pi*y+y", "23.306192983");
	EXPECT_TRUE(result.isDouble());
	EXPECT_FALSE(result.unit().isRad());
	EXPECT_TRUE(Num::isVariable(testVar, varValue));
	EXPECT_STREQ(varValue.asString().c_str(), "3.2");

	result = testEquation("y=3.2 2pi*y+y=y 5.2-y", "-18.106192983");
	EXPECT_TRUE(result.isDouble());
	EXPECT_TRUE(Num::isVariable(testVar, varValue));
	EXPECT_STREQ(varValue.asString().c_str(), "23.306192983");

	// While exec is still set, see if variable can be used
	result = testEquation("2y", "46.612385966");
	EXPECT_TRUE(Num::isVariable(testVar, varValue));
	EXPECT_STREQ(varValue.asString().c_str(), "23.306192983");
}

	// "args": ["y=3.2 2pi*y+y=pi 5.2-y"],
	// Test for bad message
	// "args": ["2y"],
