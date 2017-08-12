//------------------------------------------------------------------------------
// 파 일 명 : string.c
// 프로젝트 : ezboot
// 설    명 : ezboot에서 사용하는 표출 출력 포맷 함수 관련 내용
// 작 성 자 : 유영창
// 작 성 일 : 2001년 11월 3일
// 수 정 일 : 
// 주    의 : 이 헤더 화일의 대부분의 내용은 리눅스 쏘스상에 
//            lib/string.c 에서 가져 왔다. 
//------------------------------------------------------------------------------
#include <stdio.h>

//******************************************************************************
//
// 함수 정의
//
//******************************************************************************
//------------------------------------------------------------------------------
// 설명 : 대소문자를 같은 것으로 취급한 문자열 비교 
// 매계 : s1, s2 : 비교할 문자열 len : 최대 길이 
// 반환 : 0 : 같다 그외 다르다. 
// 주의 : 비교 길이의 제한이 있다. 
//------------------------------------------------------------------------------
int strnicmp(const char *s1, const char *s2, size_t len)
{
	unsigned char c1, c2;

	c1 = 0;	c2 = 0;
	if (len) {
		do {
			c1 = *s1; c2 = *s2;
			s1++; s2++;
			if (!c1)
				break;
			if (!c2)
				break;
			if (c1 == c2)
				continue;
			c1 = tolower(c1);
			c2 = tolower(c2);
			if (c1 != c2)
				break;
		} while (--len);
	}
	return (int)c1 - (int)c2;
}

char * ___strtok;

//------------------------------------------------------------------------------
// 설명 : 문자열 복사 
// 매계 : dest : 목적지 src : 원본 
// 반환 : 목적지 최초 주소 
// 주의 : 없음 
//------------------------------------------------------------------------------
char * strcpy(char * dest,const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}

