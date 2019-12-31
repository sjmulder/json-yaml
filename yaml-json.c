#include <stdio.h>
#include <stdlib.h>
#include <yajl/yajl_gen.h>
#include <yaml.h>

static void
print_cb(void *ctx, const char *str, size_t len)
{
	(void)ctx;
	fwrite(str, len, 1, stdout);
}

static int
is_num(char *s, size_t len)
{
	size_t i = 0;

	if (!len)
		return 0;

	/* sign */
	if (s[i] == '-')
		if (++i >= len)
			return 0;
	
	/* number */
	if (s[i] == '0') {
		if (++i >= len)
			return 0;
	} else if (s[i] >= '1' && s[i] <= '9') {
		if (++i >= len)
			return 1;
		while (s[i] >= '0' && s[i] <= '9')
			if (++i >= len)
				return 1;
	} else
		return 0;
	
	/* fraction */
	if (s[i] == '.') {
		if (++i >= len)
			return 0;
		while (s[i] >= '0' && s[i] <= '9')
			if (++i >= len)
				return 1;
	}

	/* exponent */
	if (s[i] == 'e' || s[i] == 'E') {
		if (++i >= len)
			return 0;
		if (s[i] == '+' || s[i] == '-')
			if (++i >= len)
				return 0;
		while (s[i] >= '0' && s[i] <= '9')
			if (++i >= len)
				return 1;
	}

	return 0;
}

static yajl_gen_status
gen_scalar(yajl_gen gen, yaml_event_t *event)
{
	unsigned char *val;
	size_t len;

	val = event->data.scalar.value;
	len = event->data.scalar.length;

	if (event->data.scalar.quoted_implicit)
		return yajl_gen_string(gen, val, len);
	else if (!strncmp("null", (char *)val, len))
		return yajl_gen_null(gen);
	else if (!strncmp("true", (char *)val, len))
		return yajl_gen_bool(gen, 1);
	else if (!strncmp("false", (char *)val, len))
		return yajl_gen_bool(gen, 0);
	else if (is_num((char *)val, len))
		return yajl_gen_number(gen, (char *)val, len);
	else
		return yajl_gen_string(gen, val, len);
}

int
main(int argc, char **argv)
{
	FILE *file;
	yaml_parser_t parser;
	yaml_event_t event;
	yajl_gen gen;
	yajl_gen_status status;
	int ok;
	
	if (argc < 2)
		file = stdin;
	else if (argc > 2 || argv[1][0] == '-') {
		fprintf(stderr, "usage: yaml-json [filename]\n");
		exit(1);
	} else if (!(file = fopen(argv[1], "r"))) {
		perror("json-yaml");
		exit(1);
	}

	if (!yaml_parser_initialize(&parser)) {
		fprintf(stderr, "yaml-json: failed to initialize "
		    "parser\n");
		exit(1);
	}

	yaml_parser_set_input_file(&parser, file);

	if (!(gen = yajl_gen_alloc(NULL))) {
		fprintf(stderr, "yaml-json: failed to initialize "
		    "generator\n");
		exit(1);
	}

	ok = yajl_gen_config(gen, yajl_gen_beautify, 1) &&
	     yajl_gen_config(gen, yajl_gen_indent_string, "  ") &&
	     yajl_gen_config(gen, yajl_gen_print_callback, print_cb,
	        NULL);
	if (!ok) {
		fprintf(stderr, "yaml-json: failed to configure "
		    "generator\n");
		exit(1);
	}

	while (1) {
		if (!yaml_parser_parse(&parser, &event)) {
			fprintf(stderr, "yaml-json: failed to parse "
			    "event\n");
			exit(1);
		}

		switch (event.type) {
		case YAML_STREAM_START_EVENT:
		case YAML_DOCUMENT_START_EVENT:
		case YAML_DOCUMENT_END_EVENT:
			status = yajl_gen_status_ok;
			break;

		case YAML_STREAM_END_EVENT:
			goto done;

		case YAML_SCALAR_EVENT:
			status = gen_scalar(gen, &event);
			break;

		case YAML_SEQUENCE_START_EVENT:
			status = yajl_gen_array_open(gen);
			break;

		case YAML_SEQUENCE_END_EVENT:
			status = yajl_gen_array_close(gen);
			break;

		case YAML_MAPPING_START_EVENT:
			status = yajl_gen_map_open(gen);
			break;

		case YAML_MAPPING_END_EVENT:
			status = yajl_gen_map_close(gen);
			break;

		case YAML_ALIAS_EVENT:
			fprintf(stderr, "yaml-json: aliases are not "
			    "yet supported\n");
			exit(1);

		default:
			fprintf(stderr, "yaml-json: unexpected event "
			    "type: %d\n", (int)event.type);
			exit(1);
		}

		if (status != yajl_gen_status_ok) {
			fprintf(stderr, "yaml-json: failed to emit "
			    "value\n");
			exit(1);
		}

		yaml_event_delete(&event);
	}

done:
	return 0;
}
