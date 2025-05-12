#pragma once

/* super early platform initialization, before almost everything */
int platform_early_init(void);
int platform_init_da(void);
void dagent_register_commands();
       
