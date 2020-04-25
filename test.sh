#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./kwcc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

assert 0 "0;"
assert 42 "42;"
assert 21 "5+20-4;"
assert 42 "10 + 40 - 8;"
assert 47 "5+6*7;"
assert 15 "5*(9-6);"
assert 4 "(3+5)/2;"
assert 10 "-10+20;"
assert 105 "+5+5*(10--10);"
assert 1 "5==5;"
assert 0 "3*2!=1+5;"
assert 1 "3<2*3;"
assert 0 "7>1+2*4;"
assert 5 "a=5;"
assert 2 "z=1+5*2;-9+z;"
assert 6 "a=1+2;b=a-1;a*b;"
assert 5 "aaa=5;"
assert 2 "_z=1+5*2;-9+_z;"
assert 6 "f00=1+2;_B_A_R_=f00-1;f00*_B_A_R_;"

echo OK
