#include "rwfile.h"

static int* pos_bn(int* pos,const char* file,unsigned long size) {
    unsigned long i, j = 0;
    int* pos_bn_tab = xmalloc(size*sizeof(int));
    for (i = 0; i < size; i++) pos_bn_tab[i] = 0;
    for (i = 0; i < size; i++) {
        if (file[i] == '\n') {
            pos_bn_tab[j] = i;
            j++;
        }
    }
    *pos = j;
    return pos_bn_tab;
} 

static TPoint* point_of_line(char* buf,unsigned long n)
{
  double x, y;
  char* name = xcalloc(100,sizeof(char));
  char cpy[1000];

  strncpy(cpy,buf,n);
  sscanf(cpy," %s %lf %lf\n",name,&x,&y);

  return pnt_new(name,x,y,K_Target,NULL);
}

TPointFile* read_point_file(char* filename)
{
  struct stat info;
  int* bnp, pos, i;
  double key[2];
  char* buf;
  TPointFile* pf;
  FILE* fd = fopen(filename,"r");

  if (fd == NULL) {
    fprintf(stderr,"%s: error while opening %s in r mode : %s\n",nameProcessus,filename,strerror(errno)); 
    return NULL;
  }
  if (fstat(fileno(fd),&info) == -1) {
    fprintf(stderr,"%s: error stat : %s\n",nameProcessus,strerror(errno));
    return NULL;
  } 
  buf = xmalloc(info.st_size);
  if (fread(buf,sizeof(char),info.st_size,fd) == 0) {
    fprintf(stderr,"%s: error while reading of %s : %s\n",nameProcessus,filename,strerror(errno));
    return NULL;
  }
  fclose(fd);
  
  bnp = pos_bn(&pos,buf,info.st_size);
  
  pf = xmalloc(sizeof(*pf));

  pf->nbpoints = pos;
  pf->kdTree = kd_create(2);
  pf->points = xmalloc((pos+1) * sizeof(TPoint));
  pf->points[0] = point_of_line(buf,bnp[0]);
  key[0] = pf_x(0);
  key[1] = pf_y(0);
  kd_insert(pf->kdTree,key,pf->points[0]); 
  for (i = 0; i < pos-1; i++) {
    pf->points[i+1] = point_of_line(buf+bnp[i]+1,bnp[i+1]-bnp[i]);
    key[0] = pf_x(i+1);
    key[1] = pf_y(i+1);
    kd_insert(pf->kdTree,key,pf->points[i+1]); 
  }
  
  xfree(bnp);
  xfree(buf);

  return pf;
}
