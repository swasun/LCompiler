/*************************************************************************************
 * MIT License                                                                       *
 *                                                                                   *
 * Copyright (C) 2016 Charly Lamothe, Stéphane Arcellier                             *
 *                                                                                   *
 * This file is part of LCompiler.                                                   *
 *                                                                                   *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy    *
 *   of this software and associated documentation files (the "Software"), to deal   *
 *   in the Software without restriction, including without limitation the rights    *
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       *
 *   copies of the Software, and to permit persons to whom the Software is           *
 *   furnished to do so, subject to the following conditions:                        *
 *                                                                                   *
 *   The above copyright notice and this permission notice shall be included in all  *
 *   copies or substantial portions of the Software.                                 *
 *                                                                                   *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
 *   SOFTWARE.                                                                       *
 *************************************************************************************/

#include "../headers/utils.h"
#include "../headers/stacktrace.h"
#include "../headers/error.h"
#include "../headers/alloc.h"
#include "../headers/check_parameter.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#elif defined(__unix__)
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <dirent.h>
    #include <sys/time.h>
#else
    #error "OS not supported"
#endif

bool last_char_is(char *str, char c) {
    return (!str || str[strlen(str) - 1] != c) ? false : true;
}

void remove_first_char(char *str) {
   if (str && strlen(str) > 1) {
      memmove(str, str+1, strlen(str));
   }
}

void remove_last_char(char *str) {
    size_t str_size;

    str_size = strlen(str);

   if (str && str_size > 1) {
      memmove(str, str, strlen(str) - 1);
      str[str_size - 1] = 0;
   }
}

char *string_create_from(const char *str) {
   char *new_str;

   SAFE_ALLOC(new_str, char, strlen(str) + 1)
   strcpy(new_str, str);

   return new_str;
}

const char *get_file_name_extension(const char *file_name) {
    const char *dot = strrchr(file_name, '.');

    if (!dot || dot == file_name) {
        return "";
    }

    return dot + 1;
}

int get_digits_number(int number) {
   int digits;

   if (number == 0) {
      return 1;
   }

   digits = floor(log10(abs(number))) + 1;

   if (digits == 0) {
      return 1;
   }
}

char *create_dump_file_name(const char *source_file_name, char *type) {
   char *dump_file_name;

   SAFE_ALLOC(dump_file_name, char, (strlen(source_file_name) + 1) * sizeof(char));
   strcpy(dump_file_name, source_file_name);
   remove_last_char(dump_file_name);
   SAFE_REALLOC(dump_file_name, char, strlen(dump_file_name), strlen(type) + 1)
   strcat(dump_file_name, type);
   return dump_file_name;
}

char *get_file_name_from_path(char *path) {
    char *file_name, *tmp_file_name;

    if (!strstr(path, "/")) {
        SAFE_ALLOC(file_name, char, strlen(path) + 1)
        strcpy(file_name, path);
        return file_name;
    }

    tmp_file_name = strrchr(path, '/');
    SAFE_ALLOC(file_name, char, strlen(tmp_file_name + 1) + 1)
    strcpy(file_name, tmp_file_name + 1);

    return file_name;
}

bool is_file_exists(const char *file_name) {
    #if defined(__unix__)
        struct stat st;
    #elif defined(_WIN32) || defined(_WIN64)
        DWORD dw_attrib;
    #else
        #error "OS not supported"
    #endif

    #if defined(__unix__)
        if (stat(file_name, &st) == 0) {
            return S_ISREG(st.st_mode);
        }
        return false;
    #elif defined(_WIN32) || defined(_WIN64)
        dw_attrib = GetFileAttributesA(file_name);
        if (dw_attrib != INVALID_FILE_ATTRBIUTES &&
            dw_attrib != FILE_ATTRBIUTE_DIRECTORY) {
            return true;
        }
    #endif

    return false;
}

bool is_dir_exists(const char *file_name) {
    #if defined(_WIN32) || defined(_WIN64)
        DWORD dw_attrib;
    #else
        DIR *dir;
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        dw_attrib = GetFileAttributesA(file_name);
        if (dw_attrib != INVALID_FILE_ATTRBIUTES &&
            dw_attrib & FILE_ATTRBIUTE_DIRECTORY) {
            return true;
        }
    #elif defined(__unix__)
        dir = opendir(file_name);
        if (dir) {
            closedir(dir);
            return true;
        }
    #else
        #error "OS not supported"
    #endif

    return false;
}

unsigned short count_dir_files(const char *dir_name, bool recursively) {
    char path[2048];
    unsigned short files;

    CHECK_PARAMETER_OR_RETURN(dir_name)

    #if defined(_WIN32) || defined(_WIN64)
        WIN32_FIND_DATA fd_file;
        HANDLE file_handle;
    #elif defined(__unix__)
        DIR *d;
        struct dirent *dir;
    #else
        #error "OS not supported"
    #endif

    files = 0;

    #if defined(__unix__)
        d = opendir(dir_name);
        if (!d) {
            PUSH_STACK_ERRNO()
            return -1;
        }

        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 &&
                strcmp(dir->d_name, "..") != 0) {
                memset(path, 0, 2048 * sizeof(char));
                strcat(path, dir_name);
                strcat(path, "/");
                strcat(path, dir->d_name);

                if (is_dir_exists(path) && recursively) {
                    files += count_dir_files(path, true);
                } else if (is_file_exists(path)) {
                    files++;
                }
            }
        }

        closedir(d);
    #elif defined(_WIN32) || defined(_WIN64)
        file_handle = NULL;

        /* Specify a file mask */
        sprintf(path, "%s\\*.*", dir_name);

        if((file_handle = FindFirstFile(path, &fd_file)) == INVALID_HANDLE_VALUE) {
            PUSH_STACK_MSG("Failed to get first file")
            return -1;
        }

        files++;

        do {
            /*
               Find first file will always return "."
               and ".." as the first two directories.
            */
            if(strcmp(fd_file.cFileName, ".") != 0 &&
               strcmp(fd_file.cFileName, "..") != 0) {
                /* Build up our file path */
                sprintf(path, "%s\\%s", dir, fd_ile.cFileName);

                /* Is the entity a file or folder ? */
                if(fd_file.dwFileAttributes &FILE_ATTRBIUTE_DIRECTORY && recursively) {
                    files += count_dir_files(path, true);
                } else if (is_file_exists(path)) {
                    files++;
                }
            }
        }
        while(FindNextFile(file_handle, &fd_file)); /* Find the next file. */

        FindClose(hFind);
    #else
        #error "OS not supported"
    #endif

    return files;
}

