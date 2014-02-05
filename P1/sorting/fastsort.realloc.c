#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "sort.h"
#include <malloc.h>

int compare (const void* a, const void* b) {
  
  unsigned aKey = ((rec_t*)a)->key;
  unsigned bKey = ((rec_t*)b)->key;

  return (aKey - bKey);
}

void usage() {
  fprintf (stderr, " Usage: fastsort -i inputfile -o outputfile\n");
  exit(1);
}

int main(int argc, char* argv[]) {
  
  int c;
  
  char* inFile = NULL;
  char* outFile = NULL;

  while ((c = getopt(argc,argv, "i:o:")) != -1) {
    switch(c) {
    case 'i':
      inFile = strdup(optarg);
      break;
    case 'o':
      outFile = strdup(optarg);
      break;
    default:
      usage();
    }
  }
  
  if (!inFile || !outFile) {
    usage();
  }

  int fileDesc = open(inFile, O_RDONLY);

  if (fileDesc < 0) {
    fprintf(stderr, "Error: Cannot open file %s", inFile);
    exit(1);
  }

  rec_t r;
  size_t recSize = 0;
  rec_t* recArr = NULL;
  
  while (1) {
    int rc;
    
    rc = read(fileDesc, &r, sizeof(rec_t));
    if (rc == 0)
      break;
    if (rc < 0) {
      fprintf(stderr, "Error: Cannot read file %s", inFile);
      exit(1);
    }
    
    recSize++;
    recArr = (rec_t*)realloc(recArr, (recSize*(sizeof(rec_t))));
    
    if (!recArr) {
      free(recArr);
      fprintf(stderr, "Error allocating memory!\n");
      exit(1);
    }
  
    recArr[recSize-1] = r;
  }
  
  (void) close(fileDesc);

  fileDesc = open(outFile, (O_WRONLY | O_CREAT| O_TRUNC), S_IRWXU);

  if (fileDesc < 0) {
    fprintf(stderr, "Error: Cannot open file %s", outFile);
    exit(1);
  }
 
  qsort (recArr, recSize, sizeof(rec_t), compare);

  int i;
  for (i = 0; i < recSize; i++) {
    int rc = write(fileDesc, &recArr[i], sizeof(rec_t));
    if (rc != sizeof(rec_t)) {
      fprintf(stderr, "Error: Cannot write file %s", outFile);
      exit(1);
    }
  }

  (void) close(fileDesc);

  free(recArr);
  return 0;
}
