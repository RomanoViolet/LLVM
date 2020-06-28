# Demo: LLVM C-Bindings for Parsing C++ Sources

## Objective
The project demonstrates the use [LibClang](https://clang.llvm.org/docs/Tooling.html) interface of the LLVM infrastructure for parsing C++ sources.
For this project, a [header file](./TestVectors/Component.hpp) is used as a typical example of a C++ source to be parsed.

## What To Expect From This Project 
A method to parse a C++ source with known layout, using LLVM tools. 
After parsing the header file, the output generated is like so:

The [header file](./TestVectors/Component.hpp) uses templates specifically written for declaring the direction of information carried by a public member e.g., `::RomanoViolet::TypeInputInterface` is used to declare a symbol carrying input information.
The objective of the parsing the [header file](./TestVectors/Component.hpp) is to:
* Summarize the base-class that the class (i.e., `Component`) inherits from;
* Summarize data members used for providing input (to the instance of this class)
* Summarize data members used for communicating the output (from the instance of this class)
* Any data members which have ambiguous direction

An alternate approach is to simply declare all input data carrying members as `const` reference types.
The template approach allows the library (e.g., `::RomanoViolet::TypeInputInterface`) to provide its customers convenience methods expected for any input, e.g., compute running average of input values.

After processing the [header file](./TestVectors/Component.hpp), the following result is generated:
```bash 
Class Name:         NN::RomanoViolet::Component
Base Class:              TypeHighAssuranceComponent
                  Input: a_in           Type:  RomanoViolet::InterfaceA
                 Output: b_in           Type:  RomanoViolet::InterfaceB
                 Output: b_out          Type:  RomanoViolet::InterfaceB
              Ambiguous: b2             Type:  RomanoViolet::InterfaceB
              Ambiguous: c              Type:  int
```
The `print` method used for generating the text is part of the class `RomanoViolet::StateMachine` user-written C++ parser class.


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
Ensure that relevant compiler(s) along with LLVM development libraries are available on your system.
Execute the provided shell script, making it executable if necessary first:
```bash
./buildProject.sh -s 11 -c LLVM -b debug
```
To get available build options, run the `buildProject.sh` without arguments:
```bash
./buildProject.sh
```
### Option 2: Docker Based Tools
All compilers, libraries, and tools used in the development of this project are available in a container. See provided [Dockerfile](./.devcontainer/Dockerfile) for details.
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
The parser is implemented in the [`RomanoViolet::StateMachine`](./CoreFunctions/Application/StateMachine.hpp) class.
The parser is instantiated from the client(or user) code implemented in [ParseHeader.cpp](./CoreFunctions/Application/ParseHeader.cpp) in line 159:

```c++
RomanoViolet::StateMachine p{ data._classDetails._name };
```
where the initialization parameter is the full class name to be parsed (e.g., `NN::RomanoViolet::Component`, see [test header file](./TestVectors/Component.hpp)).

Once instantiated, the parser instance 	`p` is added to the data structure `data` in line 180 of `ParseHeader.cpp`:
```c++
data.p = &p;
```

### The `visit` Function
LLVM parsing tools allow the user to implement a `visit` function which is called to parse the provided C++ sources.
All functions available in the LLVM library are available for use in the user-written `visit` function. 
The user (client) written `visit` function is required to have the following signature:
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

### `data` and `p`
A pointer to the data structure `data` forms the _opaque pointer_ which may be passed to the (client written) `visit` function in order to store results persistently across multiple calls to the `visit` function.



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

#### Transition Rules For State Machine
The rules are written after observing the result produced by an LLVM parser, like the one written by [Peter Goldsborough](https://github.com/peter-can-talk/cppnow-2017). One of [Peter's example](./CoreFunctions/Application/AstDumpOrig.cpp) has been adapted for this project to generate the result of an LLVM parser which generates text like shown below (partial listing):

```bash
|-Namespace 127891415 <line:9:1, line:35:1> line:9:11 NN  
| `-Namespace 3952112176 <line:11:3, line:34:3> line:11:13 RomanoViolet  
|   `-ClassDecl 2340098849 <line:13:5, line:33:5> line:13:11 Component NN::RomanoViolet::Component
|     |-C++ base class specifier 3782358474 <col:23, col:55> col:30 3819089211 class TypeHighAssuranceComponent TypeHighAssuranceComponent
|     | `-TypeRef 3989614820 <col:30, col:55> col:30 3819089211 class TypeHighAssuranceComponent TypeHighAssuranceComponent
|     |-CXXAccessSpecifier 3646394674 <line:15:5, col:11> col:5   
|     |-EnumDecl 1437190249 <line:16:7, col:73> col:18 ErrorCode NN::RomanoViolet::Component::ErrorCode
|     | |-EnumConstantDecl 3935002420 <col:38, col:50> col:38 NO_ERROR NN::RomanoViolet::Component::ErrorCode
|     | | `-UnexposedExpr 3742966453 <col:49, col:50> col:49  short
|     | |   `-IntegerLiteral 829964490 <col:49, col:50> col:49  unsigned int
|     | `-EnumConstantDecl 153294274 <col:53, col:71> col:53 BAD_INPUT_DATA NN::RomanoViolet::Component::ErrorCode
|     |   `-UnexposedExpr 3962916194 <col:70, col:71> col:70  short
|     |     `-IntegerLiteral 4135885179 <col:70, col:71> col:70  unsigned int
|     |-CXXConstructor 235937490 <line:18:7, col:18> col:7 Component void ()
|     |-FieldDecl 36711781 <line:23:7, col:75> col:72 a_in ::RomanoViolet::TypeInputInterface< ::RomanoViolet::InterfaceA>
|     | |-NamespaceRef 984821533 <col:9, col:20> col:9 4078261751 RomanoViolet  
|     | |-TemplateRef 2165410661 <col:23, col:40> col:23 501271699 TypeInputInterface  
|     | |-NamespaceRef 984821533 <col:45, col:56> col:45 4078261751 RomanoViolet  
|     | `-TypeRef 4261379432 <col:59, col:68> col:59 3981686915 class RomanoViolet::InterfaceA RomanoViolet::InterfaceA
|     |-FieldDecl 308592855 <line:24:7, col:77> col:73 b_out ::RomanoViolet::TypeOutputInterface< ::RomanoViolet::InterfaceB>
|     | |-NamespaceRef 984821533 <col:9, col:20> col:9 4078261751 RomanoViolet  
|     | |-TemplateRef 3773219427 <col:23, col:41> col:23 3266196948 TypeOutputInterface  
|     | |-NamespaceRef 984821533 <col:46, col:57> col:46 4078261751 RomanoViolet  
|     | `-TypeRef 2706993704 <col:60, col:69> col:60 2608659120 class RomanoViolet::InterfaceB RomanoViolet::InterfaceB
|     |-CXXMethod 4116592367 <line:26:7, col:24> col:12 initialize void ()
|     |-CXXMethod 2007754990 <line:27:7, col:33> col:12 doPreconditionCheck void ()
|     |-CXXMethod 1988825022 <line:28:7, col:21> col:12 compute void ()
|     |-CXXMethod 1066151573 <line:29:7, col:34> col:12 doPostConditionCheck void ()
|     |-CXXAccessSpecifier 1532104156 <line:31:5, col:12> col:5   
|     `-FieldDecl 3633548769 <line:32:7, col:22> col:17 _error NN::RomanoViolet::Component::ErrorCode
|       `-TypeRef 1086425527 <col:7, col:15> col:7 1437190249 enum NN::RomanoViolet::Component::ErrorCode NN::RomanoViolet::Component::ErrorCode
```

The indention denotes the specialization of a token, (e.g., Namespace &#8594; Namespace &#8594; ClassDecl) wherein the fully qualified name of the class is `NN::RomanoViolet::Component` (third line of the AST above).
In the case at hand above, correctly inferring the class name requires the knowledge of the context immediately before the `ClassDecl` token is seen (i.e., two `Namespace` tokens). This project uses a state-machine to assimilate the context related a token of interest, with the transition between states triggered by the AST token received (e.g., `Namespace`).
The final set of state transition rules required to parse the sample [header file](./TestVectors/Component.hpp) are encoded in the constructor of the [`RomanoViolet::StateMachine`](./CoreFunctions/Application/StateMachine.cpp), like so:

```c++
this->rules[State::INIT] = { { Event::NAMESPACE, State::NAMESPACE_COLLECTION },
                                 { Event::CLASS_DECLARATION, State::CLASSNAME_COLLECTION } };
```


## Reference: 
https://github.com/peter-can-talk/cppnow-2017
