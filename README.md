# Demo: LLVM C-Bindings for Parsing C++ Sources

## Objective
The project demonstrates the use [LibClang](https://clang.llvm.org/docs/Tooling.html) interface of the LLVM infrastructure for parsing C++ sources.
For this project, a [header file](./TestVectors/Component.hpp) is used as a typical example of a C++ source to be parsed.


## Tools, Etc.
| Tool |   Version Used |
| ---:          |     :---      |
| Operating System | Linux Mint 19.3 (Based on Ubuntu 18.04/Bionic)
| CMake |   3.16.3 |
| GCC   |   9.3.0 |
| Clang |   11.0.0 |
| C++ Standard   |   11, 14, 17 |
| Docker*         | 19.03.12 |

\* Docker is required if container-based development is preferred.

## Build And Run The Project
### Option 1: Via Shell Script
Execute the provided shell script, making it executable if necessary first:
```bash
./buildProject.sh -s 11 -c LLVM -b debug
```
To get available build options, run the `buildProject.sh` without arguments:
```bash
./buildProject.sh
```
### Option 2: Docker Based Tools
All compilers and tools used in the development of this project are available in a container. See provided [Dockerfile](./.devcontainer/Dockerfile) for details.
#### Pre-Requisities
1. [Visual Studio Code](https://code.visualstudio.com)
2. Visual Studio Code plugin [Remote Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)
3. Docker with access to internet.

#### How To Use
1. Ensure pre-requisites are met,
2. Clone this repository,
3. Open Visual Studio Code, Run the Remote-Containers: `Open Folder in Container...` command and select the folder where (the master branch of) this repository is cloned.
   1. See [this link](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) for more help.
4. VSCode will attempt to build the container based on the provided [Dockerfile](./.devcontainer/Dockerfile) automatically.
5. On success, `Ctrl+Shift+B` will bring a menu of possible build choices that may be selected from.

#### Configurations for Machines Behind A Proxy
1. You may require an *authenticating proxy* such as [cntlm](http://cntlm.sourceforge.net)
2. You will need to configure Docker to work with your proxy
3. You will need to configure ./devcontainer/devcontainer.json to work with proxy.

## Brief Note on Implementation
The parser is implemented in the class [`RomanoViolet::StateMachine`](./CoreFunctions/Application/StateMachine.hpp) class.
The parser is instantiated from the client(or user) code implemented in [ParseHeader.cpp](./CoreFunctions/Application/ParseHeader.cpp) in line 159:

```c++
RomanoViolet::StateMachine p{ data._classDetails._name };
```
where the initialization parameter is the full class name to be parsed (e.g., `NN::RomanoViolet::Component`, see [test header file](./TestVectors/Component.hpp)).

Once instantiated, the parser instance 	`p` is added to the data structure `data` in line 180 of `ParseHeader.cpp`:
```c++
data.p = &p;
```
### `data` and `p`
A pointer to the data structure `data` forms the _opaque pointer_ which may be passed to the (client written) `visit` function in order to store results persistently across multiple calls to the `visit` function.

### The `visit` Function
The LLVM parser expects a user (client) written `visit` function to have the following signature:
```c++
CXChildVisitResult functionName( CXCursor cursor, CXCursor parent, CXClientData clientData )
```
where the actual function name (e.g., `visit` in the line above) is left to the user. In this project, the following two functions visit functions are used:

Line 43, [ParseHeader.cpp](./CoreFunctions/Application/ParseHeader.cpp):
```c++
CXChildVisitResult visitForFirstPass( CXCursor cursor, CXCursor parent, CXClientData clientData )
```

Line 90, [ParseHeader.cpp](./CoreFunctions/Application/ParseHeader.cpp):
```c++
CXChildVisitResult visit( CXCursor cursor, CXCursor parent, CXClientData clientData )
```

### Brief Approach
#### The State Machine
The parser is implemented as a state-machine, with rules encoded in the constructor of the [`RomanoViolet::StateMachine`](./CoreFunctions/Application/StateMachine.cpp), like so:

```c++
this->rules[State::INIT] = { { Event::NAMESPACE, State::NAMESPACE_COLLECTION },
                                 { Event::CLASS_DECLARATION, State::CLASSNAME_COLLECTION } };
```

with [rules](./CoreFunctions/Application/StateMachine.hpp) declare a data structure to encode state-transitions from _current state_ to a _new state_ based on an _Event_:
```c++
std::map< _currentState_t, std::vector< std::pair< Event, _newState_t > > > rules;
```
The `std::vector< std::pair< Event, _newState_t > >` allows transition to multiple new states from a given current state based on different values taken on by the event.

## Reference: https://github.com/peter-can-talk/cppnow-2017
## Reference: https://github.com/lanl/CoARCT/blob/clang-9.0
## Reference: https://devblogs.microsoft.com/cppblog/exploring-clang-tooling-part-2-examining-the-clang-ast-with-clang-query/
## Reference: https://gist.github.com/raphaelmor/3150866
