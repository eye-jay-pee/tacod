#ifndef COUNTOF_HEADER
#define COUNTOF_HEADER
#define countof(x) (sizeof(x) / sizeof(x[0]))
#undef COUNTOF_HEADER
#endif
