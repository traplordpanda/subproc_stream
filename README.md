# subproc_stream

## Overview
`subproc_stream` is a Python library offering bindings for unidirectional C++ popen streaming. This library enables the execution of shell commands within Python or C++ environments, interacting with the output in a non-blocking manner. It's important to note that `subproc_stream` is unidirectional, meaning it only captures and outputs data to standard out; it does not capture output from commands beyond displaying it to standard output. This makes it particularly suited for real-time interaction with subprocess output in streaming scenarios.

## Installation
Install `subproc_stream` on Python 3.8 or later using pip:

```bash
pip install subproc_stream
```

## Dependencies
- `scikit-build-core >= 0.7.0`
- `nanobind >= 1.8.0`

Ensure these dependencies are installed in your environment.

## Features
- Non-blocking execution of shell commands.
- Unidirectional output streaming to standard out.
- Python and C++ integration.
- Supports CPython 3.12+ ABI.

## Usage

### Python Example
```python
from subproc_stream import SubProc

SubProc("echo hello").exec()  # Outputs to standard out
SubProc("ls -la").exec()      # Outputs directory listing to standard out
```

### C++ Example
```cpp
#include "subproc_stream.hpp"

int main() {
    subproc_stream::SubProc command("ls -a");
    command.exec();  // Outputs to standard out
}
```

## Building From Source
Clone the repository and run the build command:

```bash
git clone https://github.com/your-repo/subproc_stream.git
cd subproc_stream
python -m pip install .
```

## Testing
Run tests using `pytest`:

```bash
pytest tests/
```

## Contributing
Any contributions are welcomed!


## Authors
- aubuchon-kyle ([kdaub.tf2@gmail.com](mailto:kdaub.tf2@gmail.com))

---

This revised README includes the specifics of the library's unidirectional nature and its output limitations. Feel free to adjust this template as needed for your project.
