#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
    printf("Usage: %s -n -f <database file>\n", argv[0]);
    printf("\t -n - create new database file\n");
    printf("\t -f - (required) path to database file");
    return;
} 

int main(int argc, char *argv[]) {
    char *filepath = NULL;
    char *addstring = NULL;
    char *updatestring = NULL;
    char *deletestring = NULL;
    bool newfile = false;
    bool list = false;
    //getopt case - what we get from commandline
    int c;
    int dbfd = -1;
    struct dbheader_t *header = NULL;
    struct employee_t *employees = NULL;
    // f: and a: because the both have data coming after
    while ((c = getopt(argc, argv, "nf:a:lu:d:")) != -1) {
        switch (c) {
            case 'n':
                newfile = true;
                break;
            case 'f':
                filepath = optarg;
                break;
            case 'a':
                addstring = optarg;
                break;
            case 'l':
                list = true;
                break;
            case 'u':
                updatestring = optarg;
                break;
            case 'd':
                deletestring = optarg;
                break;
            case '?':
                printf("Unknown option -%c\n", c);
                break;
            default:
                return -1;
        }
    }

    if (filepath == NULL) {
        printf("Filepath is a required argument\n");
        print_usage(argv);
        return 0;
    }

    if (newfile) {
        dbfd = create_db_file(filepath);
        if (dbfd == STATUS_ERROR) {
            printf("Unable to create database file\n");
            return -1;
        }
        if (create_db_header(dbfd, &header) == STATUS_ERROR) {
            printf("Failed to create db header\n");
            return -1;
        }
    } else {
        dbfd = open_db_file(filepath);
        if (dbfd == STATUS_ERROR) {
            printf("Unable to open database file\n");
            return -1;
        }
        if (validate_db_header(dbfd, &header) == STATUS_ERROR) {
            printf("Failed to validate database header\n");
            return -1; 
        }
    }

    if(read_employees(dbfd, header, &employees) != STATUS_SUCCESS) {
        printf("Failed to read employees\n");
        return 0;
    }

    if (addstring) {
        header->count++;
        employees = realloc(employees, header->count*(sizeof(struct employee_t)));
        add_employee(header, employees, addstring);
    }

    if (updatestring) {
        update_employee(header, employees, updatestring);
    }

    if (deletestring) {
        remove_employee(header, &employees, deletestring);
    }

    if (list) {
        list_employees(header, employees);
    }

    output_file(dbfd, header, employees);

    return 0;
}
