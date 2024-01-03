#define DEF_PROGRAM_NAME "bsum"
#define DEF_AUTHOR "Marcin Strągowski <marcin.stragowski@gmail.com>"
#define DEF_LICENSE ""


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

long parse_size(const char *size_str) {
		char *end;
		long size = strtol(size_str, &end, 10);
		switch (*end) {
				case 'T': case 't': return size * 1024 * 1024 * 1024 * 1024;
				case 'G': case 'g': return size * 1024 * 1024 * 1024;
				case 'M': case 'm': return size * 1024 * 1024;
				case 'K': case 'k': return size * 1024;
				default: return size;
		}
}

void print_sha512(unsigned char *hash, int block_num, int total_blocks) {
		fprintf(stdout, "%d/%d: ", block_num, total_blocks);
		for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
				fprintf(stdout, "%02x", hash[i]);
		fprintf(stdout, "\n");
		fflush(stdout);
}

void display_help(const char *program_name) {
		fprintf(stderr, "%s, Block device segments sha512sum printer, Author: %s\n", DEF_PROGRAM_NAME, DEF_AUTHOR);
		fprintf(stderr, "Usage: %s [--progress] [--blocksize <size>] [--device <file>]\n", program_name);
		fprintf(stderr, "  --progress\t\tOptional flag to display progress percentage.\n");
		fprintf(stderr, "  --blocksize <size>\tBlock size to read. Specify the size in bytes, or use suffixes 'K', 'M', 'G', 'T'. Default is 1G.\n");
		fprintf(stderr, "  --device <file>\tFile or device to read.\n");
}

int main(int argc, char *argv[]) {
		int progress_flag = 0;
		long block_size = 1024 * 1024 * 1024; // Default block size 1GB
		char *file_name = NULL;

		for (int i = 1; i < argc; i++) {
				if (strcmp(argv[i], "--progress") == 0) {
						progress_flag = 1;
				} else if (strcmp(argv[i], "--blocksize") == 0 && i + 1 < argc) {
						block_size = parse_size(argv[++i]);
				} else if (strcmp(argv[i], "--device") == 0 && i + 1 < argc) {
						file_name = argv[++i];
				} else {
						display_help(argv[0]);
						return 1;
				}
		}

		if (!file_name) {
				fprintf(stderr, "File not specified.\n");
				display_help(argv[0]);
				return 1;
		}

		FILE *file = fopen(file_name, "rb");
		if (!file) {
				perror("Error opening file");
				return 1;
		}

		// Determine the total number of blocks
		fseek(file, 0, SEEK_END);
		long file_size = ftell(file);
		fseek(file, 0, SEEK_SET);
		int total_blocks = (file_size + block_size - 1) / block_size;

		unsigned char *buffer = (unsigned char *)malloc(block_size);
		if (!buffer) {
				fprintf(stderr, "Memory allocation failed.\n");
				fclose(file);
				return 1;
		}

		unsigned char hash[SHA512_DIGEST_LENGTH];
		int block_num = 0;
		size_t bytes_read;
		long total_bytes_read = 0;

		while ((bytes_read = fread(buffer, 1, block_size, file)) > 0) {
				total_bytes_read += bytes_read;
				if (progress_flag) {
						int progress = (int)((total_bytes_read * 100.0) / file_size);
						fprintf(stderr, "Progress: %d%% (%d/%d blocks)\n", progress, block_num, total_blocks);
				}

				SHA512(buffer, bytes_read, hash);
				print_sha512(hash, ++block_num, total_blocks);
		}

		free(buffer);
		fclose(file);
		return 0;
}

