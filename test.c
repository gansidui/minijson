#include <stdio.h>
#include "minijson.h"

void test(const char *text, const char *key) {
	char *value = 0, temp[100] = {0};
	int value_len = 0;
	json_type_t type = 0;

	json_parse_object(text, key, &value, &value_len, &type);

	if (type != json_type_error) {
		memcpy(temp, value, value_len);
		if (type == json_type_string) {
			printf("key[%s]  value[%s]  type[%d]\n", key, temp, type);
		}
		else if (type == json_type_number) {
			printf("key[%s]  value[%d]  type[%d]\n", key, atoi(temp), type);
		}
	}
	else {
		printf("key[%s]  ep[%s]  type[%d]\n", key, json_get_ep(), type);
	}
}

int main() {
	const char *text = "{ \"number\" : -001234, \"name\": \"lijie\", \"language\" : \"c\"}";
	test(text, "number");
	test(text, "name");
	test(text, "language");
	test(text, "failed");

	const char *text2 = "{ \"id\": abc, \"name\": \"lijie\" }";
	test(text2, "id");

	return 0;
}