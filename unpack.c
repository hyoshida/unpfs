#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <libgen.h>

void unpack_file_header(int fp) {
  int start;
  int end;
  int size;
  int unknown1, unknown2, unknown3;

  char *filepath;

  char buf[10];
  off_t prev_offset;

  read(fp, &size, sizeof(size));

  filepath = (char*)malloc(size);
  read(fp, filepath, size);
  printf("filepath: %s\n", filepath);
  free(filepath);

  read(fp, &unknown1, sizeof(unknown1));
  read(fp, &unknown2, sizeof(unknown2));
  read(fp, &unknown3, sizeof(unknown3));

  read(fp, &start, sizeof(start));
  read(fp, &end, sizeof(end));
  printf("start: %d\n", start);

  prev_offset = lseek(fp, 0, SEEK_CUR);
  lseek(fp, start, SEEK_SET);
  read(fp, &buf, sizeof(buf) - 1);
  printf("buf: %s\n", buf);
  lseek(fp, prev_offset, SEEK_SET);

  printf("\n");
}

void unpack_file(int fp) {
  int fp_output;

  int start;
  int end;
  int size;
  int unknown1, unknown2, unknown3;

  char *filepath;
  char *buf;
  off_t prev_offset;

  read(fp, &size, sizeof(size));

  filepath = (char*)malloc(size + 1);
  filepath[size] = '\0';
  read(fp, filepath, size);
  printf("filepath: %s\n", filepath);

  read(fp, &unknown1, sizeof(unknown1));
  read(fp, &unknown2, sizeof(unknown2));
  read(fp, &unknown3, sizeof(unknown3));

  read(fp, &start, sizeof(start));
  read(fp, &end, sizeof(end));
  printf("start: %d\n", start);
  printf("end: %d\n", end);

  prev_offset = lseek(fp, 0, SEEK_CUR);
  lseek(fp, start, SEEK_SET);

  buf = (char*)malloc(end);
  read(fp, buf, end);
  printf("size: %d\n", end);

  fp_output = open(basename(filepath), O_WRONLY | O_CREAT, 0644);

  write(fp_output, buf, end);
  close(fp_output);

  free(buf);
  free(filepath);

  lseek(fp, prev_offset, SEEK_SET);

  printf("----------------\n");
  printf("output!\n");
  printf("----------------\n");
  printf("\n");
}

int unpack(char *path) {
  int fp;

  char header[3];
  int version;
  int start, end;

  fp = open(path, O_RDONLY);

  read(fp, &header, sizeof(header));
  printf("%s\n", header);

  read(fp, &version, sizeof(version));
  read(fp, &start, sizeof(start));
  read(fp, &end, sizeof(end));

  unpack_file_header(fp);
  unpack_file(fp);
  unpack_file_header(fp);

  close(fp);
}

int main(int  argc, char *argv[]) {
  if (argc <= 1) {
    printf("usage: %s <file>\n", argv[0]);
    return 1;
  }
  unpack(argv[1]);
  return 0;
}