//------------------------------------------------------------------------------
// 설명 : 문자열 복사 
// 매계 : dest : 목적지 src : 원본 count : 최대 길이 
// 반환 : 목적지 최초 주소 
// 주의 : 길이의 제한을 둔다. 
//------------------------------------------------------------------------------
char * strncpy(char * dest,const char *src,size_t count)
{
	char *tmp = dest;

	while (count-- && (*dest++ = *src++) != '\0')
		/* nothing */;

	return tmp;
}
//------------------------------------------------------------------------------
// 설명 : 문자열 추가 
// 매계 : dest : 목적지 src : 원본 
// 반환 : 목적지 최초 주소 
// 주의 : 없음 
//------------------------------------------------------------------------------
char * strcat(char * dest, const char * src)
{
	char *tmp = dest;

	while (*dest)
		dest++;
	while ((*dest++ = *src++) != '\0')
		;

	return tmp;
}
//------------------------------------------------------------------------------
// 설명 : 문자열 추가 
// 매계 : dest : 목적지 src : 원본 
// 반환 : 목적지 최초 주소 
// 주의 : 길이의 제한이 있다. 
//------------------------------------------------------------------------------
char * strncat(char *dest, const char *src, size_t count)
{
	char *tmp = dest;

	if (count) {
		while (*dest)
			dest++;
		while ((*dest++ = *src++)) {
			if (--count == 0) {
				*dest = '\0';
				break;
			}
		}
	}

	return tmp;
}
//------------------------------------------------------------------------------
// 설명 : 문자열 비교 
// 매계 : cs, ct : 비교할 문자열 
// 반환 : 0 : 같다 그외 다르다. 
// 주의 : 없음 
//------------------------------------------------------------------------------
int strcmp(const char * cs,const char * ct)
{
	register signed char __res;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}

	return __res;
}
//------------------------------------------------------------------------------
// 설명 : 문자열 비교 
// 매계 : cs, ct : 비교할 문자열 count : 최대 길이  
// 반환 : 0 : 같다 그외 다르다. 
// 주의 : 길이의 제한이 있다. 
//------------------------------------------------------------------------------
int strncmp(const char * cs,const char * ct,size_t count)
{
	register signed char __res = 0;

	while (count) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
		count--;
	}

	return __res;
}
//------------------------------------------------------------------------------
// 설명 : 문자열속에서 문자를 발견한다. 
// 매계 : s : 대상 문자열 c : 찿고자 하는 문자 
// 반환 : 위치 
// 주의 : 없음 
//------------------------------------------------------------------------------
char * strchr(const char * s, int c)
{
	for(; *s != (char) c; ++s)
		if (*s == '\0')
			return NULL;
	return (char *) s;
}
//------------------------------------------------------------------------------
// 설명 : 문자열의 길이를 반환한다. 
// 매계 : s : 대상 문자열 
// 반환 : 길이 
// 주의 : 없음
//------------------------------------------------------------------------------
size_t strlen(const char * s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

//------------------------------------------------------------------------------
// 설명 : 문자열속에서 문자를 발견한다. 
// 매계 : s : 대상 문자열 c : 찿고자 하는 문자 
// 반환 : 위치 
// 주의 : 역으로 찾는다 
//------------------------------------------------------------------------------
char * strrchr(const char * s, int c)
{
       const char *p = s + strlen(s);
       do {
           if (*p == (char)c)
               return (char *)p;
       } while (--p >= s);
       return NULL;
}
//------------------------------------------------------------------------------
// 설명 : 문자열의 길이를 반환한다. 
// 매계 : s : 대상 문자열 count : 최대 길이  
// 반환 : 길이 
// 주의 : 길이의 제한이 있다. 
//------------------------------------------------------------------------------
size_t strnlen(const char * s, size_t count)
{
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}
//------------------------------------------------------------------------------
// 설명 : 문자열의 문자 구성을 비교 한다. 
//------------------------------------------------------------------------------
size_t strspn(const char *s, const char *accept)
{
	const char *p;
	const char *a;
	size_t count = 0;

	for (p = s; *p != '\0'; ++p) {
		for (a = accept; *a != '\0'; ++a) {
			if (*p == *a)
				break;
		}
		if (*a == '\0')
			return count;
		++count;
	}

	return count;
}
//------------------------------------------------------------------------------
// 설명 : 구분 문자를 검색한다. 
//------------------------------------------------------------------------------
char * strpbrk(const char * cs,const char * ct)
{
	const char *sc1,*sc2;

	for( sc1 = cs; *sc1 != '\0'; ++sc1) {
		for( sc2 = ct; *sc2 != '\0'; ++sc2) {
			if (*sc1 == *sc2)
				return (char *) sc1;
		}
	}
	return NULL;
}
//------------------------------------------------------------------------------
// 설명 : 토큰 문자를 판정한다. 
//------------------------------------------------------------------------------
char * strtok(char * s,const char * ct)
{
	char *sbegin, *send;

	sbegin  = s ? s : ___strtok;
	if (!sbegin) {
		return NULL;
	}
	sbegin += strspn(sbegin,ct);
	if (*sbegin == '\0') {
		___strtok = NULL;
		return( NULL );
	}
	send = strpbrk( sbegin, ct);
	if (send && *send != '\0')
		*send++ = '\0';
	___strtok = send;
	return (sbegin);
}
//------------------------------------------------------------------------------
// 설명 : 문자열에서 토큰을 추출한다. 
//------------------------------------------------------------------------------
char * strsep(char **s, const char *ct)
{
	char *sbegin = *s, *end;

	if (sbegin == NULL)
		return NULL;

	end = strpbrk(sbegin, ct);
	if (end)
		*end++ = '\0';
	*s = end;

	return sbegin;
}
//------------------------------------------------------------------------------
// 설명 : 메모리 영역을 특정 값을 채운다. 
//------------------------------------------------------------------------------
void * memset(void * s,int c,size_t count)
{
	char *xs = (char *) s;

	while (count--)
		*xs++ = c;

	return s;
}
//------------------------------------------------------------------------------
// 설명 : 메모리 영역을 복사한다. 
//------------------------------------------------------------------------------
char * bcopy(const char * src, char * dest, int count)
{
	char *tmp = dest;

	while (count--)
		*tmp++ = *src++;

	return dest;
}
//------------------------------------------------------------------------------
// 설명 : 메모리 영역을 복사한다. 
//------------------------------------------------------------------------------
void * memcpy(void * dest,const void *src,size_t count)
{
	char *tmp = (char *) dest, *s = (char *) src;

	while (count--)
		*tmp++ = *s++;

	return dest;
}
//------------------------------------------------------------------------------
// 설명 : 메모리 영역을 복사한다. 
//------------------------------------------------------------------------------
void * memmove(void * dest,const void *src,size_t count)
{
	char *tmp, *s;

	if (dest <= src) {
		tmp = (char *) dest;
		s = (char *) src;
		while (count--)
			*tmp++ = *s++;
		}
	else {
		tmp = (char *) dest + count;
		s = (char *) src + count;
		while (count--)
			*--tmp = *--s;
		}

	return dest;
}
//------------------------------------------------------------------------------
// 설명 : 메모리 영역을 비교한다. 
//------------------------------------------------------------------------------
int memcmp(const void * cs,const void * ct,size_t count)
{
	const unsigned char *su1, *su2;
	signed char res = 0;

	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}
//------------------------------------------------------------------------------
// 설명 : 메모리 영역에서 값을 탐색한다 
//------------------------------------------------------------------------------
void * memscan(void * addr, int c, size_t size)
{
	unsigned char * p = (unsigned char *) addr;

	while (size) {
		if (*p == c)
			return (void *) p;
		p++;
		size--;
	}
  	return (void *) p;
}
//------------------------------------------------------------------------------
// 설명 : 문자열에서 다른 문자열을 탐색한다. 
//------------------------------------------------------------------------------
char * strstr(const char * s1,const char * s2)
{
	int l1, l2;

	l2 = strlen(s2);
	if (!l2)
		return (char *) s1;
	l1 = strlen(s1);
	while (l1 >= l2) {
		l1--;
		if (!memcmp(s1,s2,l2))
			return (char *) s1;
		s1++;
	}
	return NULL;
}
//------------------------------------------------------------------------------
// 설명 : 메모리에서 1 바이트를 탐색한다. 
//------------------------------------------------------------------------------
void *memchr(const void *s, int c, size_t n)
{
	const unsigned char *p = s;
	while (n-- != 0) {
        	if ((unsigned char)c == *p++) {
			return (void *)(p-1);
		}
	}
	return NULL;
}
//------------------------------------------------------------------------------
// 설명 : 대문자 문자열로 바꾼다. 
//------------------------------------------------------------------------------
void UpperStr( char *Str )
{
   while( *Str ){ *Str = toupper( *Str ); Str++; }
}
//------------------------------------------------------------------------------
// 설명 : 소문자 문자열로 바꾼다. 
//------------------------------------------------------------------------------
void LowerStr( char *Str )
{
   while( *Str ){ *Str = tolower( *Str ); Str++; }
}
//------------------------------------------------------------------------------
// 설명 : 수치형 문자열을 정수형으로 바꾼다. 
//------------------------------------------------------------------------------
unsigned long strtoul(const char *p, char **out_p, int base)
{
	unsigned long v = 0;

	while (isspace(*p))
		p++;
	if (((base == 16) || (base == 0)) &&
	    ((*p == '0') && ((p[1] == 'x') || (p[1] == 'X'))))
	{
		p += 2;
		base = 16;
	}
	if (base == 0)
	{
		if (*p == '0')
			base = 8;
		else
			base = 10;
	}
	while (1)
	{
		char c = *p;
		if ((c >= '0') && (c <= '9') && (c - '0' < base))
			v = (v * base) + (c - '0');
		else if ((c >= 'a') && (c <= 'z') && (c - 'a' + 10 < base))
			v = (v * base) + (c - 'a' + 10);
		else if ((c >= 'A') && (c <= 'Z') && (c - 'A' + 10 < base))
			v = (v * base) + (c - 'A' + 10);
		else
			break;
		p++;
	}

	if (out_p) *out_p = (char*)p;
	return v;
}
