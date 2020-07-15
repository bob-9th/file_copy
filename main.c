#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Usage : %s <src 1> <src 2> ... <src N> <dest folder>\n", argv[0]);
		return 0;
	}

	if (strlen(argv[argc - 1]) > 100) {
		printf("[ERR] Destination Folder's length must be less than 100.\n");
		return 0;
	}

	char dest[100];
	strcpy(dest, argv[argc - 1]);
	strcat(dest, "/");

	int ret = mkdir(dest, 0777);
	if ((ret != -1 || access(dest, F_OK)) && ret) { //first condition: file exist, but folder non-exist
		printf("[ERR] Destination Folder '%s' is not valid.\n", dest);
		return 0;
	}

	bool file_flag = false;
	for (int i = 1; i < argc - 1; i++) {
		if (strlen(argv[i]) > 100) {
			printf("[ERR] Source File's length must be less than 100.\n");
			return 0;
		}
		if (access(argv[i], F_OK) == -1) {
			printf("[ERR] Source File '%s' is not exist.\n", argv[i]);
			file_flag = true;
		}
	}
	FILE *f[100];
	if (file_flag) return 0;

	for (int i = 1; i < argc - 1; i++) {
		char dest_tmp[100];
		strcpy(dest_tmp, dest);
		strcat(dest_tmp, argv[i]);
		FILE *src = fopen(argv[i], "r"), *dest_file = fopen(dest_tmp, "w");
		if (src == NULL || dest_file == NULL) {
			if (src != NULL) fclose(src);
			if (dest_file != NULL) fclose(dest_file);
			printf("[ERR] %s -> %s (skip)\n", argv[i], dest_tmp);
			continue;
		}

		char buf;
		while ((buf = fgetc(src)) != EOF) fputc(buf, dest_file);

		fclose(src);
		fclose(dest_file);

		printf("[LOG] %s -> %s\n", argv[i], dest_tmp);
	}

	printf("[LOG] %d file%s copied!\n", argc - 2, argc > 3 ? "s" : "");
}