#ifndef __MATRIX_KEYS_H
#define __MATRIX_KEYS_H

#include "include.h"
#ifdef __cplusplus
extern "C" {
#endif

void matrix_keys_row_scan_init(void);
void matrix_keys_col_scan_init(void);
uint8_t matrix_keys_get_key_num(void);
void matrix_keys_operate(void);
void matrix_keys_task(void *param);
void do_matrix_keys_task(void);

#ifdef __cplusplus
}
#endif

#endif /* __MATRIX_KEYS_H */

