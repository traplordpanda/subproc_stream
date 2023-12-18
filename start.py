import asyncio
import os
from pathlib import Path

# Set the environment variable to force color output
my_env = os.environ.copy()
my_env["MYPY_FORCE_COLOR"] = "1"

# Define the base directories using pathlib
BASE_DIRECTORY = Path("/home/kyle/code")
rk_logs_directory = BASE_DIRECTORY / "rk-test-logs"
rk_prototype_directory = BASE_DIRECTORY / "rk-prototype"
rk_testing_directory = BASE_DIRECTORY / "rk-testing"


async def run(cmd):
    os.chdir(rk_prototype_directory) 
    proc = await asyncio.create_subprocess_shell(
        cmd,
        stdout=asyncio.subprocess.PIPE,
        stderr=asyncio.subprocess.PIPE)

    async for line in (proc.stdout or proc.stderr):
        print(line.decode(), end='')
    # stdout, stderr = await proc.communicate()

    print(f'[{cmd!r} exited with {proc.returncode}]')

asyncio.run(run('./bin/build.sh -cfb'))