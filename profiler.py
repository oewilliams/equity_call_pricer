from timeit import timeit
from typing import List, Optional


"""
Run a list of external programs a number of times, timing each run, then print the minimum run time.
"""


CALL_TEMPLATE = "subprocess.run([{arg_list}], stdout=subprocess.DEVNULL, stderr=subprocess.STDOUT, shell={use_shell})"

def main():
    program_executors = {
        "python": Executor(loc="./python/main.py", runtime="python3", use_shell=False),
        "c++": Executor(loc="./cpp/equity_call_pricer", use_shell=True),
        "rust": Executor(loc="./rust/equity_call_pricer/target/release/equity_call_pricer", use_shell=True),
    }

    number_of_samples = 5

    for lang, executor in program_executors.items():
        run_times = []
        for _ in range(number_of_samples):
            time = time_subprocess(executor)
            run_times.append(time)
        
        min_time = min(run_times)
        print(lang, min_time)
    
    return

def time_subprocess(executor: 'Executor'):
    call_string = executor.get_call_string()
    time_taken = timeit(
        stmt=call_string,
        setup="import subprocess",
        number=1,
    )
    return time_taken

class Executor:

    def __init__(self, loc: str, runtime: Optional[str] = None, args: Optional[List[str]] = None, use_shell: bool = False) -> None:
        self.runtime = runtime
        self.loc = loc
        self.args = args
        self.use_shell = use_shell
    
        return

    def get_call_string(self):
        if self.runtime:
            terminal_inputs = [self.runtime, self.loc]
        else:
            terminal_inputs = [self.loc]
        
        if self.args:
            terminal_inputs += self.args
        
        terminal_string = ""
        for arg in terminal_inputs:
            terminal_string += f"'{arg}', "

        call_string = CALL_TEMPLATE.format(arg_list=terminal_string, use_shell=str(self.use_shell))

        return call_string
        

if __name__ == "__main__":
    main()