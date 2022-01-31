
// Tests Execute Functions
#include "fncBaseTest.h"


class fncFunctionTest : public fncBaseTest
{
	// Execution base - Tests basic fnc objects
};

TEST_F(fncFunctionTest, IntAddition)
{
	Num result = testEquation("1+2", "3");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("1+-2", "-1");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("-1+2", "1");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("-1+-2", "-3");
	EXPECT_TRUE(result.isInteger());
}

TEST_F(fncFunctionTest, IntSubtraction)
{
	Num result = testEquation("45-6", "39");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("-45-6", "-51");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("45--6", "51");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("-45--6", "-39");
	EXPECT_TRUE(result.isInteger());
}

TEST_F(fncFunctionTest, IntMultiply)
{
	Num result = testEquation("5*6", "30");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("-5*6", "-30");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("5*-6", "-30");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("-5*-6", "30");
	EXPECT_TRUE(result.isInteger());
}

TEST_F(fncFunctionTest, IntDivide)
{
	Num result = testEquation("30/5", "6");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("-30/5", "-6");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("30/-5", "-6");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("-30/-5", "6");
	EXPECT_TRUE(result.isInteger());
}

TEST_F(fncFunctionTest, IntDivideFloatResult)
{
	Num result = testEquation("32/5", "6.4");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("-32/5", "-6.4");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("32/-5", "-6.4");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("-32/-5", "6.4");
	EXPECT_TRUE(result.isDouble());
}

TEST_F(fncFunctionTest, IntFloatAddition)
{
	Num result = testEquation("1+2", "3");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("1+-2", "-1");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("-1+2", "1");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("-1+-2", "-3");
	EXPECT_TRUE(result.isInteger());
}

TEST_F(fncFunctionTest, IntFloatSubtraction)
{
	Num result = testEquation("45-6", "39");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("-45-6", "-51");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("45--6", "51");
	EXPECT_TRUE(result.isInteger());

	result = testEquation("-45--6", "-39");
	EXPECT_TRUE(result.isInteger());
}

// Even when number looks integer, it is a float
TEST_F(fncFunctionTest, IntFloatMultiply)
{
	Num result = testEquation("5*6.2", "31");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("-5*6.2", "-31");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("5*-6.2", "-31");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("-5*-6.2", "31");
	EXPECT_TRUE(result.isDouble());
}

TEST_F(fncFunctionTest, IntFloatDivide)
{
	Num result = testEquation("30/5.0", "6");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("-30/5.0", "-6");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("30/-5.0", "-6");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("-30/-5.0", "6");
	EXPECT_TRUE(result.isDouble());
}

TEST_F(fncFunctionTest, FloatIntAddition)
{
	Num result = testEquation("5.5+2", "7.5");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("5.5+-2", "3.5");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("-5.5+2", "-3.5");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("-5.5+-2", "-7.5");
	EXPECT_TRUE(result.isDouble());
}

TEST_F(fncFunctionTest, FloatIntSubtraction)
{
	Num result = testEquation("45.2-6", "39.2");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("-45.2-6", "-51.2");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("45.2--6", "51.2");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("-45.2--6", "-39.2");
	EXPECT_TRUE(result.isDouble());
}

TEST_F(fncFunctionTest, FloatIntMultiply)
{
	Num result = testEquation("5.2*6", "31.2");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("-5.2*6", "-31.2");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("5.2*-6", "-31.2");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("-5.2*-6", "31.2");
	EXPECT_TRUE(result.isDouble());
}

TEST_F(fncFunctionTest, FloatIntDivide)
{
	Num result = testEquation("30.2/5", "6.04");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("-30.2/5", "-6.04");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("30.2/-5", "-6.04");
	EXPECT_TRUE(result.isDouble());

	result = testEquation("-30.2/-5", "6.04");
	EXPECT_TRUE(result.isDouble());
}

	// "args": ["4-(.5/(3+4))"],     // Should return 3.92857...
	// "args": ["4-(.5/7)"],     // Should return 3.92857...
	// "args": ["4-.5/7"],    // Should return .5
	// "args": ["30 sin"],
	// "args": ["0.5rad asin"],
	// "args": ["sin(60)","5.6","*", "cos(30)"],
	// "args": ["6.*(9-1)/inv(10.0+2)-4.2"],
	// "args": ["3.4","*","5.3"],   // Funkiness on linux
	// "args": ["3.4 5.3"],         // default multiply
	// "args": ["0x5A * 0x48"]
