#include "SecAnd.h"
#include "randombytes.h"

// [http://www.crypto-uni.lu/jscoron/publications/secconvorder.pdf, Algorithm 1]
void SecAND32(size_t nshares, uint32_t z[nshares], const uint32_t x[nshares], const uint32_t y[nshares])
{
	uint32_t r[nshares][nshares];

	for (size_t i = 0; i < nshares; i++)
	{
		for (size_t j = (i + 1); j < nshares; j++)
		{
			r[i][j] = random_uint32_in();
			r[j][i] = r[i][j] ^ (x[i] & y[j]);
			r[j][i] = r[j][i] ^ (x[j] & y[i]);
		}
	}

	for (size_t i = 0; i < nshares; i++)
	{
		z[i] = x[i] & y[i];
		for (size_t j = 0; j < nshares; j++)
		{
			if (i != j)
			{
				z[i] ^= r[i][j];
			}
		}
	}

#ifdef DEBUG
	uint32_t x_unmasked = 0;
	uint32_t y_unmasked = 0;
	uint32_t z_unmasked = 0;

	for (size_t j = 0; j < nshares; j++)
	{
		x_unmasked ^= x[j];
		y_unmasked ^= y[j];
		z_unmasked ^= z[j];
	}

	assert(z_unmasked == (x_unmasked & y_unmasked));
#endif
}

// [http://www.crypto-uni.lu/jscoron/publications/secconvorder.pdf, Algorithm 1]
void SecAND64(size_t nshares, uint64_t z[nshares], const uint64_t x[nshares], const uint64_t y[nshares])
{
	uint64_t r[nshares][nshares];

	for (size_t i = 0; i < nshares; i++)
	{
		for (size_t j = (i + 1); j < nshares; j++)
		{
			r[i][j] = random_uint64_in();
			r[j][i] = r[i][j] ^ (x[i] & y[j]);
			r[j][i] = r[j][i] ^ (x[j] & y[i]);
		}
	}

	for (size_t i = 0; i < nshares; i++)
	{
		z[i] = x[i] & y[i];
		for (size_t j = 0; j < nshares; j++)
		{
			if (i != j)
			{
				z[i] ^= r[i][j];
			}
		}
	}

#ifdef DEBUG
	uint64_t x_unmasked = 0;
	uint64_t y_unmasked = 0;
	uint64_t z_unmasked = 0;

	for (size_t j = 0; j < nshares; j++)
	{
		x_unmasked ^= x[j];
		y_unmasked ^= y[j];
		z_unmasked ^= z[j];
	}

	assert(z_unmasked == (x_unmasked & y_unmasked));
#endif
}