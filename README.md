# CFuzzer
## URL Fuzzer written in C
### Usage:
Compile:
``` gcc main.c -lcurl -lpthread```
run:
```./a.out -url <url> -w <wordlist path> -threads <number of threads>
```
example:
```
./a.out -url www.google.com/ -w trial.txt -threads 10
```
PS: the `/` at the end of the URL is necessary.

can use a shit ton of improvements
