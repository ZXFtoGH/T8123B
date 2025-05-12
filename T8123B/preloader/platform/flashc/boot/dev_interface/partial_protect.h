#pragma once

struct protect_unit {
  unsigned int storage;
  unsigned int region;
  unsigned long long addr;
  unsigned long long length;
};

int check_register_read_permission(unsigned int address);

int check_register_write_permission(unsigned int address);

