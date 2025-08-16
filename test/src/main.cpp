#include <gtest/gtest.h>

using namespace std;

TEST(TestGroupName, Subtest_1) {
  ASSERT_TRUE(1 == 1);
}

TEST(TestGroupName, Subtest_2) {
  ASSERT_FALSE('b' == 'b');
  cout << "continue test after failure" << endl;
}
