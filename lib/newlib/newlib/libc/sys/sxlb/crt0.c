#include <fcntl.h>
 
extern void exit(int code);
extern int  main();
 
void _start()
{
    exit(main());
}