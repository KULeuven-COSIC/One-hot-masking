#include "helper_Coron.h"
#include "MaskedComparison_Coron.h"
#include "randombytes.h"
#include "params.h"
#include "hal.h"
#include <string.h>

// whatever helper function
void linear_arithmetic_refresh(uint32_t x[NSHARES], uint32_t q){
  uint32_t r[2];
  for(uint32_t i=0; i< NSHARES-1; i+=2){
    rand_q(r);
    x[i] = (x[i] + r[0])%q;
    x[NSHARES-1] = (x[NSHARES-1] - r[0] + q)%q;

    x[i+1] = (x[i+1] + r[1])%q;
    x[NSHARES-1] = (x[NSHARES-1] - r[1] + q)%q;
  }
  #if (NSHARES-1)%2 == 1
  rand_q(r);
  x[NSHARES-2] = (x[NSHARES-2] + r[0])%q;
  x[NSHARES-1] = (x[NSHARES-1] - r[0] + q)%q;
  #endif
}

// randomness
void rand_q(uint32_t v[2]){
  uint32_t r;
  do{
    r = random_uint32_in();
  } while(r > (387U * Q*Q));
  r = r%(Q*Q);
  v[0] = r%(Q);
  v[1] = r/Q;
}

uint16_t rand16(){
  return (uint16_t)random_uint32_in()&(0xFFFF);
}

// modulus switching operations
static uint32_t umodulus_switch(const uint32_t x, const uint32_t q_start, const uint32_t q_end){
  return (2*q_end*x+q_start)/(2*q_start);
}

static uint32_t compress(const uint32_t x, const uint32_t q, const uint32_t d){
  return umodulus_switch(x, q, 1<<d)%(1<<d);
}

static uint32_t decompress(const uint32_t x, const uint32_t q, const uint32_t d){
  return umodulus_switch(x, 1<<d, q);
}

// multiplication
void sec_mult(uint32_t a[NSHARES], uint32_t b[NSHARES], uint32_t c[NSHARES], uint32_t q)
{
  uint32_t r[2], t;
  uint32_t pool_size = (NSHARES*(NSHARES-1))/2;
  uint32_t pool[pool_size];

  for(uint32_t i=0; i < pool_size; i += 2){
    rand_q(r);
    pool[i  ] = r[0];
    pool[i+1] = r[1];
  }
  if ((pool_size%2) == 1){
    rand_q(r);
    pool[pool_size-1] = r[0];
  }

  uint32_t cpt = 0;

  for(uint32_t i=0; i < NSHARES; ++i) c[i] = (a[i]*b[i])%q;

  for(uint32_t i=0; i < NSHARES; ++i){
    for(uint32_t j=i+1; j < NSHARES; ++j){
      t = ((pool[cpt]+a[i]*b[j])+a[j]*b[i])%q;
      c[i] = (c[i] + q - pool[cpt])%q;
      c[j] = (c[j] + t)%q;
      cpt++;
    }
  }
}

// zero tests
uint32_t zero_test_mod_mult(uint32_t a[NSHARES], uint32_t q){
  uint16_t u_j, B;
  for(uint32_t j = 0; j < NSHARES; ++j){
    u_j = 1 + rand16()%(q-1);
    for(uint32_t i=0; i < NSHARES; ++i) a[i] = (u_j*a[i])%q;
    linear_arithmetic_refresh(a, q);

  }
  B = 0;
  for(uint32_t i=0; i < NSHARES; ++i) B = (B + a[i])%q;
  if (B == 0) return 1;
  else        return 0;
}

uint32_t zero_testing_prime_multi(const uint32_t SIZE, uint32_t ppoly[SIZE][NSHARES], uint32_t q){
  uint32_t a[NSHARES], t[NSHARES], C[NSHARES];
  for(uint32_t i=0; i < NSHARES; ++i) a[i] = rand16()%q;
  sec_mult(a, ppoly[0], C, q);

  for(uint32_t i=1; i < SIZE; ++i){
    for(uint32_t j=0; j < NSHARES; ++j) a[j] = rand16()%q;
    sec_mult(a, ppoly[i], t, q);
    for(uint32_t j=0; j < NSHARES; ++j) C[j] = (C[j] + t[j])%q;
  }
  return zero_test_mod_mult(C, q);
}

