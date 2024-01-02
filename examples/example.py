from pathlib import Path
from subproc_stream import SubProc, SubProcLog

p = Path(__file__).parent / "example.log"

SubProc("echo hello").exec()
SubProcLog("ls -la", p).exec()
