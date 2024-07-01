#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

/*
Name: Manindranath Kommineni
BlazerId: mkommine
Project #: HW_02
To compile: make
To run: ./search
./search ../
./search /home
./search -S ../
./search -s 1024
./search -s 1024 ../
./search -f jpg 1
./search -f jpg 1 -s 1024
./search -s 1024 -f jpg 1
./search -S -f jpg 1 -s 1024
*/


typedef void (*ListDirFunc)(const char *, int, int, long, const char *, int, int, char);
typedef struct {
    char path[PATH_MAX];
    int depth;
} DirEntry;

void list_directory(const char *dir_name, int indentation, int show_attributes, long size_limit, const char *pattern, int depth_limit, int current_depth, char type_filter);

void list_directory_recursive(const char *dir_name, int indentation, int show_attributes, long size_limit, const char *pattern, int depth_limit, int current_depth, char type_filter) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char filepath[PATH_MAX];
    char link_target[PATH_MAX];
    ssize_t len;
    DirEntry stack[1000]; // Stack for directory entries, adjust size as needed
    int top = 0;

    if ((dir = opendir(dir_name)) == NULL) {
        perror("Unable to read directory");
        return;
    }

    printf("%*s%s:\n", indentation * 4, "", dir_name);
    strcpy(stack[top].path, dir_name);
    stack[top].depth = 0;
    top++;

    while (top > 0) {
        top--;
        strcpy(filepath, stack[top].path);
        int current_depth = stack[top].depth;

        if ((dir = opendir(filepath)) == NULL) {
            perror("Unable to read directory");
            continue;
        }

        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            snprintf(filepath, sizeof(filepath), "%s/%s", stack[top].path, entry->d_name);
            lstat(filepath, &file_stat);  // Use lstat to handle symbolic links

            if (S_ISDIR(file_stat.st_mode)) {
                if (type_filter == 'f') continue;  // Skip directories if file filter is active
                printf("%*s%s (directory)\n", (indentation + current_depth) * 4, "", entry->d_name);
                if (current_depth + 1 < depth_limit) {
                    if (top < 999) { // Ensure stack overflow protection
                        strcpy(stack[top].path, filepath);
                        stack[top].depth = current_depth + 1;
                        top++;
                    } else {
                        fprintf(stderr, "Stack overflow!\n");
                        closedir(dir);
                        return;
                    }
                }
            } else if (S_ISLNK(file_stat.st_mode)) {
                if (type_filter == 'd') continue;  // Skip symbolic links if directory filter is active
                len = readlink(filepath, link_target, sizeof(link_target) - 1);
                if (len != -1) {
                    link_target[len] = '\0';
                    printf("%*s%s (-> %s)\n", (indentation + current_depth) * 4, "", entry->d_name, link_target);
                }
            } else {
                if (type_filter == 'd') continue;  // Skip files if directory filter is active
                if ((size_limit > 0 && file_stat.st_size > size_limit) || (pattern && !strstr(entry->d_name, pattern))) {
                    continue;  // Skip files that don't match the pattern
                } else if (show_attributes) {
                    printf("%*s%s (size: %ld, permissions: %o, last access: %ld)\n",
                           (indentation + current_depth) * 4, "", entry->d_name, file_stat.st_size, file_stat.st_mode & 0777, file_stat.st_atime);
                } else {
                    printf("%*s%s\n", (indentation + current_depth) * 4, "", entry->d_name);
                }
            }
        }

        closedir(dir);
    }
}

int main(int argc, char *argv[]) {
    int opt;
    int show_attributes = 0;
    long size_limit = -1;
    char *pattern = NULL;
    int depth_limit = INT_MAX;
    const char *dir_name = ".";  // Default to current directory
    char type_filter = 0;  // 0 for no filter, 'f' for files, 'd' for directories

    ListDirFunc listFunc = list_directory_recursive;

    while ((opt = getopt(argc, argv, "Ss:f:t:")) != -1) {
       if (opt == 'S') {
           show_attributes = 1;
       }
       else if (opt == 's') {
    	   size_limit = atol(optarg);
	}
	else if (opt == 'f') {
    	    pattern = optarg;
    	    if (optind < argc && argv[optind][0] != '-') {
        	depth_limit = atoi(argv[optind]);
        	optind++;
    	    }
    	    else {
        	fprintf(stderr, "Depth not provided for -f option\n");
        	exit(EXIT_FAILURE);
    	    }
	}
	else if (opt == 't') {
    		if (strcmp(optarg, "f") == 0) {
        	type_filter = 'f';
    		}
    		else if (strcmp(optarg, "d") == 0) {
        		type_filter = 'd';
    		}
    		else {
        		fprintf(stderr, "Invalid argument for -t option. Use 'f' for files or 'd' for directories.\n");
        		exit(EXIT_FAILURE);
    		}
	}
	else {
		fprintf(stderr, "Usage: %s [-S] [-s size] [-f pattern depth] [-t f|d] [directory]\n", argv[0]);
    		exit(EXIT_FAILURE);
	}


    }

    if (optind < argc) {
        dir_name = argv[optind];

         // Resolve relative path such as '../' to its absolute path
        char resolved_path[PATH_MAX];
        if (realpath(dir_name, resolved_path) == NULL) {
            perror("Error resolving directory path");
            exit(EXIT_FAILURE);
        }
        dir_name = resolved_path;
    }

    listFunc(dir_name, 0, show_attributes, size_limit, pattern, depth_limit, 0, type_filter);

    return 0;
}
