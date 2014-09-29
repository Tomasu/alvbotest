#ifndef DEBUG_H_GUARD
#define DEBUG_H_GUARD

#ifdef DEBUG

void Debug_(const char *FILE, const char *FUNCTION, int LINE, const char *t, const char *format, ...);

#define Debug(a, b...) Debug_(__FILE__, __FUNCTION__, __LINE__, "D", a, ##b)
#define Warn(a, b...) Debug_(__FILE__, __FUNCTION__, __LINE__, "W", a, ##b)
#define Error(a, b...) Debug_(__FILE__, __FUNCTION__, __LINE__, "E", a, ##b)

#else

#define Debug(a, b...) ((void)a)
#define Warn(a, b...) ((void)a)
#define Error(a, b...) ((void)a)

#endif /* !defined DEBUG */

#endif /* DEBUG_H_GUARD */
