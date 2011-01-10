#include <dlfcn.h>
#include <cstdio>
#include <cstdlib>

extern "C" {
        typedef struct _bridge { int a; } Bridge;
        void do_something(void *p);
        void bridge_callback(Bridge *b);
        
        void do_something(void *p) {
                // Bridge *bridge = reinterpret_cast<Bridge*>(p);
                // bridge_callback(bridge);
                printf("Got %p\n", p);
        }

        void bridge_callback(Bridge *b) {
                //b->notify_click_event(2.0, 13.0);
        }
}

int main() {
        void *lib = dlopen(NULL, RTLD_GLOBAL);
        void (*uia)(void*), *smt;
        int *b = new int;
        *b = 4;
        do_something(b);
        *(void**)(&uia) = dlsym(lib, "do_something");
        if(!uia) {
                printf("%s\n", dlerror());
                exit(0);
        }
        printf("smt: %p", uia);
        uia(b);
}
