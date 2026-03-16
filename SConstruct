from SCons.Environment import *
from SCons.Builder import *
from SCons.Node.FS import *
from SCons.Action import *
import webbrowser


# Environment

env = Environment(
    CXX = 'clang++', # or your compiler of choice
    CXXFLAGS = ['-std=c++11', '-Wall', '-Wextra', '-Wpedantic', '-Wconversion', 
                '-Wshadow', '-Werror']
)

env.Append(
    CPPDEFINES = [
        'DOCTEST_CONFIG_SUPER_FAST_ASSERTS',
        '__WSTL_NO_INITIALIZERLIST__'
    ],
    CPPPATH = [
        env.Dir('doctest/doctest').srcnode(),
        env.Dir('include').srcnode()
    ]
)

# Doxygen

def OpenDoxygen(target: str, source: str, env: Environment) -> None:
    '''
    Opens the Doxygen documentation in the default browser
    '''
    index_path = 'docs/html/index.html'
    print(f"Opening documentation {index_path}...")

    webbrowser.open('file://' + os.path.realpath(index_path))

doxygen_builder = Builder(
    action=Action('doxygen ${SOURCE}', cmdstr='Generating Doxygen documentation...'),
    target_factory=Dir,
    source_factory=File,
)

env.Append(BUILDERS={'Doxygen': doxygen_builder})

doxygen = env.Doxygen('docs', 'Doxyfile')

env.Alias('open', doxygen, OpenDoxygen)

env.Clean(doxygen, 'docs')
env.AlwaysBuild(doxygen)

# Unit tests

def GlobRecursive(env: Environment, pattern, node='.'):
    src = str(env.Dir(node).srcnode())
    cwd = str(env.Dir('.').srcnode())

    dir_list = [src]
    for root, directories, _ in os.walk(src):
        for d in directories:
            dir_list.append(os.path.join(root, d))

    globs = []
    for d in dir_list:
        glob = env.Glob(os.path.join(os.path.relpath(d, cwd), pattern))
        globs.append(glob)

    return globs

env.VariantDir('build', 'tests', duplicate=0)
tests = env.Program('build/test', GlobRecursive(env, 'build/*.cpp'))

env.AlwaysBuild(env.Alias('test', tests, 'build/test'))