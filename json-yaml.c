#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <yajl/yajl_parse.h>
#include <yaml.h>

#define PROG_NAME	"json-yaml"
#define PROG_VER	"1.0.1"

static yajl_handle	g_yajl;
static bool		g_yaml_initialized = false;
static yaml_emitter_t	g_emitter;

static void print_help()
{
	printf("Usage: " PROG_NAME " [OPTIONS] [FILE]\n\n");
	printf("Convert JSON into YAML\n");
	printf("Uses standard input if no filename is supplied.\n\n");
	printf("Options:\n");
	printf(" -v, --version   print version number and exit\n");
	printf(" -h, --help      print this message and exit\n");
}

static void cleanup()
{
	if (g_yajl) {
		yajl_free(g_yajl);
		g_yajl = NULL;
	}

	if (g_yaml_initialized) {
		yaml_emitter_delete(&g_emitter);
		g_yaml_initialized = false;
	}
}

static void check_yajl(yajl_status status)
{
	if (status != yajl_status_ok) {
		const char* message = yajl_status_to_string(status);
		if (!message) {
			message = "no reason given";
		}
		fprintf(stderr, PROG_NAME ": error parsing JSON: %s\n",
				message);
		exit(EXIT_FAILURE);
	}
}

static void check_yaml(int status)
{
	if (status) {
		return;
	}

	switch (g_emitter.error) {
	case YAML_MEMORY_ERROR:
		fprintf(stderr, PROG_NAME ": error writing YAML: "
			"out of memory\n");
		break;
	
	case YAML_WRITER_ERROR:
	case YAML_EMITTER_ERROR:
		fprintf(stderr, PROG_NAME ": error writing YAML: %s\n",
			g_emitter.problem);
		break;

	default:
		fprintf(stderr, PROG_NAME ": error writing YAML\n");
		break;
	}
	
	exit(EXIT_FAILURE);
}

static int handle_null(void* ctx)
{
	yaml_event_t event;
	yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)"null",
			4, true, true, YAML_PLAIN_SCALAR_STYLE);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return true;
}

static int handle_boolean(void* ctx, int val)
{
	yaml_event_t event;
	if (val) {
		yaml_scalar_event_initialize(&event, NULL, NULL,
				(yaml_char_t*)"true", 4, true, true,
				YAML_ANY_SCALAR_STYLE);
	} else {
		yaml_scalar_event_initialize(&event, NULL, NULL,
				(yaml_char_t*)"false", 5, true, true,
				YAML_ANY_SCALAR_STYLE);
	}

	check_yaml(yaml_emitter_emit(&g_emitter, &event));
	return true;
}

static int handle_integer(void* ctx, long long val)
{
	char str[32];
	int num = snprintf(str, sizeof(str), "%lli", val);
	if (num >= sizeof(str)) {
		fprintf(stderr, PROG_NAME ": number too large: %lli\n", val);
		exit(EXIT_FAILURE);
	}

	yaml_event_t event;
	yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)str,
			(size_t)num, true, true, YAML_ANY_SCALAR_STYLE);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return true;
}

static int handle_double(void* ctx, double val)
{
	char str[32];
	int num = snprintf(str, sizeof(str), "%f", val);

	yaml_event_t event;
	yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)str,
			(size_t)num, true, true, YAML_ANY_SCALAR_STYLE);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return true;
}

static int handle_number(void* ctx, const char* str, size_t len)
{
	yaml_event_t event;
	yaml_scalar_event_initialize(&event, NULL, NULL,
			(yaml_char_t*)str, len, true, true,
			YAML_ANY_SCALAR_STYLE);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return true;
}

static int handle_string(void* ctx, const unsigned char* str, size_t len)
{
	yaml_event_t event;
	yaml_scalar_event_initialize(&event, NULL, NULL,
			(yaml_char_t*)str, len, true, true,
			YAML_ANY_SCALAR_STYLE);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return true;
}

static int handle_map_start(void* ctx)
{
	yaml_event_t event;
	yaml_mapping_start_event_initialize(&event, NULL, NULL, true,
			YAML_ANY_MAPPING_STYLE);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return true;
}

static int handle_map_key(void* ctx, const unsigned char* str, size_t len)
{
	yaml_event_t event;
	yaml_scalar_event_initialize(&event, NULL, NULL,
			(yaml_char_t*)str, len, true, true,
			YAML_ANY_SCALAR_STYLE);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return true;
}

static int handle_map_end(void* ctx)
{
	yaml_event_t event;
	yaml_mapping_end_event_initialize(&event);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return true;
}

static int handle_array_start(void* ctx)
{
	yaml_event_t event;
	yaml_sequence_start_event_initialize(&event, NULL, NULL, false,
			YAML_ANY_SEQUENCE_STYLE);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return true;
}

static int handle_array_end(void* ctx)
{
	yaml_event_t event;
	yaml_sequence_end_event_initialize(&event);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return true;
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

int main(int argc, const char* argv[])
{
	atexit(cleanup);

	const char* filename = NULL;

	for (int i = 1; i < argc; i++) {
		const char* arg = argv[i];
		if (strcmp("-v", arg) == 0 ||
				strcmp("--version", arg) == 0) {
			printf(PROG_VER "\n");
			exit(EXIT_SUCCESS);
		} else if (strcmp("-h", arg) == 0 ||
				strcmp("--help", arg) == 0) {
			print_help();
			exit(EXIT_SUCCESS);
		} else if (arg[0] == '-') {
			fprintf(stderr, PROG_NAME ": unknown option: %s "
				"(try -h)\n", arg);
			exit(EXIT_FAILURE);
		} else if (filename) {
			fprintf(stderr, PROG_NAME ": multiple filenames "
				"given (try -h)\n");
			exit(EXIT_FAILURE);
		} else {
			filename = arg;
		}
	}
	
	FILE* file = stdin;
	if (filename) {
		file = fopen(filename, "r");
		if (!file) {
			perror(PROG_NAME);
			exit(EXIT_FAILURE);
		}
	}

	yaml_emitter_initialize(&g_emitter);
	yaml_emitter_set_output_file(&g_emitter, stdout);

	g_yaml_initialized = true;

	yaml_event_t event;
	yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	yaml_document_start_event_initialize(&event, NULL, NULL, NULL, true);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));
	
	yajl_handle yajl = yajl_alloc(&callbacks, NULL, NULL);

	unsigned char buf[4096];
	size_t num;
	while ((num = fread(buf, 1, sizeof(buf), file)) > 0) {
		check_yajl(yajl_parse(yajl, buf, num));
	}

	if (ferror(file)) {
		perror(PROG_NAME);
		exit(EXIT_FAILURE);
	}

	check_yajl(yajl_complete_parse(yajl));

	yaml_document_end_event_initialize(&event, true);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	yaml_stream_end_event_initialize(&event);
	check_yaml(yaml_emitter_emit(&g_emitter, &event));

	return 0;
}
