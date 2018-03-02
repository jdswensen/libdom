#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <dom.h>
#include "readable.h"

char loadf_path_str[PATH_MAX] = "";
FILE *input_html_file = NULL;

char output_path_str[PATH_MAX] = "";
FILE *output_stream = NULL;

void usage ( void );

void usage ( void )
{
    printf("%s\n", "USAGE: html2stream -i [input.html]");
    printf("\n");
    printf("%s\n", "COMMANDS");
    printf("\t%s\t%s\n", "-i", "input file");
    printf("\t%s\t%s\n", "-o", "output file (omit for stdout)");
}

int main (int argc, char **argv)
{
    dom_node_t *root_doc = NULL;
    int32_t i = 0;

    for ( i = 1; i < argc; ++i )
    {
        if ( 0 == strcmp(argv[i], "-i"))
        {
            if ( argv[i + 1] != NULL )
            {
                strncpy(loadf_path_str, argv[i + 1], 64);
                if ( access(loadf_path_str, F_OK) != -1 )
                {
                    input_html_file = fopen(loadf_path_str, "r");
                }
                i++;
            }
        }

        if ( 0 == strcmp(argv[i], "-o"))
        {

            if ( argv[i + 1] != NULL )
            {
                strncpy(output_path_str, argv[i + 1], 64);
                output_stream = fopen(output_path_str, "w");
                i++;
            }
        }
    }

    if ( input_html_file == NULL )
    {
        fprintf(stderr, "%s\n", "example-readable-html: no input file specified");
        usage();
        exit(0);
    }

    if ( output_stream == NULL )
    {
        output_stream = stdout;
    }

    root_doc = dom_loadf(input_html_file);
    setup_readable(root_doc);
    dom_node_print_readable(root_doc, output_stream);
    dom_node_free(root_doc);

    fclose(input_html_file);
    fclose(output_stream);

    return 0;
}
