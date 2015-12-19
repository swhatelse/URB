#include"common.h"

int join(char* fname){
 char buf[16];
  char** group = NULL;
  int group_count = 0;
  FILE *fd;
  fd = fopen(fname, "r");
  
  while(fgets(buf, sizeof(buf), fd) != NULL){
    group = realloc( (void*) group, group_count + 1 );
    group[group_count] = malloc(16);
    memcpy(group[group_count], buf, strlen(buf));
    group_count++;
  }

  // Debug to remove
  for(int i = 0; i < group_count; i++){    
    printf("%s", group[i]);
  }

  for(int i = 0; i < group_count; i++){
    connection(group[i]);
  }
  
  for(int i = 0; i < group_count; i++){
    free(group[i]);
  }
  free(group);
  fclose(fd);

  return EXIT_SUCCESS;
}

