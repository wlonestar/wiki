#include "thread.h"

/**
 * 在读者写者问题的基础上增加如下条件：
 * 若写者写完的内容还没有被任何一个读者读取，则新的写进程不能进行写操作，
 * 直到有至少一个读进程进行了读操作。
 */

sem_t wlock;    // 写者锁
sem_t rwlock;   // 读写锁
sem_t wwlock;   // 写者写完至少读一次
sem_t mutex_rc; // 互斥访问 rcount
sem_t mutex_rt; // 互斥访问 rtime

int rcount = 0; // 读者数量
int rtime = 0;  // 文件读的次数

void _write() { printf("("); } // 读者输出 '('
void _read() { printf(")"); }  // 写者输出 ')'

void Twrite() {
  while (1) {
    P(&wwlock);
    P(&rwlock);
    P(&wlock);
    _write(); // write
    P(&mutex_rt);
    rtime = 0; // 写者写完，读次数置0
    V(&mutex_rt);
    V(&wlock);
    V(&rwlock);
  }
}

void Tread() {
  while (1) {
    P(&rwlock);
    P(&mutex_rc);
    rcount++;
    if (rcount == 1) P(&wlock); // 第一个读者
    V(&mutex_rc);
    V(&rwlock);
    _read(); // read
    P(&mutex_rt);
    rtime++;
    if (rtime == 1) V(&wwlock); // 写者写完第一次读
    V(&mutex_rt);
    P(&mutex_rc);
    rcount--;
    if (rcount == 0) V(&wlock); // 最后一个读者
    V(&mutex_rc);
  }
}

int main(int argc, char *argv[]) {
  assert(argc == 3);
  int n = atoi(argv[1]); // 写线程数
  int m = atoi(argv[2]); // 读线程数

  SEM_INIT(&wlock, 1);
  SEM_INIT(&rwlock, 1);
  SEM_INIT(&wwlock, 0);
  SEM_INIT(&mutex_rt, 1);
  SEM_INIT(&mutex_rc, 1);

  setbuf(stdout, NULL);
  for (int i = 0; i < n; i++) create(Twrite);
  for (int i = 0; i < m; i++) create(Tread);
}
