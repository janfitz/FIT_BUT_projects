#!/bin/sh
# Author: Jan Fitz
# Date: 11.2016
# Description: Script for automatic testing of IFJ16 interpret output

#######################
#make ad main definitions
tput setaf 3;
echo "Compilation started..."
$counterSuccess= 0
$counterAll = 0

make

if [ $? -eq 0 ] 
then
  tput setaf 2;
  echo "Compilation succesful"
else
  tput setaf 1;
  echo "Compilation error" >&2
  tput setaf 7;
  exit $?
fi

#######################
#TEST 1
tput setaf 3;
echo ""
echo "Test 1..."
output1="$(./ifj16  test1.java)"
echo "${output1}"
counterAll=$((counterAll + 1))

if [ $? -eq 0 ] 
then
  tput setaf 2;
  echo "TEST 1 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 1 FAIL" >&2
fi

########################
#TEST 2
tput setaf 3;
echo ""
echo "Test 2..."
output2="$(./ifj16  test2.java)"
echo "${output2}"
counterAll=$((counterAll + 1))

if [ ${output2} -eq "5" ] 
then
  tput setaf 2;
  echo "TEST 2 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 2 FAIL" >&2
fi

########################
#TEST 3
tput setaf 3;
echo ""
echo "Test 3..."
output3="$(./ifj16  test3.java)"
echo "${output3}"
counterAll=$((counterAll + 1))

if [ ${output3} -eq "6" ] 
then
  tput setaf 2;
  echo "TEST 3 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 3 FAIL" >&2
fi

########################
#TEST 4
tput setaf 3;
echo ""
echo "Test 4..."
output4="$(./ifj16  test4.java)"
echo "${output4}"
counterAll=$((counterAll + 1))

if [ ${output4} -eq "42" ] 
then
  tput setaf 2;
  echo "TEST 4 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 4 FAIL" >&2
fi

########################
#TEST 5
tput setaf 3;
echo ""
echo "Test 5..."
output5="$(./ifj16  test5.java)"
echo "${output5}"
counterAll=$((counterAll + 1))

if [ ${output5} -eq "6" ] 
then
  tput setaf 2;
  echo "TEST 5 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 5 FAIL" >&2
fi

########################
#TEST 6
tput setaf 3;
echo ""
echo "Test 6..."
output6="$(./ifj16  test6.java)"
echo "${output6}"
counterAll=$((counterAll + 1))

if [ ${output6} -eq "8" ] 
then
  tput setaf 2;
  echo "TEST 6 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 6 FAIL" >&2
fi
########################
#TEST 7
tput setaf 3;
echo ""
echo "Test 7..."
./ifj16  test7.java

if [ $? -eq 2 ] 
then
  tput setaf 2;
  echo "TEST 7 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 7 FAIL" >&2
fi
counterAll=$((counterAll + 1))

########################
#TEST 8
tput setaf 3;
echo ""
echo "Test 8..."
output8="$(./ifj16  test8.java)"
echo "${output8}"
counterAll=$((counterAll + 1))

if [ ${output8} -eq "18" ] 
then
  tput setaf 2;
  echo "TEST 8 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 8 FAIL" >&2
fi

########################
#TEST 9
tput setaf 3;
echo ""
echo "Test 9..."
output9="$(./ifj16  test9.java)"
echo "${output9}"
counterAll=$((counterAll + 1))

if [ ${output9} -eq "20" ] 
then
  tput setaf 2;
  echo "TEST 9 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 9 FAIL" >&2
fi

########################
#TEST 10
tput setaf 3;
echo ""
echo "Test 10..."
output10="$(./ifj16  test10.java)"
echo "${output10}"
counterAll=$((counterAll + 1))

if [ ${output10} -eq "15" ] 
then
  tput setaf 2;
  echo "TEST 10 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 10 FAIL" >&2
fi

########################
#TEST 11
tput setaf 3;
echo ""
echo "Test 11..."
output11="$(./ifj16  test11.java)"
echo "${output11}"
counterAll=$((counterAll + 1))

if [ ${output11} -eq "12" ] 
then
  tput setaf 2;
  echo "TEST 11 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 11 FAIL" >&2
fi

########################
#TEST 12
tput setaf 3;
echo ""
echo "Test 12..."
output12="$(./ifj16  test12.java)"
echo "${output12}"
counterAll=$((counterAll + 1))

if [ ${output12} -eq "9" ] 
then
  tput setaf 2;
  echo "TEST 12 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 12 FAIL" >&2
fi

########################
#TEST 13
tput setaf 3;
echo ""
echo "Test 13..."
output13="$(./ifj16  test13.java)"
echo "${output13}"
counterAll=$((counterAll + 1))

