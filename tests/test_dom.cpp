#include <string.h>
#include <limits.h>
#include "gtest/gtest.h"
#include "dom.h"
#include "readable.h"

char loadf_path_str[PATH_MAX] = "";
FILE *load_file_test = NULL;

dom_node_t *test_dom_gen_example_dom (void)
{
    dom_node_t *test_root = NULL;

    if ( load_file_test != NULL )
    {
        test_root = dom_loadf(load_file_test);
    }

    return test_root;
}

namespace
{
    class test_dom: public ::testing::Test
    {
        protected:
        test_dom()
        {
            // Test constructor
        }
        virtual void TearDown ()
        {
            // Test destructor
        }

        // private variables
    };

    TEST_F(test_dom, node_success_init_and_free)
    {

        dom_node_t *test_root = NULL;
        const char *test_root_str = "html";

        test_root = dom_node_element(test_root_str);
        dom_node_free(test_root);
    }

    TEST_F(test_dom, node_success_single_append_and_free)
    {
        dom_node_t *test_root = NULL;
        const char *test_root_str = "html";

        dom_node_t *test_head = NULL;
        const char *test_head_str = "head";

        bool test_res = false;

        test_root = dom_node_element(test_root_str);
        test_head = dom_node_element(test_head_str);
        test_res = dom_node_append(test_root, test_head);
        EXPECT_EQ(true, test_res);

        dom_node_free(test_root);
    }

    TEST_F(test_dom, node_success_multi_sibling_append_and_free)
    {
        dom_node_t *test_root = NULL;
        const char *test_root_str = "html";

        dom_node_t *test_head = NULL;
        const char *test_head_str = "head";

        dom_node_t *test_body = NULL;
        const char *test_body_str = "body";

        bool test_res_head = false;
        bool test_res_body = false;

        test_root = dom_node_element(test_root_str);
        test_head = dom_node_element(test_head_str);
        test_body = dom_node_element(test_body_str);
        test_res_head = dom_node_append(test_root, test_head);
        test_res_body = dom_node_append(test_root, test_body);
        EXPECT_EQ(true, test_res_head && test_res_body);

        dom_node_free(test_root);
    }

    TEST_F(test_dom, node_success_multi_children_and_siblings_append_and_free)
    {
        dom_node_t *test_root = NULL;
        bool test_res = false;

        test_root = test_dom_gen_example_dom();
        if ( test_root != NULL )
        {
            test_res = true;
        }

        dom_node_print_ancestry(test_root);
        EXPECT_EQ(true, test_res);

        dom_node_free(test_root);
    }

    TEST_F(test_dom, node_success_count_children)
    {
        dom_node_t *test_root = NULL;
        const char *test_root_str = "html";

        dom_node_t *test_head = NULL;
        const char *test_head_str = "head";

        dom_node_t *test_body = NULL;
        const char *test_body_str = "body";

        bool test_res_head = false;
        bool test_res_body = false;
        int32_t num_children = 0;

        test_root = dom_node_element(test_root_str);
        test_head = dom_node_element(test_head_str);
        test_body = dom_node_element(test_body_str);
        test_res_head = dom_node_append(test_root, test_head);
        test_res_body = dom_node_append(test_root, test_body);
        EXPECT_EQ(true, test_res_head && test_res_body);

        dom_node_print_ancestry(test_root);
        num_children = dom_node_num_children(test_root);
        EXPECT_EQ(2, num_children);

        dom_node_free(test_root);
    }

    TEST_F(test_dom, node_fail_when_root_node_null)
    {
        dom_node_t *test_head = NULL;
        const char *test_head_str = "head";
        bool test_res = false;

        test_head = dom_node_element(test_head_str);
        test_res = dom_node_append(NULL, test_head);
        EXPECT_EQ(false, test_res);

        dom_node_free(test_head);
    }

