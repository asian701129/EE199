#include <stdio.h>
#include <linux/module.h>


inline volatile long long RDTSC();

int main()
{
	long long TSC_value = RDTSC();
	printk("%lld\n",TSC_value);
} 

inline volatile long long RDTSC()
{
	register long long TSC asm("eax");
	asm volatile (".byte 15, 49" : : : "eax", "edx");
	return TSC;
} 