if [ ${output13} -eq "7" ] 
then
  tput setaf 2;
  echo "TEST 13 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 13 FAIL" >&2
fi

########################
#TEST 14
tput setaf 3;
echo ""
echo "Test 14..."
output14="$(./ifj16  test14.java)"
echo "${output14}"
counterAll=$((counterAll + 1))

if [ "${output14}" == "2368" ] 
then
  tput setaf 2;
  echo "TEST 14 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 14 FAIL" >&2
fi

########################
#TEST 15
tput setaf 3;
echo ""
echo "Test 15..."
output15="$(./ifj16  test15.java)"
echo "${output15}"
counterAll=$((counterAll + 1))

if [ ${output15} -eq "1" ] 
then
  tput setaf 2;
  echo "TEST 15 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 15 FAIL" >&2
fi

########################
#TEST 16
tput setaf 3;
echo ""
echo "Test 16..."
output16="$(./ifj16  test16.java)"
echo "${output16}"
counterAll=$((counterAll + 1))

if [ "${output16}" == "3" ] 
then
  tput setaf 2;
  echo "TEST 16 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 16 FAIL" >&2
fi

########################
#TEST 17
tput setaf 3;
echo ""
echo "Test 17..."
output17="$(./ifj16  test17.java)"
echo "${output17}"
counterAll=$((counterAll + 1))

if [ "${output17}" == "Vysledek je: 120" ] 
then
  tput setaf 2;
  echo "TEST 17 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 17 FAIL" >&2
fi

########################
#TEST 18
tput setaf 3;
echo ""
echo "Test 18..."
output18="$(./ifj16  test18.java)"
echo "${output18}"
counterAll=$((counterAll + 1))

if [ "${output18}" == "Vysledek: 120" ] 
then
  tput setaf 2;
  echo "TEST 18 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 18 FAIL" >&2
fi

########################
#TEST 19
tput setaf 3;
echo ""
echo "Test 19..."
output19="$(./ifj16  test19.java)"
echo "${output19}"
counterAll=$((counterAll + 1))

if [ "${output19}" == "Pozice retezce text v retezci str2: 0" ] 
then
  tput setaf 2;
  echo "TEST 19 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "TEST 19 FAIL" >&2
fi

########################
#TEST 20
tput setaf 3;
echo ""
echo "Test 20..."
output20="$(./ifj16  test20.java)"

if [ $? -eq 0 ] 
then
  tput setaf 2;
  echo "TEST 20 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "${output20}"
  echo "TEST 20 FAIL" >&2
fi
counterAll=$((counterAll + 1))

########################
#TEST 21
tput setaf 3;
echo ""
echo "Test 21..."
output21="$(./ifj16  test21.java)"

if [ $? -eq 1 ] 
then
  tput setaf 2;
  echo "TEST 21 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "${output21}"
  echo "TEST 21 FAIL" >&2
fi
counterAll=$((counterAll + 1))

########################
#TEST 22
tput setaf 3;
echo ""
echo "Test 22..."
output22="$(./ifj16  test22.java)"

if [ $? -eq 0 ] 
then
  tput setaf 2;
  echo "TEST 22 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "${output22}"
  echo "TEST 22 FAIL" >&2
fi
counterAll=$((counterAll + 1))

########################
#TEST 23
tput setaf 3;
echo ""
echo "Test 23..."
output23="$(./ifj16  test23.java)"

if [ $? -eq 1 ] 
then
  tput setaf 2;
  echo "TEST 23 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "${output23}"
  echo "TEST 23 FAIL" >&2
fi
counterAll=$((counterAll + 1))

########################
#TEST 24
tput setaf 3;
echo ""
echo "Test 24..."
output24="$(./ifj16  test24.java)"

if [ $? -eq 0 ] 
then
  tput setaf 2;
  echo "TEST 24 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "${output24}"
  echo "TEST 24 FAIL" >&2
fi
counterAll=$((counterAll + 1))

########################
#TEST 25
tput setaf 3;
echo ""
echo "Test 25..."
output25="$(./ifj16  test25.java)"

if [ $? -eq 1 ] 
then
  tput setaf 2;
  echo "TEST 25 OK"
  counterSuccess=$((counterSuccess + 1))
else
  tput setaf 1;
  echo "${output25}"
  echo "TEST 25 FAIL" >&2
fi
counterAll=$((counterAll + 1))

#######################
#Final statement
tput setaf 2;
echo ""
echo "Testing finished..."
echo "${counterSuccess} test from ${counterAll} were succesful"


tput setaf 7;
