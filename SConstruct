from SCons.Environment import *
from SCons.Builder import *
from SCons.Node.FS import *
from SCons.Action import *
import webbrowser


def open_doxygen(target: str, source: str, env: Environment) -> None:
    '''
    Opens the Doxygen documentation in the default browser
    '''
    index_path = 'docs/html/index.html'
    print(f"Opening documentation {index_path}...")

    webbrowser.open('file://' + os.path.realpath(index_path))

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