#include <math.h>
#include <assert.h>
#include <string.h>
#include "Arduino.h"
#ifndef commonRSA_h
#define commonRSA_H
typedef struct{
  long l1;
  long l2;
  char* toString(){
    int l1len = 0;
    int l2len = 0;
    long l1cpy = l1;
    long l2cpy = l2;
    // determinine rhe length of a string
    while(l1cpy!=0){
      l1cpy/=10;
      l1len++;
    }
    while(l2cpy!=0){
      l2cpy/=10;
      l2len++;
    }
    l1cpy = l1;
    l2cpy = l2;
    char* keyStr = (char*)malloc((l1len+l2len+2)*sizeof(char));
    char* l1s = (char*)malloc(l1len*sizeof(char));
    char* l2s = (char*)malloc(l2len*sizeof(char));
    itoa(l1,l1s,10);
    itoa(l2,l2s,10);
    strcpy(keyStr,l1s);
    strcat(keyStr,",");
    strcat(keyStr,l2s);
    keyStr[l1len+l2len+1] = '\0';
    return keyStr;
  }
}key;

class RSA{
  public:
  RSA(long p1, long p2);
  key getPrivKey();
  key getPubKey();
  long rsa(int m, long e, long n);
  };
#endif
