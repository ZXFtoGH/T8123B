#ifndef __SHA256_H_
#define __SHA256_H_

#define SHA256_HASH_SZ (32)

#define MIN(a, b) (a < b ? a : b)

//typedef unsigned int size_t;
typedef unsigned long ulong32;
typedef unsigned long long ulong64;

struct sha256_state {
  ulong64 length;
  ulong32 state[8], curlen, handledlen;
  unsigned char buf[64];
};

typedef union Hash_state {
  char dummy[1];
  struct sha256_state sha256;
  void *data;
} hash_state;

int sha256_init(hash_state *md);
int sha256_process(hash_state *md, const unsigned char *in,
                   unsigned long inlen);
int sha256_done(hash_state *md, unsigned char *hash);
int sha256(const unsigned char *in, unsigned long inlen, unsigned char *hash);

#define STORE32L(x, y)                                                         \
  {                                                                            \
    (y)[3] = (unsigned char)(((x) >> 24) & 255);                               \
    (y)[2] = (unsigned char)(((x) >> 16) & 255);                               \
    (y)[1] = (unsigned char)(((x) >> 8) & 255);                                \
    (y)[0] = (unsigned char)((x)&255);                                         \
  }

#define LOAD32L(x, y)                                                          \
  {                                                                            \
    x = ((unsigned long)((y)[3] & 255) << 24) |                                \
        ((unsigned long)((y)[2] & 255) << 16) |                                \
        ((unsigned long)((y)[1] & 255) << 8) |                                 \
        ((unsigned long)((y)[0] & 255));                                       \
  }

#define STORE64L(x, y)                                                         \
  {                                                                            \
    (y)[7] = (unsigned char)(((x) >> 56) & 255);                               \
    (y)[6] = (unsigned char)(((x) >> 48) & 255);                               \
    (y)[5] = (unsigned char)(((x) >> 40) & 255);                               \
    (y)[4] = (unsigned char)(((x) >> 32) & 255);                               \
    (y)[3] = (unsigned char)(((x) >> 24) & 255);                               \
    (y)[2] = (unsigned char)(((x) >> 16) & 255);                               \
    (y)[1] = (unsigned char)(((x) >> 8) & 255);                                \
    (y)[0] = (unsigned char)((x)&255);                                         \
  }

#define LOAD64L(x, y)                                                          \
  {                                                                            \
    x = (((ulong64)((y)[7] & 255)) << 56) |                                    \
        (((ulong64)((y)[6] & 255)) << 48) |                                    \
        (((ulong64)((y)[5] & 255)) << 40) |                                    \
        (((ulong64)((y)[4] & 255)) << 32) |                                    \
        (((ulong64)((y)[3] & 255)) << 24) |                                    \
        (((ulong64)((y)[2] & 255)) << 16) | (((ulong64)((y)[1] & 255)) << 8) | \
        (((ulong64)((y)[0] & 255)));                                           \
  }

#define STORE32H(x, y)                                                         \
  {                                                                            \
    (y)[0] = (unsigned char)(((x) >> 24) & 255);                               \
    (y)[1] = (unsigned char)(((x) >> 16) & 255);                               \
    (y)[2] = (unsigned char)(((x) >> 8) & 255);                                \
    (y)[3] = (unsigned char)((x)&255);                                         \
  }

#define LOAD32H(x, y)                                                          \
  {                                                                            \
    x = ((unsigned long)((y)[0] & 255) << 24) |                                \
        ((unsigned long)((y)[1] & 255) << 16) |                                \
        ((unsigned long)((y)[2] & 255) << 8) |                                 \
        ((unsigned long)((y)[3] & 255));                                       \
  }

#define STORE64H(x, y)                                                         \
  {                                                                            \
    (y)[0] = (unsigned char)(((x) >> 56) & 255);                               \
    (y)[1] = (unsigned char)(((x) >> 48) & 255);                               \
    (y)[2] = (unsigned char)(((x) >> 40) & 255);                               \
    (y)[3] = (unsigned char)(((x) >> 32) & 255);                               \
    (y)[4] = (unsigned char)(((x) >> 24) & 255);                               \
    (y)[5] = (unsigned char)(((x) >> 16) & 255);                               \
    (y)[6] = (unsigned char)(((x) >> 8) & 255);                                \
    (y)[7] = (unsigned char)((x)&255);                                         \
  }

#define LOAD64H(x, y)                                                          \
  {                                                                            \
    x = (((ulong64)((y)[0] & 255)) << 56) |                                    \
        (((ulong64)((y)[1] & 255)) << 48) |                                    \
        (((ulong64)((y)[2] & 255)) << 40) |                                    \
        (((ulong64)((y)[3] & 255)) << 32) |                                    \
        (((ulong64)((y)[4] & 255)) << 24) |                                    \
        (((ulong64)((y)[5] & 255)) << 16) | (((ulong64)((y)[6] & 255)) << 8) | \
        (((ulong64)((y)[7] & 255)));                                           \
  }
#endif /* __SHA256_H_ */
