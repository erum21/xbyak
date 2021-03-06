/*
	sample to use static memory
*/
#include <stdio.h>
#include "xbyak/xbyak.h"

MIE_ALIGN(4096) char buf[4096];

struct Code : Xbyak::CodeGenerator {
	Code()
		: Xbyak::CodeGenerator(sizeof(buf), buf)
	{
		puts("generate");
		printf("ptr=%p, %p\n", getCode(), buf);
		Xbyak::CodeArray::protect(buf, sizeof(buf), true);
#ifdef XBYAK32
		mov(eax, ptr [esp + 4]);
		add(eax, ptr [esp + 8]);
#elif defined(XBYAK64_WIN)
		lea(rax, ptr [rcx + rdx]);
#else
		lea(rax, ptr [rdi + rsi]);
#endif
		ret();
	}
} s_code;

inline int add(int a, int b)
{
	return ((int (*)(int,int))(void*)buf)(a, b);
}

int main()
{
	int sum = 0;
	for (int i = 0; i < 10; i++) {
		sum += add(i, 5);
	}
	printf("sum=%d\n", sum);
}
