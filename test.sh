#!/bin/bash

$program_name = "my_ls"
#prints if test succeed or failed. if failed prints the difference between outp and expected output
#input
# $1 = test number
# $2 = test letter
passFail() {
        cmp "testOutputs/test$1$2" "Expected_Outputs/test$1$2"
        status=$?
        if [[ $status = 0 ]]; then
            printf "\033[1;32m"; #green
            printf "\nTest %c%c\n" "$1" "$2"
            printf "PASSED\n";
            printf "\033[0m"; 
            
        else
            printf "\033[0;31m";
            printf "\nTest %c%c\n" "$1" "$2"
            printf "FAILED\n";
            printf "\033[0m"; 
            diff Expected_Outputs/test$1$2 testOutputs/test$1$2
        fi
        return 1;
}
#input
# $1 = test number
# $2 = test letter
# $3 = test input
test() {
        printf "\nTest %c%c\n" "$1" "$2"
        printf "flags: '%s'\n" "$3";
        if [[ $3 == "" ]]; then
            printf "empty flag input %s\n" "$3";
            ls > Expected_Outputs/test$1$2
            ./my_ls > testOutputs/test$1$2
        else
            ls $3> Expected_Outputs/test$1$2
            ./my_ls $3> testOutputs/test$1$2
        fi
        
        passFail $1 $2
        return 1;
}
make
printf "\n";
echo "*****************************"
echo "TESTING $program_name"
echo "*****************************"

test '1' 'a'
test '1' 'b' "-a"
test '1' 'c' "-t"
test '1' 'd' "-at"
test '1' 'e' "-a  -t"
test '1' 'f' "dirList"
test '1' 'g' "testOutputs"
test '1' 'h' "testOutputs dirList"
#test '1' 'j' "testOutputs -t"
#test '1' 'k' "testOutputs -at"
test '1' 'l' "testOutputs Expected_Outputs"