    TEST_F(test_dom, node_fail_when_new_node_null)
    {
        dom_node_t *test_root = NULL;
        const char *test_root_str = "html";
        bool test_res = false;

        test_root = dom_node_element(test_root_str);
        test_res = dom_node_append(test_root, NULL);
        EXPECT_EQ(false, test_res);

        dom_node_free(test_root);
    }

    TEST_F(test_dom, dom_node_success_printable_heading)
    {
        dom_node_t *test_h1 = NULL;
        const char *test_h1_str = "h2";
        bool test_res = false;

        dom_node_t *test_h1_text = NULL;
        const char *test_h1_text_str = "text";
        const char *test_h1_text_text_str = "2018";
        bool test_res_h1_text = false;

        test_h1 = dom_node_element(test_h1_str);
        test_h1_text = dom_node_text();
        test_res_h1_text = dom_node_append(test_h1, test_h1_text);
        dom_node_set_text(test_h1_text, test_h1_text_text_str);

        dom_node_print_ancestry(test_h1);
        //dom_node_to_stream(test_h1, stdout);

        dom_node_free(test_h1);
    }

    TEST_F(test_dom, dom_node_success_loadf)
    {
        dom_node_t *test_dom = NULL;

        if ( load_file_test != NULL )
        {
            test_dom = dom_loadf(load_file_test);
        }

        dom_node_free(test_dom);
    }

    TEST_F(test_dom, dom_node_is_success_title)
    {
        dom_node_t *test_title = NULL;
        bool test_res = false;

        test_title = dom_node_title();
        test_res = dom_node_is_title(test_title);
        EXPECT_EQ(true, test_res);

        dom_node_free(test_title);
    }

    TEST_F(test_dom, dom_node_is_fail_title)
    {
        dom_node_t *test_br = NULL;
        bool test_res = false;

        test_br = dom_node_br();
        test_res = dom_node_is_title(test_br);
        EXPECT_EQ(false, test_res);

        dom_node_free(test_br);
    }

    TEST_F(test_dom, dom_node_is_handles_null)
    {
        bool test_res = false;

        test_res = dom_node_is_title(NULL);
        EXPECT_EQ(false, test_res);
    }

    TEST_F(test_dom, dom_node_print_ancestry_handles_null)
    {
        dom_node_print_ancestry(NULL);
    }

    TEST_F(test_dom, dom_node_success_html2stream)
    {
        dom_node_t *test_root = NULL;
        bool test_res = false;

        test_root = test_dom_gen_example_dom();
        setup_readable(test_root);
        dom_node_print_ancestry(test_root);
        dom_node_print_readable(test_root, stdout);

        dom_node_free(test_root);
    }

    TEST_F(test_dom, dom_node_html2stream_handles_null_node)
    {
        dom_node_t *test_root = NULL;
        bool test_res = false;

        test_root = test_dom_gen_example_dom();
        setup_readable(test_root);
        dom_node_print_readable(NULL, stdout);

        dom_node_free(test_root);
    }

    TEST_F(test_dom, dom_node_walk_handles_null_node)
    {
        dom_node_t *test_root = NULL;
        dom_node_t *test_out = NULL;
        bool test_res = false;

        test_out = dom_node_walk(test_root);

        if ( test_out == NULL )
        {
            test_res = true;
        }
        EXPECT_EQ(true, test_res);

        dom_node_free(test_root);
    }


}

int main (int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    int i = 0;

    for ( i = 1; i < argc; ++i )
    {
        if ( 0 == strcmp(argv[i], "-c"))
        {
            strncpy(loadf_path_str, argv[i + 1], 64);
            if ( access(loadf_path_str, F_OK) != -1 )
            {
                load_file_test = fopen(loadf_path_str, "r");
            }
        }
        i++;
    }

    printf("loadf_path_str: %s\n", loadf_path_str);

    int test_res = RUN_ALL_TESTS();

    if ( load_file_test != NULL )
    {
        fclose(load_file_test);
    }

    if ( test_res != 0 )
    {
        printf("%s\n", "test_dom: test failed");
    }

    return 0;
}
