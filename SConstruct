from SCons.Environment import *
from SCons.Builder import *
from SCons.Node.FS import *
from SCons.Action import *


def open_doxygen(target: str, source: str, env: Environment) -> None:
    '''
    Opens the Doxygen documentation in the default browser
    '''
    index_path = 'docs/html/index.html'
    print(f"Opening documentation {index_path}...")

    if os.name == 'nt':
        os.system(f'start {index_path}')
    elif os.name == 'posix':
        os.system(f'xdg-open {index_path}')
    else:
        print("Don't know how to open files on this OS.")

env = Environment()

doxygen_builder = Builder(
    action=Action('doxygen ${SOURCE}', cmdstr='Generating Doxygen documentation...'),
    target_factory=Dir,
    source_factory=File,
)

env.Append(BUILDERS={'Doxygen': doxygen_builder})

doxygen = env.Doxygen('docs', 'Doxyfile')

env.Alias('open', doxygen, open_doxygen)

env.Clean(doxygen, 'docs')
env.AlwaysBuild(doxygen)