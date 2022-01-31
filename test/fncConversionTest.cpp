// Tests Execute Functions
#include "fncBaseTest.h"

class fncConversionTest : public fncBaseTest
{
	// Execution base - Tests basic fnc objects
};



TEST_F(fncConversionTest, TemperatureConversion)
{
	// All numeric conversion should be double and unit define can be either with or without ":"
	Num result = testEquation("25C::F", "77F");
	EXPECT_TRUE(result.isDouble());
	EXPECT_TRUE(result.unit().isUnitType(UnitType::UNIT_TEMPERATURE));

	result = testEquation("25:C::F", "77F");
	EXPECT_TRUE(result.isDouble());
	EXPECT_TRUE(result.unit().isUnitType(UnitType::UNIT_TEMPERATURE));

	result = testEquation("41F::C", "5C");
	EXPECT_TRUE(result.isDouble());
	EXPECT_TRUE(result.unit().isUnitType(UnitType::UNIT_TEMPERATURE));

	result = testEquation("23F::C", "-5C");
	EXPECT_TRUE(result.isDouble());
	EXPECT_TRUE(result.unit().isUnitType(UnitType::UNIT_TEMPERATURE));

	result = testEquation("-40F::C", "-40C");
	EXPECT_TRUE(result.isDouble());
	EXPECT_TRUE(result.unit().isUnitType(UnitType::UNIT_TEMPERATURE));

	result = testEquation("-40C::F", "-40F");
	EXPECT_TRUE(result.isDouble());
	EXPECT_TRUE(result.unit().isUnitType(UnitType::UNIT_TEMPERATURE));

}

TEST_F(fncConversionTest, MetricLengthConversion)
{
	Num result = testEquation("1in::cm", "2.54cm");
	EXPECT_TRUE(result.isDouble());
	EXPECT_TRUE(result.unit().isUnitType(UnitType::UNIT_LENGTH));

	result = testEquation("1in::cm", "2.54cm");
	EXPECT_TRUE(result.isDouble());
	EXPECT_TRUE(result.unit().isUnitType(UnitType::UNIT_LENGTH));
}

