#if !defined(JPT_H)
#define JPT_H

#include "kt.h"

#define jpt_self kt_self();
#define jpt_fork(f,a) kt_fork((void *)f,a)

#define jpt_join(tid, f, a)\
{\
	kt_join(tid);\
	kt_fork((void *)f,a);\
	kt_exit();\
}

#define jpt_joinall(f,a)\
{\
	kt_joinall();\
	(f)(a);\
	kt_exit();\
}

#define jpt_exit() kt_exit()
#define j_Gsem kt_sem
#define j_make_gsem(c) make_kt_sem(c)
#define j_kill_gsem(s) kill_kt_sem(s)
#define j_gsem_getval(s) kt_getval(s)

#define j_gsem_P(s,f,a)\
{\
	P_kt_sem(s);\
	kt_fork((void *)f,a);\
	kt_exit();\
}

#define j_gsem_V(s) V_kt_sem(s)

#define jpt_sleep(sec,f,a)\
{\
	kt_sleep(sec);\
	kt_fork((void *)f,a);\
	kt_exit();\
}

#define jpt_yield(f,a)\
{\
	kt_yield();\
	kt_fork((void *)f,a);\
	kt_exit();\
}


#endif

