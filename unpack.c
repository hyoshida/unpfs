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
  printf("\n");

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

int unpack(int fp, int only_header_flag) {
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
  if (only_header_flag == 0) {
    result = output_to_file(fp, filepath, end, start);
  } else {
    output_file_header(fp, filepath, start);
  }

  free(filepath);
  return result;
}

int unpack_file_header(int fp) {
  return unpack(fp, 1);
}

int unpack_file(int fp) {
  return unpack(fp, 0);
}

void unpacker(char *path) {
  int fp;

  char header[4];
  int version;
  int start, end;

  fp = open(path, O_RDONLY);

  read(fp, &header, sizeof(header) - 1);
  header[sizeof(header) - 1] = '\0';
  printf("%s\n", header);

  read(fp, &version, sizeof(version));
  read(fp, &start, sizeof(start));
  read(fp, &end, sizeof(end));

  unpack_file(fp);
  unpack_file(fp);
  unpack_file(fp);

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
