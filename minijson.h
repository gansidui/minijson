#ifndef __MINI_JSON_H__
#define __MINI_JSON_H__

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	json_type_error = 0,
	json_type_number,
	json_type_string,
} json_type_t;

const char *json_parse_object(const char *str, const char *key, char **value, int *value_len, json_type_t *type);

const char *json_get_ep();


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __MINI_JSON_H__ */