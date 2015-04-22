#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define JSON_Number  1
#define JSON_String  2

static const char *ep;

static bool _strncpy(char *des, int size, const char *src, int n) {
	if (size - 1 < n) {
		return false;
	}
	memcpy(des, src, n);
	des[n] = 0;
	return true;
}

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
	if (!(*str == '-' || (*str >= '0') && (*str <= '9'))) {ep = str; return 0;}
	*out = (char*)str;
	if (*str == '-') {len++; str++;}
	while (*str >= '0' && *str <= '9') {len++; str++;}
	*out_len = len;
	return str;
}

static const char *parse_value(const char *str, char **out, int *out_len, int *type) {
	if (!str) return 0;
	
	if (*str == '\"') {
		*type = JSON_String;
		return parse_string(str, out, out_len);
	}
	else if (*str == '-' || (*str >= '0') && (*str <= '9')) {
		*type = JSON_Number;
		return parse_number(str, out, out_len);
	}

	ep = str;
	return 0;
}

static const char *parse_object(const char *str, const char *key, char **out, int *out_len, int *type) {
	const char *ptr = skip(str);
	bool match = false;
	int key_len = strlen(key);
	*out = *out_len = *type = 0;

	if (*ptr != '{') {ep = ptr; return 0;}
	ptr = skip(ptr + 1);
	if (*ptr == '}') return ptr + 1;

	ptr = skip(parse_string(skip(ptr), out, out_len));
	if (!ptr) return 0;
	if (*out_len == key_len && !strncmp(*out, key, key_len)) match = true;

	if (*ptr != ':') {ep = ptr; return 0;}
	ptr = skip(parse_value(skip(ptr + 1), out, out_len, type));
	if (!ptr) return 0;
	if (match) return ptr;

	while (*ptr == ',') {
		ptr = skip(parse_string(skip(ptr + 1), out, out_len));
		if (!ptr) return 0;
		if (*out_len == key_len && !strncmp(*out, key, key_len)) match = true;

		if (*ptr != ':') {ep = ptr; return 0;}
		ptr = skip(parse_value(skip(ptr + 1), out, out_len, type));
		if (!ptr) return 0;
		if (match) return ptr;
	}
}

int main() {
	const char *text = "{ \"number\" : -001234, \"name\": \"lijie\", \"language\" : \"c\"}";

	char *out = 0, value[100] = {0};
	int out_len = 0, type = 0;

	parse_object(text, "number", &out, &out_len, &type);
	_strncpy(value, sizeof(value), out, out_len);
	printf("value[%s] type[%d]\n", value, type);

	parse_object(text, "name", &out, &out_len, &type);
	_strncpy(value, sizeof(value), out, out_len);
	printf("value[%s] type[%d]\n", value, type);

	parse_object(text, "language", &out, &out_len, &type);
	_strncpy(value, sizeof(value), out, out_len);
	printf("value[%s] type[%d]\n", value, type);

	return 0;
}