// C implementation to see how errno value is
// set in the case of any error in C


#include <stdio.h>
#include <stdlib.h>
#include "e4c.h"

E4C_DEFINE_EXCEPTION(ColorException, "Colorful error.", RuntimeException);

// https://web.archive.org/web/20150518235657/https://code.google.com/p/exceptions4c/wiki/tutorial

void color()
{
     throw(ColorException, "Could not allocate buffer");
}

int main(void)
{
    e4c_using_context(E4C_TRUE){
     try {
         color();
       } catch(ColorException) {
        const e4c_exception * e = e4c_get_exception();
        printf("Error printed with try catch: %s (%s).\n", e->name, e->message);
       } finally {
         printf("finally\n");
      }
    }

   return 0;
}
