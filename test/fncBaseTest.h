

// Tests Execute Functions

#include <gtest/gtest.h>

#include "exec.h"
#include "calstring.h"

#include "numDefs.h"

class fncBaseTest : public ::testing::Test
{
protected:
	void SetUp() override
	{

	}

	void TearDown() override
	{

	}

	Num runTest(std::string in)
	{
		CalString str{in};

		bool ok = m_cmd.parse(str, m_messageList);

		EXPECT_TRUE(ok);

		return m_cmd.run();
	}

	Num testEquation(std::string in, std::string expected)
	{
		std::cout << "===== Equation: '" << in.c_str() << "' ======== expecting '" << expected << "' =====" << std::endl;

		Num result = runTest(in);

		EXPECT_STREQ(result.asString().c_str(),expected.c_str());

		return result;
	}

	// Use the Exec command
	Exec  m_cmd;

	CalcList m_messageList;
};
