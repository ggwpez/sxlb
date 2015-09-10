typedef struct tm_t tm;
struct tm_t
{
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;	//month day 1-31
	int tm_mon;
	int tm_year;	//since 1900
	int tm_wday;	//weak day 0-6
	int tm_yday;	//year day
	int tm_isdst;	//>0 for summertime, ==0 no summertime, <0 unknown time
};

typedef unsigned int clock_t;
typedef unsigned int  time_t;
#define CLOCKS_PER_SEC 100

clock_t clock();
double difftime(time_t t1, time_t t2);