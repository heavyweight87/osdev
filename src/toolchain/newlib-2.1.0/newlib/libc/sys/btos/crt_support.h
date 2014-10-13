#ifndef _CRT_SUPPORT_H
#define _CRT_SUPPORT_H

bool btos_path_parse(const char *opath, char *buffer, size_t size);
bool btos_path_is_absolute(const char *path);
size_t btos_get_drive_end(const char *path);
int btos_set_filenum(bt_handle fh);
bt_handle btos_get_handle(int fd);
void btos_set_specific_filenum(int fd, bt_handle fh);
void btos_remove_filenum(int fd);
void btos_open_std_streams();

#endif