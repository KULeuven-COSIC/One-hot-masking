#include "MaskedComparison_Coron.h"
#include "helper_Coron.h"
#include "randombytes.h"
#include "params.h"
#include "hal.h"
#include <string.h>
#include "A2B.h"
#include "B2A.h"


// range_compare
void range_compare(uint32_t out[NSHARES], uint32_t CT[NSHARES], const uint32_t public_CT, const uint32_t k, const uint32_t q){
  uint32_t temp[NSHARES];
  uint32_t range[2], delta;

  compress_inv(range, public_CT, q, k);
  
  out[0] = 1;
  for(int i=1; i < NSHARES; ++i) out[i] = 0;
  
  CT[0] = (CT[0] + q - range[0])%q;

  if (range[1] < range[0]) range[1] += q;
  delta = range[1] - range[0] + 1;

  for(uint32_t i=0; i < delta; ++i){
    sec_mult(CT, out, temp, q);
    for(int j=0; j < NSHARES; ++j) {
      out[j] = temp[j];
    }
    CT[0] = (CT[0] + q - 1)%q;
  }
}

// high order compress
void high_order_compress(uint32_t out[32][NSHARES], uint32_t CT[32][NSHARES], uint32_t q, uint32_t k, uint32_t ell){
  uint32_t z[32][NSHARES];
  uint64_t temp;

  for(uint32_t j=0; j<32; j++)  
  {
    temp = (uint64_t)(CT[j][0]);
    z[j][0] = (temp<<(k+ell+1))/(2*q);
    z[j][0] = (z[j][0] + (1<<(ell-1)))%(1<<(k+ell));

    for(int i=1; i < NSHARES; ++i) {
      temp = (uint64_t)(CT[j][i]);
      z[j][i] = ((temp<<(k+ell+1))/(2*q))%(1<<(k+ell));
    }
  }

  uint32_t t[32][NSHARES];
  A2B_bitsliced(NSHARES, k+ell, t, z);
  // convert_A2B_CGV14_32bits(&z, &t, k+ell);

  for(uint32_t j=0; j<32; j++)  
  {
    for(int i=0; i < NSHARES; ++i) 
    {
      out[j][i] = (t[j][i] >> ell)%(1<<(k));
    }
  }
} 


// main function
uint64_t MaskedComparison_Hybrid(const uint32_t Bin[NCOEFFS_B][NSHARES], const uint32_t Cin[NCOEFFS_C][NSHARES], const uint32_t public_B[NCOEFFS_B], const uint32_t public_C[NCOEFFS_C])
{
  uint32_t individualcheck[NCOEFFS_B + 1][NSHARES];
  uint32_t B[NCOEFFS_B][NSHARES];
  uint32_t C[NCOEFFS_C][NSHARES];
  uint32_t C2[NCOEFFS_C][NSHARES];

  uint32_t b2[NSHARES];


  memcpy(B, Bin, NCOEFFS_B * NSHARES * sizeof(uint32_t));
  memcpy(C, Cin, NCOEFFS_C * NSHARES * sizeof(uint32_t));

  for (size_t i = 0; i < NCOEFFS_B; i++)
  {
    #if defined(NORANDOMSAMPLING)
      rng_idx = 0; // to avoid overflow of randomness register
    #endif
    range_compare(individualcheck[i], B[i], public_B[i], KYBER_DU, Q);
  }

  for (size_t i = 0; i < NCOEFFS_C; i+=32)
  {
    #if defined(NORANDOMSAMPLING)
      rng_idx = 0; // to avoid overflow of randomness register
    #endif
    high_order_compress(&C2[i], &C[i], Q, KYBER_DV, 16);
  }
  for(int i=0; i < NCOEFFS_C; ++i)
  {
    C2[i][0] = (C2[i][0] ^ public_C[i])&((1<<KYBER_DV)-1);
  } 

  #if defined(NORANDOMSAMPLING)
    rng_idx = 0; // to avoid overflow of randomness register
  #endif

  bool_poly_zero_test(NCOEFFS_C, C2, b2, KYBER_DV, 2);
  b2[0] = (~b2[0])&1;
  for(int i=1; i < NSHARES; ++i) b2[i] &=1;
  convert_B2A(b2, individualcheck[NCOEFFS_B], 1, Q);

  #if defined(NORANDOMSAMPLING)
    rng_idx = 0; // to avoid overflow of randomness register
  #endif

  return zero_test_poly_mul_with_reduction(NCOEFFS_B+1, individualcheck, Q, 11); 
  
}