//unknown liscense..
#ifndef _ASSERT_H_
#ifdef __cplusplus
extern "C"{
#endif
#ifdef NDEBUG
#define assert(x) \
  if (!x) {\
      printf("ASSERT FAILED: ");printf(#x); printf(" evaluated to false in ");printf( __FUNCTION__) ;printf(", ");printf( __FILE__);printf(",");printf( __LINE__);\
  }
#else
#define assert(x) x
#endif
#ifdef __cplusplus
}
#endif
#endif