#include "minijson.h"

static const char *ep;

static const char *skip(const char *in) {
	while (in && *in && (unsigned char)*in <= 32) in++;
	return in;
}

static const char *parse_string(const char *str, char **out, int *out_len) {
	const char *ptr = str + 1;
	int len = 0;
	if (*str != '\"') {ep = str; return 0;}
	while (*ptr != '\"' && *ptr && ++len) {
		if (*ptr++ == '\\') ptr++;
	}
	if (*ptr == '\"') ptr++;
	*out = (char*)(str + 1);
	*out_len = len;
	return ptr;
}

static const char *parse_number(const char *str, char **out, int *out_len) {
	int len = 0;
	if (!(*str == '-' || (*str >= '0' && *str <= '9'))) {ep = str; return 0;}
	*out = (char*)str;
	if (*str == '-') {len++; str++;}
	while (*str >= '0' && *str <= '9') {len++; str++;}
	*out_len = len;
	return str;
}

static const char *parse_value(const char *str, char **out, int *out_len, json_type_t *type) {
	if (!str) return 0;
	
	if (*str == '\"') {
		*type = json_type_string;
		return parse_string(str, out, out_len);
	}
	else if (*str == '-' || (*str >= '0' && *str <= '9')) {
		*type = json_type_number;
		return parse_number(str, out, out_len);
	}

	*type = json_type_error;
	ep = str;
	return 0;
}

const char *json_parse_object(const char *str, const char *key, char **value, int *value_len, json_type_t *type) {
	const char *ptr = skip(str);
	bool match = false;
	int key_len = strlen(key);

	if (*ptr != '{') {ep = ptr; return 0;}
	ptr = skip(ptr + 1);
	if (*ptr == '}') return ptr + 1;

	ptr = skip(parse_string(skip(ptr), value, value_len));
	if (!ptr) return 0;
	if (*value_len == key_len && !strncmp(*value, key, key_len)) match = true;

	if (*ptr != ':') {ep = ptr; return 0;}
	ptr = skip(parse_value(skip(ptr + 1), value, value_len, type));
	if (!ptr || match) return ptr;

	while (*ptr == ',') {
		ptr = skip(parse_string(skip(ptr + 1), value, value_len));
		if (!ptr) return 0;
		if (*value_len == key_len && !strncmp(*value, key, key_len)) match = true;

		if (*ptr != ':') {ep = ptr; return 0;}
		ptr = skip(parse_value(skip(ptr + 1), value, value_len, type));
		if (!ptr || match) return ptr;
	}

	if (!match) {
		*value = 0;
		*value_len = 0;
		*type = json_type_error;
	}
	
	return 0;
}

const char *json_get_ep() {
	return ep;
}