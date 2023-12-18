import sys
import time

def countdown(n):
    print('START', n)
    while n >= 0: 
        print(n, end=' ', flush=True)
        time.sleep(0.1)
        n -= 1
    print('END')

if __name__ == '__main__':
    args = sys.argv[1:]
    if len(args) != 1:
        sys.exit(3)
    countdown(int(args[0]))
