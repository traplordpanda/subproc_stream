#!/usr/bin/env python3
import time
import random

def main():
    # Generate a random sleep duration between 1 and 10 seconds
    sleep_duration = random.uniform(1, 5)
    for _ in range(20): 
        print(f"Sleeping for {sleep_duration:.2f} seconds...", flush=True)
        print("*" * (256//4), flush=True)
        time.sleep(sleep_duration)
    time.sleep(sleep_duration)
    
    print(f"Finished sleeping! for {sleep_duration:.2f} seconds...", flush=True)

if __name__ == "__main__":
    main()