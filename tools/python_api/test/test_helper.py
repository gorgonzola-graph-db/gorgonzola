import sys
from pathlib import Path

GORGONZOLA_ROOT = Path(__file__).parent.parent.parent.parent

if sys.platform == "win32":
    # \ in paths is not supported by gorgonzola's parser
    GORGONZOLA_ROOT = str(GORGONZOLA_ROOT).replace("\\", "/")
