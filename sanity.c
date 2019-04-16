
#include "types.h"
#include "user.h"
#include "fcntl.h"

int success=0, fail=0,ans=-1, fibNum=40;




void make_test(void (*f)(void) , int expected ,char * fail_msg){

  f();
  if(ans == expected)
    success++;
  else {
    fail++;
    printf(1,"%s\n",fail_msg);
  }

}
int main(void){
    

    printf(1,"num of success:%d num of failures: %d\n",success,fail );

    exit(0);
  }
