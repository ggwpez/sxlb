extern "C"
{
  int  __cxa_atexit(void (*f)(void *), void *p, void *d);
  void __cxa_finalize(void *d);
  void __cxa_pure_virtual();
};

void* __dso_handle;

struct object
{
  void (*f)(void*);
  void *p;
  void *d;
} object[32] = {0};
unsigned int iObject = 0;

int __cxa_atexit(void (*f)(void *), void *p, void *d)
{
  if (iObject >= 32) return -1;
  object[iObject].f = f;
  object[iObject].p = p;
  object[iObject].d = d;
  ++iObject;
  return 0;
}

void __cxa_pure_virtual()   //called on not implemented virtual functions
{

}

/* NOTE: Zerstört momentan unabhängig von d alle Objekte */
void __cxa_finalize(void *d)
{
  unsigned int i = iObject;
  for (; i > 0; --i)
  {
    --iObject;
    object[iObject].f(object[iObject].p);
  }
}

