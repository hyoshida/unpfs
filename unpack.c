#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <libgen.h>
#include <string.h>

#include "mkdir_p.c"
#include "strtr.c"

int output_to_file(int fp, char *filepath, int filesize, off_t offset) {
  int fp_output;
  off_t prev_offset;
  char *buf;

  prev_offset = lseek(fp, 0, SEEK_CUR);
  lseek(fp, offset, SEEK_SET);

  buf = (char*)malloc(filesize);
  if (buf != NULL) {
    read(fp, buf, filesize);
    printf("size: %d\n", filesize);

    mkdir_p(dirname(filepath));
    fp_output = open(filepath, O_WRONLY | O_CREAT, 0644);

    write(fp_output, buf, filesize);
    close(fp_output);

    free(buf);
  }

  lseek(fp, prev_offset, SEEK_SET);

  printf("----------------\n");
  printf("output!\n");
  printf("----------------\n");

  return (buf != NULL);
}


void output_file_header(int fp, char *path, off_t offset) {
  off_t prev_offset;
  char buf[10];

  prev_offset = lseek(fp, 0, SEEK_CUR);
  lseek(fp, offset, SEEK_SET);

  read(fp, &buf, sizeof(buf) - 1);
  buf[sizeof(buf) - 1] = '\0';
  printf("buf: %s\n", buf);
  printf("\n");

  lseek(fp, prev_offset, SEEK_SET);
}

int unpack(int fp, int output_flag) {
  int size;
  char *filepath;
  int unknown1, unknown2, unknown3;
  int start;
  int end;
  int result;

  read(fp, &size, sizeof(size));

  filepath = (char*)malloc(size + 1);
  if (filepath == NULL) {
    return 1;
  }
  filepath[size] = '\0';

  read(fp, filepath, size);
  // replace: '\' => '/'
  strtr(filepath, '\\', '/');

  printf("filepath: %s\n", filepath);

  read(fp, &unknown1, sizeof(unknown1));
  read(fp, &unknown2, sizeof(unknown2));
  read(fp, &unknown3, sizeof(unknown3));

  read(fp, &start, sizeof(start));
  read(fp, &end, sizeof(end));
  printf("start: %d\n", start);
  printf("end: %d\n", end);

  result = 0;
  if (output_flag != 0) {
    result = output_to_file(fp, filepath, end, start);
  }

  free(filepath);
  return result;
}

int unpack_file_header(int fp) {
  return unpack(fp, 0);
}

int unpack_file(int fp) {
  return unpack(fp, 1);
}

void unpacker(char *path) {
  int fp;

  char prefix[4];
  int header_start;
  int header_end;
  int unknown;

  int current_offset;

  fp = open(path, O_RDONLY);

  read(fp, &prefix, sizeof(prefix) - 1);
  prefix[sizeof(prefix) - 1] = '\0';
  printf("%s\n", prefix);

  read(fp, &header_end, sizeof(header_end));
  read(fp, &header_start, sizeof(header_start));
  read(fp, &unknown, sizeof(unknown));

  printf("header_end: %d\n", header_end);
  printf("header_start: %d\n", header_start);
  printf("unknown: %d\n", unknown);

  while ((current_offset = lseek(fp, 0, SEEK_CUR)) < header_end) {
    printf("\n");
    printf("current_offset: %d\n", current_offset);
    unpack_file(fp);
  }

  close(fp);
}

int main(int  argc, char *argv[]) {
  if (argc <= 1) {
    printf("usage: %s <file>\n", argv[0]);
    return 1;
  }
  unpacker(argv[1]);
  return 0;
}