uint32_t zero_test_poly_mul(const uint32_t SIZE, uint32_t ppoly[SIZE][NSHARES], uint32_t q, uint32_t lambda){
  uint32_t b=1;
  for(uint32_t i=0; (i < lambda); ++i){
    b &= zero_testing_prime_multi(SIZE, ppoly, q);
  }
  return b;
}

uint32_t zero_test_poly_mul_with_reduction(const uint32_t SIZE, uint32_t ppoly[SIZE][NSHARES], uint32_t q, uint32_t kappa)
{
  uint32_t a;
  uint32_t y[kappa][NSHARES];
  for(uint32_t k=0; k < kappa; ++k){
    for(uint32_t i=0; i < NSHARES; ++i) y[k][i] = 0;
    for(uint32_t j=0; j < SIZE; ++j){
      a = rand16()%q;
      #if defined(NORANDOMSAMPLING)
        rng_idx = 0; // to avoid overflow of randomness register
      #endif
      for(uint32_t i=0; i < NSHARES; ++i) y[k][i] = (y[k][i] + a*ppoly[j][i])%q; 
    }
  }

  return zero_test_poly_mul(kappa, y, q, kappa);
}


// SecAnd SecAdd
void SecAnd(uint32_t a[NSHARES],uint32_t b[NSHARES],uint32_t c[NSHARES],uint32_t k,uint32_t n)
{
  for(uint32_t i=0;i<n;i++)
    c[i]=a[i] & b[i];

  for(uint32_t i=0;i<n;i++)
  {
    for(uint32_t j=i+1;j<n;j++)
    {
      uint32_t tmp=random_uint32_in(); //rand();
      uint32_t tmp2=(tmp ^ (a[i] & b[j])) ^ (a[j] & b[i]);
      c[i]^=tmp;
      c[j]^=tmp2;
    }
  }
  for(uint32_t i=0;i<n;i++) c[i]=c[i] % (1 << k);
}

// void SecAdd_Coron(uint32_t x[NSHARES],uint32_t y[NSHARES],uint32_t z[NSHARES],uint32_t k,uint32_t n)
// {
//   uint32_t u[n];
//   for(uint32_t i=0;i<n;i++) u[i]=0;
//   uint32_t w[n];
//   SecAnd(x,y,w,k,n);
//   uint32_t a[n];
//   for(uint32_t i=0;i<n;i++) a[i]=x[i] ^ y[i];
//   for(uint32_t j=0;j<k-1;j++)
//   {
//     uint32_t ua[n];
//     SecAnd(u,a,ua,k,n);
//     for(uint32_t i=0;i<n;i++) u[i]=(2*(ua[i] ^ w[i])) % (1 << k);
//   }
//   for(uint32_t i=0;i<n;i++) z[i]=x[i] ^ y[i] ^ u[i];
// }

void boolean_refresh(uint32_t x[NSHARES], uint32_t k){
  uint32_t r;
  for(uint32_t i=0; i< NSHARES; ++i){
    for(uint32_t j=i+1; j < NSHARES; ++j){
      r = (uint32_t) random_uint32_in() & ((1<<k)-1);
      x[i] = (x[i] ^ r);
      x[j] = (x[j] ^ r);
    }
  }
}

