#include "A2B_table.h"
#include "hal.h"
#include "randombytes.h"
#include <string.h>

void convert_B2A(uint32_t x[NSHARES], uint32_t y[NSHARES], unsigned k, unsigned q);
void optimized_convert_B2A(uint32_t x[NSHARES], uint32_t y[NSHARES], unsigned k, unsigned q);
void shift1(uint32_t z[NSHARES], uint32_t a[NSHARES], uint32_t k);

void convert_B2A(uint32_t x[NSHARES], uint32_t y[NSHARES], unsigned k, unsigned q)
{
  /*
   * Generic Boolean to arithmetic conversion from G={0,1}^k to H=Z_{q} 
   */
  uint32_t  T[(1<<k)][NSHARES];
  uint32_t  T_p[(1<<k)][NSHARES];
  uint32_t r;
  
  for(int u=0; u < (1<<k); ++u){
    T[u][0] = u%q;
    for(int i=1; i < NSHARES; ++i) T[u][i] = 0;
  }
  for(int i=0; i < NSHARES-1; ++i){
    for(int u=0; u < (1<<k); ++u){
      for(int j=0; j < NSHARES; ++j){
        T_p[u][j] = T[u^(x[i])][j]; 
      }
    }
    for(int u=0; u < (1<<k); ++u){
      for(int l=0; l< NSHARES-1; ++l){
        r = random_uint32_in()%q;     
        T_p[u][l] = (T_p[u][l] + r)%q;
        T_p[u][NSHARES-1] = (T_p[u][NSHARES-1] - r + q)%q;
      }
      for(int l=0; l < NSHARES; ++l) T[u][l] = T_p[u][l];
    }
  }
  for(int i=0; i < NSHARES; ++i) y[i] = T[x[NSHARES-1]][i]; 
  for(int i=0; i< NSHARES-1; ++i){
    r = random_uint32_in()%q;     
    y[i] = (y[i] + r)%q;
    y[NSHARES-1] = (y[NSHARES-1] - r + q)%q;
  }
}

void optimized_convert_B2A(uint32_t x[NSHARES], uint32_t y[NSHARES], unsigned k, unsigned q){ 
  /*
   * Optimized Boolean to arithmetic conversion from G={0,1}^k to H=Z_{q}  
   * using a small size 1 generic convert
   */
  uint32_t z[NSHARES], t[NSHARES]; 
  for(int i=0; i < NSHARES; ++i) y[i] = 0;

  for(unsigned j=0; j < k; ++j){
    for(int i=0; i < NSHARES; ++i) z[i] = (x[i] >> j)&1;
    convert_B2A(z, t, 1, q);
    for(int i=0; i < NSHARES; ++i) y[i] = (y[i] + (t[i]<<j))%q;
  }

}

void shift1(uint32_t z[NSHARES], uint32_t a[NSHARES], uint32_t k){
  uint32_t x[NSHARES], c_bool[NSHARES], c[NSHARES];
  uint16_t T[NSHARES];
  uint16_t r;

  for(int i=0; i < NSHARES; ++i) x[i] = z[i]&1;
  T[0] = 0xFA50;
  for(int i=1; i < NSHARES; ++i) T[i] = 0;



  for(int i=0; i < NSHARES-1; ++i){
    for(int j=0; j < NSHARES; ++j) T[j]>>= (2*x[i]);
    for(int j=0; j < NSHARES-1; ++j){
      r = random_uint32_in();
      T[j] ^= r;
      T[NSHARES-1] ^= r;
    }
  }

  for(int i=0; i < NSHARES; ++i) c_bool[i] = (T[i]>>(2*x[NSHARES-1]))&3; 
  for(int j=0; j < NSHARES-1; ++j){
    r = random_uint32_in();
    c_bool[j] ^= r;
    c_bool[NSHARES-1] ^= r;
  }

  optimized_convert_B2A(c_bool, c, 2, 1<<(k-1));
  for(int i=0; i < NSHARES; ++i) a[i] = ((z[i] >> 1) + (c[i]&((1<<(k-1))-1)))%(1<<(k-1));
}

void A2B_table_inner(uint32_t B[NSHARES], uint32_t A[NSHARES], uint32_t bits);
void A2B_table_inner(uint32_t B[NSHARES], uint32_t A[NSHARES], uint32_t bits)
{
    for(int sh = 0; sh < NSHARES; sh++) B[sh] = 0;
    for(unsigned j=0; j<bits; j++)
    {
        for(int sh=0; sh<NSHARES; sh++) B[sh] = B[sh] + ((A[sh] & 1) << j);
        shift1(A, A, bits-j);
    }
}

void A2B_table(size_t NCOEFF, size_t modulus, uint32_t B[NCOEFF][NSHARES], const uint32_t A[NCOEFF][NSHARES])
{
    uint32_t Ap[NCOEFF][NSHARES];
    
    memcpy(Ap, A, NCOEFF * NSHARES * sizeof(uint32_t));

    for(uint32_t k=0; k<NCOEFF; k++)
    {
        #if defined(NORANDOMSAMPLING)
            rng_idx = 0;
        #endif
        A2B_table_inner(B[k], Ap[k], modulus); 
    }
    
}