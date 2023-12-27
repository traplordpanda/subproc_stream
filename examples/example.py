from subproc_stream import SubProc, SubProcLog

SubProc("echo hello").exec()
SubProcLog("ls -la", "example.log").exec()
