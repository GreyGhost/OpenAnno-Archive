#ifndef CONFIGINCLUDE_H
#define CONFIGINCLUDE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef _WIN32
#	ifdef HAVE_INTTYPES_H
#		undef HAVE_INTTYPES_H
#	endif
#	ifdef HAVE_STDINT_H
#		undef HAVE_STDINT_H
#	endif
#	ifdef HAVE_STRINGS_H
#		undef HAVE_STRINGS_H
#	endif
#   ifdef HAVE_SYS_STAT_H
#		undef HAVE_SYS_STAT_H
#	endif
#endif

#endif // CONFIGINCLUDE_H
