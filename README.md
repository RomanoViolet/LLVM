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
| Docker         | 19.03.12 |

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

with [rules]((./CoreFunctions/Application/StateMachine.hpp) declare a data structure to encode state-transitions from _current state_ to a _new state_ based on an _Event_:
```c++
std::map< _currentState_t, std::vector< std::pair< Event, _newState_t > > > rules;
```
The `std::vector< std::pair< Event, _newState_t > >` allows transition to multiple new states from a given current state based on different values taken on by the event.

## Reference: https://github.com/peter-can-talk/cppnow-2017
## Reference: https://github.com/lanl/CoARCT/blob/clang-9.0
## Reference: https://devblogs.microsoft.com/cppblog/exploring-clang-tooling-part-2-examining-the-clang-ast-with-clang-query/
## Reference: https://gist.github.com/raphaelmor/3150866