void sec_and(uint32_t x[NSHARES], uint32_t y[NSHARES], uint32_t res[NSHARES], uint32_t k){

#if (NSHARES-1) == 1
    uint16_t u = rand16()&((1<<k)-1);
    uint16_t z;
    z = u ^ (x[0] & y[0]);
    z = z ^ (x[0] & y[1]);
    z = z ^ (x[1] & y[0]);
    z = z ^ (x[1] & y[1]);
    res[0] = z;
    res[1] = u;

#else
    uint32_t r[NSHARES];
    uint16_t i, j, z_ij, z_ji;
    for(i=0; i < NSHARES; ++i) r[i] = x[i] & y[i];
    for(i=0; i < NSHARES; ++i)
        for(j=i+1; j < NSHARES; ++j){
            z_ij  = rand16()&((1<<k)-1);
            z_ji  = (x[i] & y[j]) ^ z_ij;
            z_ji ^= (x[j] & y[i]);
            r[i] ^= z_ij;
            r[j] ^= z_ji;            
        }
    for(i=0; i < NSHARES; ++i) res[i] = r[i];
#endif

}


// zero tests C
void boolean_zero_test(uint32_t x[NSHARES], uint32_t y[NSHARES], uint32_t k, uint32_t logk){
  uint32_t z[NSHARES];
  y[0] = (~x[0]) | ((1<<(1<<logk))-(1<<k));
  for(uint32_t i=1; i < NSHARES; ++i) y[i] = x[i];

  for(uint32_t i=0; i < logk; ++i){
    for(uint32_t j=0; j < NSHARES; ++j) z[j] = y[j] >> (1 << i);
    boolean_refresh(z, k);
    sec_and(y, z, y, k); 
  }
  for(uint32_t i=0; i < NSHARES; ++i) y[i] = (y[i])&1;

  boolean_refresh(y, k);

}

void bool_poly_zero_test(const uint32_t SIZE, uint32_t ppoly[SIZE][NSHARES], uint32_t b[NSHARES], uint32_t k, uint32_t logk){
  uint32_t temp1[NSHARES], temp2[NSHARES];
  uint32_t *p1=temp1, *p2=temp2, *swap;
//   Masked *p1=&temp1, *p2=&temp2, *swap;
  temp1[0] = 0xFFFF; 
  for(uint32_t i=1; i < NSHARES; ++i) temp1[i] = 0;

  for(uint32_t i=0; i < SIZE; ++i){
    ppoly[i][0] = ~ppoly[i][0]; 
    sec_and(ppoly[i], p1, p2, k);
    swap = p2;
    p2 = p1;
    p1 = swap;
  }
  temp1[0] = ~temp1[0];

  boolean_zero_test(temp1, b, k, logk);
}

// invert compression
void compress_inv(uint32_t range[2], const uint32_t ct, const uint32_t q, const uint32_t d)
{
  uint32_t B, y, a, b;

  B = ((q+(1<<d))>>(d+1));
  y = decompress(ct, q, d);
  a = (q + y - B) % q;
  b = (y + B)     % q;
  if (compress(a, q, d) != ct) a = (a+1)%q;
  if (compress(b, q, d) != ct) b = (b+q-1)%q;
  range[0] = a; range[1] = b;
}


// B2A

void convert_B2A(uint32_t x[NSHARES], uint32_t y[NSHARES], uint32_t k, uint32_t q){
  uint32_t T[(1<<k)][NSHARES];
  uint32_t T_p[(1<<k)][NSHARES];
  
  for(uint32_t u=0; u < (1<<k); ++u){
    T[u][0] = u%q;
    for(uint32_t i=1; i < NSHARES; ++i) T[u][i] = 0;
  }
  for(uint32_t i=0; i < NSHARES-1; ++i){
    for(uint32_t u=0; u < (1<<k); ++u){
      for(uint32_t j=0; j < NSHARES; ++j){
        T_p[u][j] = T[u^(x[i])][j]; 
      }
    }
    for(uint32_t u=0; u < (1<<k); ++u){
      linear_arithmetic_refresh(T_p[u], q); 
      for(uint32_t j=0; j < NSHARES; ++j) T[u][j] = T_p[u][j];
    }
  }
  for(uint32_t i=0; i < NSHARES; ++i) y[i] = T[x[NSHARES-1]][i]; 
  linear_arithmetic_refresh(y, q);
}