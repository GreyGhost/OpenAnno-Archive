#pragma once

#if defined(__GNUC__)
	#if __GNUC__ >= 4
		#define FORCEINLINE __attribute__((always_inline))
	#else
		#define FORCEINLINE
		#warning You are using a deprecated version of GCC. Inlining may be deactivated.
	
	#endif
#else
	#define FORCEINLINE
#endif
