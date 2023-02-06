#ifndef HELPER_CORON_H
#define HELPER_CORON_H

#include <stdint.h>
#include <stddef.h>
#include "params.h"

#ifdef DEBUG
#include <stdio.h>
#include <assert.h>
#endif

void rand_q(uint32_t v[2]);
uint16_t rand16(void);

void sec_mult(uint32_t a[NSHARES], uint32_t b[NSHARES], uint32_t c[NSHARES], uint32_t q);

uint32_t zero_test_poly_mul_with_reduction(const uint32_t SIZE, uint32_t ppoly[SIZE][NSHARES], uint32_t q, uint32_t kappa);
uint32_t zero_test_poly_mul(const uint32_t SIZE, uint32_t ppoly[SIZE][NSHARES], uint32_t q, uint32_t lambda);
uint32_t zero_testing_prime_multi(const uint32_t SIZE, uint32_t ppoly[SIZE][NSHARES], uint32_t q);
uint32_t zero_test_mod_mult(uint32_t a[NSHARES], uint32_t q);

void compress_inv(uint32_t range[2], const uint32_t ct, const uint32_t q, const uint32_t d);
void linear_arithmetic_refresh(uint32_t x[NSHARES], uint32_t q);

// void SecAdd(uint32_t *x,uint32_t *y,uint32_t *z,uint32_t k,uint32_t n);
void SecAnd(uint32_t *a,uint32_t *b,uint32_t *c,uint32_t k,uint32_t n);
void boolean_refresh(uint32_t x[NSHARES], uint32_t k);
void sec_and(uint32_t x[NSHARES], uint32_t y[NSHARES], uint32_t res[NSHARES], uint32_t k);
void convert_B2A(uint32_t x[NSHARES], uint32_t y[NSHARES], uint32_t k, uint32_t q);

void boolean_zero_test(uint32_t x[NSHARES], uint32_t y[NSHARES], uint32_t k, uint32_t logk);
void bool_poly_zero_test(const uint32_t SIZE, uint32_t ppoly[SIZE][NSHARES], uint32_t b[NSHARES], uint32_t k, uint32_t logk);

#endif // HELPER_CORON_H