/**
 * iambored.c
 * 
 * I was bored on a BXL->ATL flight so I wrote this
 * after trying to get sleep that I never found.
 * It's useless and was meant to compare sort algorythm.
 * Don't ask why I ended up reading directly from /dev/random
 * instead of using the regular interface, it was just funnier this way.
 *
 * PS: entropy is king.
 *
 * 1/7/2014 - Thomas Gouverneur
 */

#include <sys/timeb.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

/* Max number size */
#define N_MAX 99
/* How many number to generate */
#define N_NUM 64
/* How many number to print per row */
#define N_PC 8
/* Random source */
#define R_SRC "/dev/random"

#define RNA(x, y) *(x + (sizeof(unsigned int) * y))

unsigned int*	m_rand(void);
void	p_rand(unsigned int*);
void	m_round(unsigned int*);
void	m_sort(unsigned int *);

void	m_tick(struct timeb*);

int main(void) {
  unsigned int *rna;
  struct timeb tv_dur;

  printf("[-] Generating %d random numbers...\n", N_NUM);
  m_tick(NULL);
  rna = m_rand();
  m_tick(&tv_dur);
  m_round(rna);
  printf("[-] Done with generation. (took %d sec %d ms)\n", 
         tv_dur.time, tv_dur.millitm);
  printf("[-] Unsorted numbers are:\n");
  p_rand(rna);
  m_tick(NULL);
  m_sort(rna);
  m_tick(&tv_dur);
  printf("[-] Done with sorting. (took %d sec %d ms)\n",
         tv_dur.time, tv_dur.millitm);
  printf("[-] Sorted numbers are:\n");
  p_rand(rna);

  free(rna);
  return 1;
}

/**
 * Check diff time between two call
 */
void	m_tick(struct timeb *tv_diff) {
  static struct timeb tv_start, tv_end;
  if (!tv_diff) {
    gettimeofday(&tv_start, NULL);
  } else {
    gettimeofday(&tv_end, NULL);
    tv_diff->time = tv_end.time - tv_start.time;
    tv_diff->millitm = tv_end.millitm - tv_start.millitm;
  }
  return;
}

/**
 * "bubble" sort
 */
void	m_sort(unsigned int *rna) {
  int i,j;
  unsigned int t;

  for (i=0; i<N_NUM; i++) {
    for (j=0; j<N_NUM; j++) {
      if (RNA(rna, i) < RNA(rna, j)) {
        t = RNA(rna, i);
        RNA(rna, i) = RNA(rna, j);
        RNA(rna, j) = t;
      }
    }
  }
}

/**
 * Round everything, remove sign, etc..
 */
void 	m_round(unsigned int *rna) {
  int i;
  
  for (i=0; i<N_NUM; i++) {
    *(rna+(sizeof(unsigned int)*i)) = *(rna+(sizeof(unsigned int)*i)) % N_MAX;
  }
}

/**
 * Get N_NUM int from S_SRC random number source
 */
unsigned int*	m_rand(void) {
  int *rna, fd, sr, rd=0;
  
  fd = open(R_SRC, O_RDONLY);
  if (fd < 0) {
    perror("[!] Cannot open defined random source");
    exit(-1);
  }
  rna = (unsigned int*)malloc(sizeof(unsigned int)*N_NUM);
  if (!rna) {
    printf("[!] Cannot allocate needed memory\n");
    close(fd);
    exit(-2);
  }
  printf("[-] Reading");
  fflush(stdout);
  do {
    sr = read(fd, rna + (sizeof(unsigned int)*rd), sizeof(unsigned int));
    if (sr < sizeof(unsigned int)) {
      printf("x");
      fflush(stdout);
      sleep(1);
      continue;
    }
    printf(".");
    fflush(stdout);
    rd++;
  } while(rd < N_NUM);
  printf("done\n");
  return rna;
}


/**
 * Print the random number array
 * pointed by rna to screen.
 */
void	p_rand(unsigned int *rna) {
  int i, j;
  for (i=0; i<N_NUM; ) {
    for (j=0; j<N_PC; j++, i++) {
      printf("%2u ", *(rna+(sizeof(unsigned int)*i)));
    }
    printf("\n");
  }
  return;
}
