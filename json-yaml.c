#include <stdio.h>
#include <string.h>
#include <err.h>
#include <yajl/yajl_parse.h>
#include <yaml.h>

#define VERSION "1.1.1"

const char usage[] =
    "json-yaml " VERSION "\n"
    "usage: json-yaml [filename]\n";

static yajl_handle	g_yajl;
static yaml_emitter_t	g_emitter;

static
void check_yajl(yajl_status status)
{
	const char *message;

	if (status == yajl_status_ok)
		return;

	if ((message = yajl_status_to_string(status)))
		errx(1, "error parsing JSON: %s", message);
	else
		errx(1, "error parsing JSON");

}

static
void check_yaml(int status)
{
	if (status)
		return;

	switch (g_emitter.error) {
	case YAML_MEMORY_ERROR:
		errx(1, "error writing YAML: out of memory");

	case YAML_WRITER_ERROR:
	case YAML_EMITTER_ERROR:
		errx(1, "error writing YAML: %s", g_emitter.problem);

	default:
		errx(1, "error writing YAML");
	}
}

static int
handle_null(void *ctx)
{
	yaml_event_t event;

	(void)ctx;

	yaml_scalar_event_initialize(&event, NULL, NULL,
	    (yaml_char_t *)"null", 4, 1, 1, YAML_PLAIN_SCALAR_STYLE);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return 1;
}

static int
handle_boolean(void *ctx, int val)
{
	yaml_event_t event;

	(void)ctx;

	if (val) {
		yaml_scalar_event_initialize(&event, NULL, NULL,
		    (yaml_char_t*)"true", 4, 1, 1,
		    YAML_ANY_SCALAR_STYLE);
	} else {
		yaml_scalar_event_initialize(&event, NULL, NULL,
		    (yaml_char_t*)"false", 5, 1, 1,
		    YAML_ANY_SCALAR_STYLE);
	}

	check_yaml(yaml_emitter_emit(&g_emitter, &event));
	return 1;
}

static int
handle_integer(void *ctx, long long val)
{
	char str[32];
	yaml_event_t event;
	int num;

	(void)ctx;

	num = snprintf(str, sizeof(str), "%lli", val);
	if (num < 0)
		err(1, NULL);
	else if ((size_t)num >= sizeof(str))
		errx(1, "number too large: %lli", val);

	yaml_scalar_event_initialize(&event, NULL, NULL,
	    (yaml_char_t *)str, (size_t)num, 1, 1,
	    YAML_ANY_SCALAR_STYLE);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return 1;
}

static int
handle_double(void *ctx, double val)
{
	char str[32];
	yaml_event_t event;

	(void)ctx;

	int num = snprintf(str, sizeof(str), "%f", val);

	yaml_scalar_event_initialize(&event, NULL, NULL,
	    (yaml_char_t*)str, (size_t)num, 1, 1,
	    YAML_ANY_SCALAR_STYLE);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return 1;
}

static int
handle_number(void *ctx, const char *str, size_t len)
{
	yaml_event_t event;

	(void)ctx;

	yaml_scalar_event_initialize(&event, NULL, NULL,
	    (yaml_char_t*)str, len, 1, 1, YAML_ANY_SCALAR_STYLE);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return 1;
}

static int
handle_string(void *ctx, const unsigned char *str, size_t len)
{
	yaml_event_t event;

	(void)ctx;

	yaml_scalar_event_initialize(&event, NULL, NULL,
	    (yaml_char_t*)str, len, 1, 1, YAML_ANY_SCALAR_STYLE);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return 1;
}

static int
handle_map_start(void *ctx)
{
	yaml_event_t event;

	(void)ctx;

	yaml_mapping_start_event_initialize(&event, NULL, NULL, 1,
	    YAML_ANY_MAPPING_STYLE);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return 1;
}

static int
handle_map_key(void *ctx, const unsigned char *str, size_t len)
{
	yaml_event_t event;

	(void)ctx;

	yaml_scalar_event_initialize(&event, NULL, NULL,
	    (yaml_char_t*)str, len, 1, 1, YAML_ANY_SCALAR_STYLE);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return 1;
}

static int
handle_map_end(void *ctx)
{
	yaml_event_t event;

	(void)ctx;

	yaml_mapping_end_event_initialize(&event);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return 1;
}

static int
handle_array_start(void *ctx)
{
	yaml_event_t event;

	(void)ctx;

	yaml_sequence_start_event_initialize(&event, NULL, NULL, 0,
	    YAML_ANY_SEQUENCE_STYLE);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return 1;
}

static int
handle_array_end(void *ctx)
{
	yaml_event_t event;

	(void)ctx;

	yaml_sequence_end_event_initialize(&event);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return 1;
}

static const yajl_callbacks callbacks = {
	handle_null,
	handle_boolean,
	handle_integer,
	handle_double,
	handle_number,
	handle_string,
	handle_map_start,
	handle_map_key,
	handle_map_end,
	handle_array_start,
	handle_array_end
};

int
main(int argc, const char **argv)
{
	FILE *file;
	yaml_event_t event;;
	unsigned char buf[4096];
	size_t num;

	if (argc < 2)
		file = stdin;
	else if (argc > 2 || argv[1][0] == '-') {
		fputs(usage, stderr);
		return 1;
	} else if (!(file = fopen(argv[1], "r")))
		err(1, NULL);

	yaml_emitter_initialize(&g_emitter);
	yaml_emitter_set_output_file(&g_emitter, stdout);

	yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	yaml_document_start_event_initialize(&event, NULL, NULL, NULL,
	    1);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));
	
	g_yajl = yajl_alloc(&callbacks, NULL, NULL);

	while ((num = fread(buf, 1, sizeof(buf), file)) > 0)
		check_yajl(yajl_parse(g_yajl, buf, num));

	if (ferror(file))
		err(1, NULL);

	check_yajl(yajl_complete_parse(g_yajl));

	yaml_document_end_event_initialize(&event, 1);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	yaml_stream_end_event_initialize(&event);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return 0;
}
