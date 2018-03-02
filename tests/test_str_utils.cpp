#include "gtest/gtest.h"
#include "str_utils.h"

namespace
{
    class test_str_utils: public ::testing::Test
    {
        protected:
        test_str_utils()
        {
            // Test constructor
        }
        virtual void TearDown ()
        {
            // Test destructor
        }

        // private variables
    };

    TEST_F(test_str_utils, str_extract_cpy_returns_success)
    {
        const char *test_str = "<p>This is a test string.</p>";
        char *test_res = NULL;
        const char *test_compare = "This is a test string.\0";
        int8_t compare_res = 0;

        test_res = str_extract_cpy(test_str, 3, 24);
        compare_res = strcmp(test_res, test_compare);

        EXPECT_EQ(0, compare_res);
        free(test_res);
    }

    TEST_F(test_str_utils, str_extract_cpy_handles_null_src)
    {
        const char *test_str = "<p>This is a test string.</p>";
        char *test_res = NULL;
        const char *test_compare = "This is a test string.\0";
        int8_t compare_res = 0;

        test_res = str_extract_cpy(NULL, 3, 24);
        if ( test_res != NULL )
        {
            compare_res = 1;
        }

        EXPECT_EQ(0, compare_res);
        free(test_res);
    }

    TEST_F(test_str_utils, str_extract_cpy_assert_start_pos_lt_end_pos)
    {
        ASSERT_DEATH({ size_t start_pos = 13; size_t end_pos = 10; str_extract_cpy(NULL, start_pos, end_pos); }, ".");
    }

    TEST_F(test_str_utils, find_first_ret_size_max_when_no_char)
    {
        const char *test_str = "This is a test string.";
        size_t test_res = 0;

        test_res = str_find_first(test_str, '<');
        EXPECT_EQ(SIZE_MAX, test_res);
    }

    TEST_F(test_str_utils, find_first_ret_size_max_on_null_str)
    {
        const char *test_str = NULL;
        size_t test_res = 0;

        test_res = str_find_first(test_str, '<');
        EXPECT_EQ(SIZE_MAX, test_res);
    }

    TEST_F(test_str_utils, find_first_ret_pos_when_found)
    {
        const char *test_str = "0123456789<strong>strong text</strong>.";
        size_t test_res = 0;

        test_res = str_find_first(test_str, '<');
        EXPECT_EQ(10, test_res);
    }

    TEST_F(test_str_utils, str_prepend_success)
    {
        char test_str[2048] = "Test Heading";
        char test_input[] = "=== ";
        char test_expected[2048] = "=== Test Heading";
        int8_t test_res = 0;

        str_prepend(test_str, test_input, sizeof(test_str));
        test_res = strcmp(test_str, test_expected);

        EXPECT_EQ(0, test_res);
    }

    TEST_F(test_str_utils, str_prepend_handles_null_dst)
    {
        str_prepend(NULL, NULL, 0);
    }

    TEST_F(test_str_utils, str_prepend_handles_null_src)
    {
        char test_str[2048] = "Test Heading";
        char test_input[] = "=== ";
        char test_expected[2048] = "Test Heading";
        int8_t test_res = 0;

        str_prepend(test_str, NULL, sizeof(test_str));
        test_res = strcmp(test_str, test_expected);

        EXPECT_EQ(0, test_res);
    }

    TEST_F(test_str_utils, strrep_success)
    {
        char test_str[2048] = "";
        char test_input[] = "===";
        char test_expected[2048] = "=========";
        int8_t test_res = 0;

        strrep(test_str, test_input, 3, sizeof(test_str));
        test_res = strcmp(test_str, test_expected);

        EXPECT_EQ(0, test_res);
    }

    TEST_F(test_str_utils, strrep_handles_null_src)
    {
        char test_str[2048] = "";
        char test_input[] = "===";
        char test_expected[2048] = "";
        int8_t test_res = 0;

        strrep(test_str, NULL, 3, sizeof(test_str));
        test_res = strcmp(test_str, test_expected);

        EXPECT_EQ(0, test_res);
    }

    TEST_F(test_str_utils, strrep_handles_null_dst)
    {
        char test_str[2048] = "";
        char test_input[] = "===";
        char test_expected[2048] = "";
        int8_t test_res = 0;

        strrep(NULL, test_input, 3, sizeof(test_str));
        test_res = strcmp(test_str, test_expected);

        EXPECT_EQ(0, test_res);
    }

    TEST_F(test_str_utils, str_rem_spc_success)
    {
        char test_str[2048] = "/t \n \v \f \rstr_rem_spc_success\0";
        char test_expected[2048] = "";

        strcpy(test_expected, test_str);
        int8_t test_res = 0;

        str_rem_spc(test_str);
        test_res = strcmp(test_str, test_expected);

        EXPECT_EQ(0, test_res);
    }

    TEST_F(test_str_utils, str_rem_spc_handles_null_src)
    {
        char test_str[2048] = "/t \n \v \f \rstr_rem_spc_success\0";
        char test_expected[2048] = "";

        strcpy(test_expected, test_str);
        int8_t test_res = 0;

        str_rem_spc(NULL);
        test_res = strcmp(test_str, test_expected);

        EXPECT_EQ(0, test_res);
    }

    TEST_F(test_str_utils, str_rem_spc_doesnt_find_ws)
    {
        char test_str[2048] = "str_rem_spc_success\0";
        char test_expected[2048] = "";

        strcpy(test_expected, test_str);
        int8_t test_res = 0;

        str_rem_spc(NULL);
        test_res = strcmp(test_str, test_expected);

        EXPECT_EQ(0, test_res);
    }

    TEST_F(test_str_utils, str_rem_spc_handles_empty_str)
    {
        char test_str[2048] = "\0";
        char test_expected[2048] = "";

        strcpy(test_expected, test_str);
        int8_t test_res = 0;

        str_rem_spc(NULL);
        test_res = strcmp(test_str, test_expected);

        EXPECT_EQ(0, test_res);
    }

}

int main (int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    int test_res = RUN_ALL_TESTS();

    if ( test_res != 0 )
    {
        printf("%s\n", "test_str_utils: tests failed");
    }

    return 0;
}
