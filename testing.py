import argparse
import inspect
import subprocess
import time
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


def run_command(commands, working_directory=None):
    """Function to run a shell command in the specified working directory."""
    if working_directory:
        os.chdir(working_directory)

    process = subprocess.Popen(
        commands,
        shell=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        encoding="utf-8",
        errors="ignore",
        universal_newlines=True,
        env=my_env,
    )
    calling_function = inspect.currentframe().f_back.f_code.co_name
    log_file_path = rk_logs_directory / f"{calling_function}.log"
    with open(log_file_path, "w", encoding="utf-8") as f:
        while True:
            output = str()
            if out := process.stdout.readline():
                output += out
            if out := process.stderr.readline():
                output += out 
            if output is None and process.poll() is not None:
                break
            if output:
                print(output.strip(), flush=True)
            f.write(output)


def build():
    run_command("sudo echo 'sudo' is required to run this script.")
    run_command("./bin/build.sh -cfbd", working_directory=rk_prototype_directory)


def create_test_users():
    run_command("./bin/create_test_users.sh", working_directory=rk_prototype_directory)


def run_tox():
    run_command("tox", working_directory=rk_prototype_directory)


def run_pytest():
    run_command(
        "pytest --cache-clear --test-suite=FAST tests/",
        working_directory=rk_testing_directory,
    )
    print("Sleeping for 5 minutes to allow the test to process...")
    time.sleep(300)
    run_command(
        "pytest --test-suite=FAST tests/",
        working_directory=rk_testing_directory,
    )


def last_failed():
    run_command(
        "pytest --test-suite=FAST --lf tests/",
        working_directory=rk_testing_directory,
    )


def main():
    parser = argparse.ArgumentParser(
        description="Run automation scripts for rk-prototype."
    )
    parser.add_argument(
        "-a", "--all", action="store_true", help="Run all automation scripts"
    )
    parser.add_argument(
        "-b", "--build", action="store_true", help="Run the build process"
    )
    parser.add_argument(
        "-c",
        "--create_test_users",
        action="store_true",
        help="Create the test users",
    )
    parser.add_argument("-t", "--tox", action="store_true", help="Run the tox tests")
    parser.add_argument(
        "-p", "--pytest", action="store_true", help="Run the pytest tests"
    )
    parser.add_argument(
        "-lf",
        "--last-failed",
        action="store_true",
        help="Run the last failed pytest tests",
    )

    args = parser.parse_args()
    # Run all if no specific flag is provided
    if args.all:
        build()
        run_tox()
        create_test_users()
        run_pytest()
    else:
        if args.build:
            build()
        if args.tox:
            run_tox()
        if args.create_test_users:
            create_test_users()
        if args.pytest:
            run_pytest()
        if args.last_failed:
            last_failed()


if __name__ == "__main__":
    main()