char **list_directory(char *dir_name, unsigned short *files, bool recursively) {
    char **file_names, **new_folder_files, path[2048], slash;
    unsigned short i, j, files_count, new_folder_files_count;

    CHECK_PARAMETER_OR_RETURN(dir_name)

    #if defined(__unix__)
        DIR *d;
        struct dirent *dir;
    #elif defined(_WIN32) || defined(_WIN64)
        WIN32_FIND_DATA fd_file;
        HANDLE file_handle;
    #else
        #error "OS not supported"
    #endif

    slash = ' ';

    #if defined(__unix__)
        slash = '/';
    #elif defined(_WIN32) || defined(_WIN64)
        slash = '\\';
    #else
        #error "OS not supported"
    #endif

    if (last_char_is(dir_name, slash)) {
        remove_last_char(dir_name);
    }

    files_count = count_dir_files(dir_name, recursively);
    if (files_count == (unsigned short)-1) {
        PUSH_STACK_MSG("Failed to count dir files")
        return NULL;
    } else if (files_count == 0) {
        return NULL;
    }

    i = 0;

    #if defined(__unix__)
        d = opendir(dir_name);
        if (!d) {
            PUSH_STACK_ERRNO()
            return NULL;
        }

        SAFE_ALLOC(file_names, char*, files_count)

        if (errno == ENOMEM || !file_names) {
            SAFE_FREE(file_names);
            closedir(d);
            return NULL;
        }

        while ((dir = readdir(d)) != NULL) {
            //memset(path, 0, 2048 * sizeof(char));
            strcpy(path, dir_name);

            if (strcmp(dir->d_name, ".") != 0 &&
                strcmp(dir->d_name, "..") != 0) {
                strcat(path, "/");
                strcat(path, dir->d_name);

                if (is_dir_exists(path) && recursively) {
                    new_folder_files = list_directory(path, &new_folder_files_count, true);
                    if (new_folder_files) {
                        for (j = 0; j < new_folder_files_count; j++) {
                            if (new_folder_files[j]) {
                                if (files_count + 1 > i) {
                                    SAFE_REALLOC(file_names, char*, files_count, 1);
                                    files_count++;
                                }
                                SAFE_ALLOC(file_names[i], char, strlen(new_folder_files[j]) + 1)
                                strcpy(file_names[i], new_folder_files[j]);
                                i++;
                            }
                        }
                        for (j = 0; j < new_folder_files_count; j++) {
                            SAFE_FREE(new_folder_files[j]);
                        }
                        SAFE_FREE(new_folder_files);
                    }
                } else if (is_file_exists(path)) {
                    if (files_count + 1 > i) {
                        SAFE_REALLOC(file_names, char*, files_count, 1);
                        files_count++;
                    }
                    SAFE_ALLOC(file_names[i], char, strlen(path) + 1)
                    strcpy(file_names[i], path);
                    i++;
                }
            }
        }

        closedir(d);
    #elif defined(_WIN32) || defined(_WIN64)
        file_handle = NULL;

        /* Specify a file mask */
        sprintf(path, "%s\\*.*", dir_name);

        TRY_OR_RETURN_MSG(
            (file_handle = FindFirstFile(path, &fd_file)) != INVALID_HANDLE_VALUE,
            "Failed to get first file"
        )

        SAFE_ALLOC(file_names, char*, files_count)

        do {
            /*
               Find first file will always return "."
               and ".." as the first two directories.
            */
            if(strcmp(fd_file.cFileName, ".") != 0 &&
               strcmp(fd_file.cFileName, "..") != 0) {
                /* Build up our file path */
                sprintf(path, "%s\\%s", dir, fd_file.cFileName);

                /* Is the entity a file or folder ? */
                if(fd_file.dwFileAttributes &FILE_ATTRBIUTE_DIRECTORY && recursively) {
                    new_folder_files = list_directory(path, &new_folder_files_count, true);
                    if (new_folder_files) {
                        for (j = 0; j < new_folder_files_count; j++) {
                            SAFE_ALLOC(file_names[i], char, strlen(new_folder_files[j]) + 1)
                            strcpy(file_names[i], new_folder_files[j]);
                            i++;
                        }
                        for (j = 0; j < new_folder_files_count; j++) {
                            SAFE_FREE(new_folder_files[j]);
                        }
                        SAFE_FREE(new_folder_files);
                    }
                } else if (is_file_exists(path)) {
                    SAFE_ALLOC(file_names[i], char, strlen(path) + 1)
                    file_names[i] = path;
                    i++;
                }
            }
        }
        while(FindNextFile(file_handle, &fd_file)); /* Find the next file. */

        FindClose(hFind);
    #else
        #error "OS not supported"
    #endif

    *files = i;

    return file_names;
}
