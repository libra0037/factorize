#include <iostream>
#include <random>
#include <intrin.h>
using ULL = uint64_t;

extern "C" ULL div64(ULL k0, ULL k1, ULL n);

unsigned long bsf(ULL x)
{
	unsigned long i;
	_BitScanForward64(&i, x);
	return i;
}

unsigned long bsr(ULL x)
{
	unsigned long i;
	_BitScanReverse64(&i, x);
	return i;
}

ULL gcd(ULL a, ULL b)
{
	if(a == 0 || b == 0)return a | b;
	int p = bsf(a | b);
	a >>= bsf(a);
	do
	{
		b >>= bsf(b);
		if(a > b)std::swap(a, b);
		b -= a;
	} while(b);
	return a << p;
}

struct Barrett
{
	ULL n, ni; int r;
	Barrett(ULL _n): n(_n), r(bsr(_n) + 1)
	{
		int r2 = r * 2 + 1;
		ULL k0 = r2 < 64 ? 1ULL << r2 : 0;
		ULL k1 = r2 < 64 ? 0 : 1ULL << (r2 - 64);
		ni = div64(k0, k1, n);
	}
	ULL rnd()const
	{
		static std::mt19937_64 gen(std::random_device{}());
		ULL mask = (1ULL << r) - 1, a;
		do a = gen() & mask; while(a >= n);
		return a;
	}
	ULL redu(ULL t0, ULL t1)const
	{
		ULL k1, k0 = _umul128(__shiftright128(t0, t1, r - 2), ni, &k1);
		k0 = r < 61 ? __shiftright128(k0, k1, r + 3) : k1 >> (r - 61);
		t0 -= k0 * n;
		return t0 < n ? t0 : t0 - n;
	}
	ULL operator()(ULL a, ULL b)const
	{
		ULL t1, t0 = _umul128(a, b, &t1);
		return redu(t0, t1);
	}
	ULL f(ULL x, ULL c)const
	{
		ULL t1, t0 = _umul128(x, x, &t1);
		unsigned char a = _addcarry_u64(0, t0, c, &t0);
		_addcarry_u64(a, t1, 0, &t1);
		return redu(t0, t1);
	}
};

bool isprime(const Barrett &N)
{
	const int s = bsf(N.n - 1); const ULL d0 = (N.n - 1) >> s;
	for(ULL a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022})
	{
		ULL x = 1;
		if(!(a %= N.n))continue;
		for(ULL d = d0; d; a = N(a, a), d >>= 1)if(d & 1)x = N(x, a);
		if(x == 1 || x == N.n - 1)continue;
		for(int i = 1; i < s && x != N.n - 1; i++)
			if((x = N(x, x)) == 1)return false;
		if(x != N.n - 1)return false;
	}
	return true;
}

ULL pollard_rho(const Barrett &N)
{
rho:
	ULL c = N.rnd(), xi = N.rnd(), xl = xi, p = 1, d;
	for(int z = 1; ; z <<= 1, xl = xi)for(int i = 1; i <= z; i++)
	{
		if(!(p = N(p, llabs((xi = N.f(xi, c)) - xl))))goto rho;
		if((i & 255 & (z - 1)) == 0 && (d = gcd(N.n, p)) > 1)return d;
	}
}

std::vector<ULL> Factorize(ULL n)
{
	std::vector<ULL> fact;
	auto fz = [&fact](auto &&self, ULL n)->void
	{
		Barrett N(n);
		if(isprime(N))return fact.push_back(n);
		ULL d = pollard_rho(N);
		self(self, d), self(self, n / d);
	};
	while(!(n & 1))fact.push_back(2), n >>= 1;
	if(n > 1)fz(fz, n);
	std::sort(fact.begin(), fact.end());
	return fact;
}

int main()
{
	while(1)
	{
		ULL n;
		std::cin >> n;
		if(n < 2 || n > 1ULL << 62)return 0;
		std::cout << "Factors:";
		for(ULL p : Factorize(n))std::cout << ' ' << p;
		std::cout << std::endl;
	}
}
