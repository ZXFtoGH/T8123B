#pragma once

int dev_efuse_write(const char* blow_data, int length);
int dev_efuse_read(char *read_data, int length);
