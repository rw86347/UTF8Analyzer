//
//  testUtf8.c
//  test_enca
//
//  Created by Rodger Wilson on 7/11/13.
//  Copyright (c) 2013 Rodger Wilson. All rights reserved.
//

#include <stdio.h>


// tested -- works!
int returnUtf8Depth(unsigned char x){
    int count;
    char tempChar;
    
    if ((x & 0x80) == 0) { //if the left most bit is 0 return 0
        return 0;
    }
    
    // we don't need to test the left most bit because it was tested above.
    // we should start by testing the second left most bit
    for (count = 1; count < 6; count++) {
        tempChar = (x >> (7-count)) & 0x01;
        if (tempChar == 0) {
            return count;
        }
    }
    return count;
}

int isCharArrayUtf8Compliant(unsigned long strLen, void * stringBuffer){
    unsigned char * string = (unsigned char *)stringBuffer;
    int stateCount = 0;
    
    for (unsigned long i = 0; i < strLen; i++) {
        int isNotAscii = returnUtf8Depth(string[i]);
        printf("%d ", isNotAscii);
        if (stateCount > 0 && isNotAscii) { // The code point ended early
            stateCount -= 1; // Cool we found next code point dec count
        } else if (stateCount > 0 && isNotAscii == 0) {
            return 0; // return false because we found ascii in a UTF-8 code point location
        } else if (stateCount == 0 && isNotAscii) {
            if (isNotAscii == 1) {
                return 0; // this should never happen the beginning of a UTF8 should begine with a min of 2
            }
            stateCount = isNotAscii - 1; //This code point count is inclusive of this byte
        } else if (stateCount == 0 && isNotAscii == 0) {
            // do nothing  this is goodness, the compile will optimize this out
        } else {
            printf("What the heck!! we should never be here!  stateCount = %d", stateCount);
        }
    }

    
    return 1;
}


// All tests tested good!

#define six     0xFc
#define five    0xf8
#define four    0xf0
#define three   0xe0
#define two     0xc0
#define one     0x80
#define zero    0x00

char test1[] = {six, five, four, three, two, one, zero, zero}; //good
char test2[] = {zero, one, zero, zero, zero, zero, zero, zero}; //fail
char test3[] = {zero, four, one, one, one, zero, zero, zero}; //good
char test4[] = {zero, four, zero, zero, zero, zero, zero, zero}; //fail

void test(void){
    
    printf("test1 = %d\n", isCharArrayUtf8Compliant(8, test1));
    printf("test2 = %d\n", isCharArrayUtf8Compliant(8, test2));
    printf("test3 = %d\n", isCharArrayUtf8Compliant(8, test3));
    printf("test4 = %d\n", isCharArrayUtf8Compliant(8, test4));
    
}




