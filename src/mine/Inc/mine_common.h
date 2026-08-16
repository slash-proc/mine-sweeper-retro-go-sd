#ifndef __EMB_MINE_COMMON_H_
#define __EMB_MINE_COMMON_H_

#include "binary.h"
#include "font.h"
#include "color.h"
#include "myMathUtil.h"
#include "def_types.h"
#include "mine_platform.h"

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

void My_delay_ms(u16 n);
u8 My_strlen(char *x);

/* True RNG helper used by embMine when USE_EXTERN_REAL_RAND_FUNC is set. */
u16 My_real_rand(void);

#define ISDEBUG 0
#define DEBUG_SKIP_HOME 1
#define DEBUG_DEMO_GAMEOVER_NOWAIT 0

#define PRINT_LOG 0
#if PRINT_LOG
#define LOG(str) printf("%s", (str))
#else
#define LOG(str)
#endif

#endif
