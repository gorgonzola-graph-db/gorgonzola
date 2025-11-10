import os
import shutil
import subprocess
import sys

GORGONZOLA_ROOT = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
# Datasets can only be copied from the root since copy.schema contains relative paths
os.chdir(GORGONZOLA_ROOT)

# Define the build type from input
if len(sys.argv) > 1 and sys.argv[1].lower() == "release":
    build_type = "release"
else:
    build_type = "relwithdebinfo"

# Change the current working directory
if os.path.exists(f"{GORGONZOLA_ROOT}/dataset/databases/tinysnb"):
    shutil.rmtree(f"{GORGONZOLA_ROOT}/dataset/databases/tinysnb")
if sys.platform == "win32":
    gorgonzola_shell_path = f"{GORGONZOLA_ROOT}/build/{build_type}/src/gorgonzola_shell"
else:
    gorgonzola_shell_path = f"{GORGONZOLA_ROOT}/build/{build_type}/tools/shell/gorgonzola"
subprocess.check_call(
    [
        "python3",
        f"{GORGONZOLA_ROOT}/benchmark/serializer.py",
        "TinySNB",
        f"{GORGONZOLA_ROOT}/dataset/tinysnb",
        f"{GORGONZOLA_ROOT}/dataset/databases/tinysnb",
        "--single-thread",
        "--gorgonzola-shell",
        gorgonzola_shell_path,
    ]
)
