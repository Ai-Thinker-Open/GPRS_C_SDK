#include "stdlib.h"
#include "api_os.h"

void* malloc(uint32_t size)
{
    return OS_Malloc(size);
}

void free(void* p)
{
    OS_Free(p);
}

void* realloc(void* p, uint32_t size)
{
    return OS_Realloc(p,size);
}

void *calloc(size_t nmemb, size_t size) {
    return malloc(nmemb * size);
}

// !!!! not the standard strtod, just impl with atof!!
double strtod(const char *restrict s, char **restrict p)
{
    double r = atof(s);
    *p = NULL;
	return r;
}


//from https://blog.csdn.net/ammana_babi/article/details/1473336
/*
   这个函数会将参数nptr字符串根据参数base来转换成长整型数。
   参数base范围从2至36，或0。参数base代表采用的进制方式，
   如base值为10则采用10进制，若base值为16则采用16进制等。
   当base值为0时则是采用10进制做转换，但遇到如’0x’前置
   字符则会使用16进制做转换、遇到’0’前置字符而不是’0x’
   的时候会使用8进制做转换。一开始strtol()会扫描参数nptr
   字符串，跳过前面的空格字符，直到遇上数字或正负符号才
   开始做转换，再遇到非数字或字符串结束时('/0')结束转换，
   并将结果返回。若参数endptr不为NULL，则会将遇到不合条件
   而终止的nptr中的字符指针由endptr返回。
*/
int strtol(const char *nptr, char **endptr, int base)

{
   const char *p = nptr;
   unsigned long ret;
   int ch;
   unsigned long Overflow;
   int sign = 0, flag, LimitRemainder;
   /*
      跳过前面多余的空格，并判断正负符号。
      如果base是0，允许以0x开头的十六进制数，
      以0开头的8进制数。
      如果base是16，则同时也允许以0x开头。
   */
   do
   {
      ch = *p++;
   } while (isspace(ch));
   if (ch == '-')
   {
      sign = 1;
      ch = *p++;
   }
   else if (ch == '+')
      ch = *p++;
   if ((base == 0 || base == 16) &&
      ch == '0' && (*p == 'x' || *p == 'X'))
   {
      ch = p[1];
      p += 2;
      base = 16;
   }
   if (base == 0)
      base = ch == '0' ? 8 : 10;
   Overflow = sign ? -(unsigned long)LONG_MIN : LONG_MAX;
   LimitRemainder = Overflow % (unsigned long)base;
   Overflow /= (unsigned long)base;
   for (ret = 0, flag = 0;; ch = *p++)
   {
      /*把当前字符转换为相应运算中需要的值。*/
      if (isdigit(ch))
        ch -= '0';
      else if (isalpha(ch))
        ch -= isupper(ch) ? 'A' - 10 : 'a' - 10;
      else
        break;
      if (ch >= base)
        break;
      /*如果产生溢出，则置标志位，以后不再做计算。*/
      if (flag < 0 || ret > Overflow || (ret == Overflow && ch > LimitRemainder))
        flag = -1;
      else
      {
        flag = 1;
        ret *= base;
        ret += ch;
      }
   }
   /*
      如果溢出，则返回相应的Overflow的峰值。
      没有溢出，如是符号位为负，则转换为负数。
   */
   if (flag < 0)
      ret = sign ? LONG_MIN : LONG_MAX;
   else if (sign)
      ret = -ret;
   /*
      如字符串不为空，则*endptr等于指向nptr结束
      符的指针值；否则*endptr等于nptr的首地址。
   */
   if (endptr != 0)
      *endptr = (char *)(flag ?(p - 1) : nptr);
   return ret;
}

unsigned int strtoul(const char *nptr, char **endptr, int base)
{
    return (unsigned int)strtol(nptr,endptr,base);
}


