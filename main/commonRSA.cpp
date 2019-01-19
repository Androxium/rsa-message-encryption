#include "commonRSA.h"
int debug_on = 0;
long gcd(long a, long b){
  long q, r;
  while (b > 0){
    q = a / b;
    r = a - q * b;
    a = b;
    b = r;
  }
  return a;
}

long euclidian_algorithm(long n1,long n2,long x1,long y1,long x2,long y2){
  long d = n1/n2;
  long r = n1%n2;
  if(r==0) return y2;
  n1 = n2;
  n2 = r;
  long xt = x1 - d*x2;
  long yt = y1 - d*y2;
  x1 = x2;
  y1 = y2;
  x2 = xt;
  y2 = yt;
  return euclidian_algorithm(n1,n2,x1,y1,x2,y2);
}

key pubKey;
key privKey;
RSA::RSA(long p1, long p2){
    long primeProd = (p1-1)*(p2-1);
    long a = primeProd/3;
    long b = 0;
    do{
    while(gcd(a,primeProd)!=1){
      a--;
    }
    a = 23;
    //a%=primeProd;
    b = euclidian_algorithm(primeProd,a,1,0,0,1);
    if(b < 0) b+= primeProd;
    if(a == b) a/=3;
    }while(a == b);
    Serial.print(primeProd);
    pubKey = {abs(p1*p2),abs(a)};
    privKey = {abs(p1*p2),abs(b)};
    //Serial.print("%s,%s",pubKey.toString(),privKey.toString());
}

key RSA::getPubKey(){
  return pubKey;
}

key RSA::getPrivKey(){
  return privKey;
}

long RSA::rsa(int m, long e, long n){
  long pow = 1;
  long remainder;
  long tempArrSize = 0;
  long *tempArr = malloc(sizeof(int));
  while (pow <= e){
    if (pow == 1){
      remainder = m % n;
    }
    else {
      remainder = (remainder * remainder) % n;
    }
    //Serial.print("pow of 2: %d, remainder: %d\n", pow_of_2, remainder);
    tempArr = realloc(tempArr, tempArrSize);
    assert(tempArr);
    tempArr[tempArrSize] = remainder;
    tempArrSize += 1;
    
    pow *= 2;
  }
  //printArray(tempArr, tempArrSize);
  pow /= 2;
  long cypherText = 1;
  // starts with the largest power of 2, working backwards towards 1
  while (e > 0){
    if (pow <= e){
      //Serial.print("pow of M: %d, remainder: %d\n", pow, tempArr[tempArrSize-1]);
      e -= pow;
      cypherText = (cypherText*tempArr[tempArrSize-1]);
    }
    pow /= 2;
    tempArrSize -= 1;
  }
  free(tempArr); tempArr = NULL;
  return (cypherText % n);
}
/**
int main(void){
  RSA test = RSA(pow(2,16));
}
**/
