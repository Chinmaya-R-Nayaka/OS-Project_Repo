#define SBRK_ERROR ((char *)-1)

struct stat;

// system calls
int fork(void);
int exit(int) __attribute__((noreturn));
int wait(int*);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(const char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sys_sbrk(int,int);
int pause(int);
int uptime(void);

// ---------------------------- Team G4 ------------------------------------
// -------------------------------------------------------------------------
int hello(void);
int getprocinfo(void);
int getyear(void);

/*-------------------------Chandrima's Work------------------------*/
int shmget(void);
void* shmat(int);
/*------------------------------------------------------------------*/

/* --- Bhanu's Work: Message Passing IPC --- */
// Send a message string to the process identified by receiver_pid.
// Returns 0 on success, -1 if the receiver's queue is full or pid not found.
int send(int receiver_pid, char *msg);
// Receive (dequeue) the oldest message from the calling process's mailbox.
// Blocks (sleeps) if the mailbox is empty until a message arrives.
// The received string is written into buf. Returns 0 on success.
int recv(char *buf);
/* ----------------------------------------- */

int clone(void *stack);
int join(void **stack);

// ulib.c

int mutex_lock(void);
int mutex_unlock(void);

// ------------------------------------------------------------------
// ------------------------------------------------------------------

int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
int atoi(const char*);
int memcmp(const void *, const void *, uint);
void *memcpy(void *, const void *, uint);
char* sbrk(int);
char* sbrklazy(int);

// printf.c
void fprintf(int, const char*, ...) __attribute__ ((format (printf, 2, 3)));
void printf(const char*, ...) __attribute__ ((format (printf, 1, 2)));

// umalloc.c
void* malloc(uint);
void free(void*);
int mutex_test_inc(void);
int get_counter(void);
