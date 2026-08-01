import subprocess
import os

def test_openmp_scaling():
    # We already have the compiled binary from earlier
    binary = os.path.abspath(os.path.join(os.path.dirname(__file__), "../../../test_omp_scale"))
    env = os.environ.copy()
    env["OMP_NUM_THREADS"] = "8"
    
    output = subprocess.check_output(binary, env=env, text=True)
    print(output)
    
    assert "Threads: 8" in output
    assert "Time:" in output
