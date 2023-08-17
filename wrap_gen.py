import base64

BASE = r'''
import sys
import os


is_judge = os.getcwd() == "/judge"


def build():
    os.chdir(os.path.dirname(__file__))
    os.makedirs("build", exist_ok=True)
    os.chdir("build")
    os.makedirs("src", exist_ok=True)
    os.makedirs("src/fastset_impl", exist_ok=True)

    code_fastset = base64.b85decode(b"{code_fastset}")
    with open("./src/fastset_impl/fastset.cpp", "wb") as f:
        f.write(code_fastset)
    
    code_fastset_py = base64.b85decode(b"{code_fastset_py}")
    with open("./src/fastset_impl/fastset.py", "wb") as f:
        f.write(code_fastset_py)
    with open("../fastset.py", "wb") as f:
        f.write(code_fastset_py)
             
    code_fastset_pyi = base64.b85decode(b"{code_fastset_pyi}")
    with open("./src/fastset_impl/fastset.pyi", "wb") as f:
        f.write(code_fastset_pyi)
    with open("../fastset.pyi", "wb") as f:
        f.write(code_fastset_pyi)

    code_setup = base64.b85decode(b"{code_setup}")
    with open("./setup.py", "wb") as f:
        f.write(code_setup)
        
    if is_judge:
        subprocess.run(f"{{sys.executable}} setup.py --hpy-abi=universal develop --user".split(), check=True)
    else:
        subprocess.run(f"{{sys.executable}} setup.py --hpy-abi=universal develop".split(), check=True)
    
    os.chdir("../")
    os.system("mv ./build/src/fastset.hpy.so ./")


# compile time
if sys.argv[-1] == "ONLINE_JUDGE" or not is_judge:
    import subprocess
    import base64
    import traceback
    import io
    from textwrap import dedent
    from contextlib import redirect_stderr


    err_f = io.StringIO()
    try:
        with redirect_stderr(err_f):
            build()
    except Exception as e:
        if isinstance(e, subprocess.CalledProcessError):
            er = err_f.getvalue()
        else:
            er = traceback.format_exc()

        with open("fastset.py", "w") as f:
            f.write(dedent(f"""\
            import sys
            sys.stderr.write(\"\"\"{{er}}\"\"\")
            exit(1)
            """))
        raise e

    if is_judge:
        exit(0)


# run time


from fastset import Set
'''


def main():
    with open("./src/fastset_impl/fastset.cpp", "r") as f:
        code_fastset = f.read()

    with open("./src/fastset_impl/fastset.py", "r") as f:
        code_fastset_py = f.read()

    with open("./src/fastset_impl/fastset.pyi", "r") as f:
        code_fastset_pyi = f.read()

    with open("./setup.py", "r") as f:
        code_setup = f.read()

    print(BASE.format(
        code_fastset=base64.b85encode(code_fastset.encode()).decode(),
        code_fastset_py=base64.b85encode(code_fastset_py.encode()).decode(),
        code_fastset_pyi=base64.b85encode(code_fastset_pyi.encode()).decode(),
        code_setup=base64.b85encode(code_setup.encode()).decode(),
    ))


if __name__ == '__main__':
    main()
