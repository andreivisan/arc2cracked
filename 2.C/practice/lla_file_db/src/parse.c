#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "common.h"
#include "parse.h"

int create_db_header(int fd, struct dbheader_t **headerOut) {
    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    if (header == -1) {
        printf("Malloc failed to create db header\n");
        return STATUS_ERROR;
    }
    header->version = 0x1;
    header->count = 0;
    header->magic = HEADER_MAGIC;
    header->filesize = sizeof(struct dbheader_t);
    *headerOut = header;
    return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct dbheader_t **headerOut) {
    if (fd < 0) {
        printf("Got a bad FD from the user\n");
        return STATUS_ERROR;
    }
    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    if (header == -1) {
        printf("Malloc failed to create a db header\n");
        return STATUS_ERROR;
    }
    if (read(fd, header, sizeof(struct dbheader_t)) != sizeof(struct dbheader_t)) {
        perror("read");
        free(header);
        return STATUS_ERROR;
    }
    // ntohs = Network to Host Short uses the Endienness of the host machine
    // ntohl = Networl to Host Long
    header->version = ntohs(header->version);
    header->count = ntohs(header->count);
    header->magic = ntohl(header->magic);
    header->filesize = ntohl(header->filesize);
    if (header->magic != HEADER_MAGIC) {
        printf("Improper header version\n");
        free(header);
        return -1;
    }
    if (header->version != 1) {
        printf("Improper header version\n");
        free(header);
        return -1;
    }
    struct stat dbstat = {0};
    fstat(fd, &dbstat);
    if (header ->filesize != dbstat.st_size) {
        printf("Corrupted database\n");
        free(header);
        return -1;
    }
    *headerOut = header;
}

int output_file(int fd, struct dbheader_t *dbhdr, struct employee_t *employees) {
    if (fd < 0) {
        printf("Got a bad FD from the user\n");
        return STATUS_ERROR;
    }
    int realcount = dbhdr->count;
    // Host to Network Long
    dbhdr->magic = htonl(dbhdr->magic);
    dbhdr->filesize = htonl(sizeof(struct dbheader_t) + sizeof(struct employee_t) * realcount);
    dbhdr->count = htons(dbhdr->count);
    dbhdr->version = htons(dbhdr->version);
    lseek(fd, 0, SEEK_SET);
    write(fd, dbhdr, sizeof(struct dbheader_t));
    for (int i = 0; i < realcount; i++) {
        employees[i].hours = htonl(employees[i].hours);
        write(fd, &employees[i], sizeof(struct employee_t));
    }
    return 0;
}

int read_employees(int fd, struct dbheader_t *dbhdr, struct employee_t **employeesOut) {
    if (fd < 0) {
        printf("Got a bad FD from the user\n");
        return STATUS_ERROR;
    }
    int count = dbhdr->count;
    struct employee_t *employees = calloc(count, sizeof(struct employee_t));
    if (employees == -1) {
        printf("Malloc failed\n");
        return STATUS_ERROR;
    }
    read(fd, employees, count*sizeof(struct employee_t));
    int i = 0;
    for (; i < count; i++) {
        employees[i].hours = ntohl(employees[i].hours);
    }
    *employeesOut = employees;
    return STATUS_SUCCESS;
}

int add_employee(struct dbheader_t *dbhdr, struct employee_t *employees, char *addstring) {
    char *name = strtok(addstring, ",");
    char *addr = strtok(NULL, ",");
    char *hours = strtok(NULL, ",");
    strncpy(employees[dbhdr->count-1].name, name, sizeof(employees[dbhdr->count-1].name));
    strncpy(employees[dbhdr->count-1].address, addr, sizeof(employees[dbhdr->count-1].address));
    employees[dbhdr->count-1].hours = atoi(hours);
    return STATUS_SUCCESS;
}

int update_employee(struct dbheader_t *dbhdr, struct employee_t *employees, char *updatestring) {
    char *hours = strtok(updatestring, ",");
    char *new_name = strtok(NULL, ",");
    char *new_addr = strtok(NULL, ",");
    char *new_hours = strtok(NULL, ",");
    for (int i = 0; i < dbhdr->count; i++) {
        if (employees[i].hours == atoi(hours)) {
            strncpy(employees[i].name, new_name, sizeof(employees[i].name));
            strncpy(employees[i].address, new_addr, sizeof(employees[i].address));
            employees[i].hours = atoi(new_hours);
            return STATUS_SUCCESS;
        }
    }
    printf("Employee not found\n");
    return STATUS_ERROR;
}

int remove_employee(struct dbheader_t *dbhdr, struct employee_t **employeesOut, char *deletestring) {
    char *hours = strtok(deletestring, ",");
    struct employee_t *employees = *employeesOut;
    for (int i = 0; i < dbhdr->count; i++) {
        if (employees[i].hours == atoi(hours)) {
            for (int j = i; i < dbhdr->count - 1; j++) {
                employees[j] = employees[j+1];
            }
            dbhdr->count--;
            struct employee_t *newmem = realloc(employees, dbhdr->count*sizeof(struct employee_t));
            if (newmem == NULL && dbhdr->count > 0) {
                printf("Realloc failed\n");
                return STATUS_ERROR;
            }
            *employeesOut = newmem;
            return STATUS_SUCCESS;
        }
    }
    printf("Employee not found\n");
    return STATUS_ERROR; 
}

void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees) {
    int i = 0;
    for (; i < dbhdr->count; i++) {
        printf("Employee %d\n", i);
        printf("\tName: %s\n", employees[i].name);
        printf("\tAddress: %s\n", employees[i].address);
        printf("\tHours: %d\n", employees[i].hours);
    }
}