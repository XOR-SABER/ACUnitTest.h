// ACUnitTest.h © Simple C Unit testing frame work that supports Timed, Random, fixed tests. Includes Color macros, Try and catch and a summary screen.
// By Alexander Betancourt XOR_SABER
// 5/22/2022
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef ALEX_ACUnitTest_H
#define ALEX_ACUnitTest_H
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <setjmp.h>

//C doesn't have booleans
enum boolean{
  False = 0,
  True = 1
};

//Constant defintions

//Booleans
#define   boolean enum boolean

//Colors
#define   RED   "\x1B[31m"
#define   GRN   "\x1B[32m"
#define   YEL   "\x1B[33m"
#define   BLU   "\x1B[34m"
#define   MAG   "\x1B[35m"
#define   CYN   "\x1B[36m"
#define   WHT   "\x1B[37m"
#define   RESET "\x1B[0m"

//Please Read this artcle, its really good I promise!:
//https://web.archive.org/web/20171226193122/http://www.di.unipi.it/~nids/docs/longjump_try_trow_catch.html
//Try Single Catch
#define   TRY do{ jmp_buf ex_buf__; if( !setjmp(ex_buf__) ){
#define   CATCH } else {
#define   THROW longjmp(ex_buf__, 1)
#define   ETRY break; } }while(0)

//Try Multple
#define   TRYMUL do{ jmp_buf ex_buf__; switch( setjmp(ex_buf__) ){ case 0:
#define   CATCHMUL(x) break; case x:
#define   THROWMUL(x) longjmp(ex_buf__, x)
#define   FINALLY break; } default: {
#define   ETRYMUL break; } } }while(0)

//Look, am sorry for overcommenting or overdocumenting. but am not gonna be like those idiots at Bridges.

/*
int assert_true(const int test_no, const boolean test, const char*desc)

Functionalty:
  Tests a single test case expression, prints the number of the test and a description when failed test.
  returns a point value ether 1 for Success or 0 for Failure
Parameters:
  const int test_no: is the number this test corresponds to.
  const boolean test: is a true or false value from a boolean test expression.
  const char *desc: is the description of a test.
Example syntax:
  total += assert_true(1, ((a.arr[0] == 10)), "push back test 1: index 0 = 10");
*/

// Please Read "CTest.h" Header file for detailed instructions
int assert_true(const int test_no, const boolean test, const char*desc){
  if(test == 1) {
    printf(GRN "TEST CASE %d: PASSED " RESET "\n", test_no);
    return 1;
  } else {
    printf(RED"TEST CASE %d: FAILED " RESET ": %s\n", test_no, desc);
    return 0;
  }
}

/*
int generated_assert_true(const int test_no, const int test_num, const boolean test, const char*desc)

Functionalty:
  Tests a single test case expression, prints the number of the test and a description when failed test, and the value generated to test.
  returns a point value ether 1 for Success or 0 for Failure
Parameters:
  const int test_no: is the number this test corresponds to.
  const int test_num: is the number that is tested.
  const boolean test: is a true or false value from a boolean test expression.
  const char *desc: is the description of a test.
Example syntax:
  total+= generated_assert_true(randomcase, randval, (get(b,i) == randval), "get check: ");
*/

//This is for randomly generated tests
// Please Read "CTest.h" Header file for detailed instructions
int generated_assert_true(const int test_no, const int test_num, const boolean test, const char*desc){
  if(test == 1) {
    printf(YEL "GENERATED TEST CASE %d: PASSED " RESET "\n", test_no);
    return 1;
  } else {
    printf(CYN "GENERATED TEST CASE %d: FAILED " RESET ": %s%d\n", test_no, desc, test_num);
    return 0;
  }
}

/*
int time_assert_true(const int test_no, const double quota, void(*func)())

Functionalty:
  Tests a functions time, and does a try catch to see if it works.
  then gives half credit for working code, and full credit for code that is below or equal to quota.
Parameters:
  const int test_no: is the number this test corresponds to.
  const double quota: is the quota, baka.
  void(*func)() is a function pointer. if your function takes parameters or non void (which is the most likey case!) please use this example code.

    //Our parameter less, void function;
    void timetest1(){
      //Initalize the class being tested
      INIT_Dynamo(C, 1000);
      //The Actual function being tested, the workaround being that we use the parameter function inside a void function
      clear_and_set(&C, 10);
      //Addional functions being tested 
      for(int i = 0; i < C.size; i++){
        push_back(&C, i);
      }
      for(int i = 0; i < C.size+10; i++){
        pop_back(&C);
      }
      //Free since no memory leaks
      free_dynamo(&C);
    }


Example syntax:
  total+= generated_assert_true(randomcase, randval, (get(b,i) == randval), "get check: ");
*/

// Please Read "CTest.h" Header file for detailed IMPORTANT (Especially for time_assert_true) instructions
int time_assert_true(const int test_no, const double quota, void(*func)()){
  //Starts the timer
  clock_t time;
  time = clock();
  //Runs the function from a function pointer
  TRY {
    func();
  }
  CATCH {
    printf(RED "TIMED TEST CASE %d: FAILED FUNCTION DID NOT WORK\n" RESET, test_no);
    return 0;
  }
  ETRY;
  time = clock() - time;
  double time_taken = ((double)time)/CLOCKS_PER_SEC;
  if(time_taken <= quota) {
    printf(BLU "TIMED TEST CASE %d: PASSED : FUNCTION TIME %lf SECONDS" RESET "\n",test_no, time_taken);
    return 10;
  } else {
    printf(MAG "TIMED TEST CASE %d: PASSED : FAILED TO MEET %lf SECOND QUOTA " RESET ": FUNCTION TIME %lf SECONDS\n", test_no, quota, time_taken);
    return 5;
  }
}
//Just a neat summary page for the person to get a grade or what ever.
void summary(int total, int final_total) {
    float precentage = final_total*100/total;
    printf(MAG "+----------------------------------+\n" RESET);
    printf(MAG "    final total: %d Tests passed    \n" RESET, final_total);
    printf(MAG "        ------------------          \n" RESET);
    printf(MAG "        total of %d Tests           \n" RESET, total);
    printf(MAG "                                    \n" RESET);
    printf(MAG "            %.2f%%                  \n" RESET,precentage);
    printf(MAG "+----------------------------------+\n" RESET);
}
#endif