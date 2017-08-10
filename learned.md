### Libraries and tools

#### STL

- C++ <random> utilities
- C++ <sstream> utilities (namely ostringstream)
- C++ exceptions
- file streams
- <algorithm> find_if, find, count, count_if
- using `std::advance` rather than a for loop to move an iterator to a desired position
- `std::swap` to exchange container elements
- variadic functions through "recursion" and partial template specialization

#### JSON for modern C++

- `at()`, `find()`

#### Makefiles

- wildcards `$(wildcard *.c)`


### Language features

- scoped enums using the "class" keyword
- friend classes and functions
- raw strings R"()"
- abstract classes and interfaces using virtual and pure virtual functions
- 'default' fns: use compiler-generated version of the fn
- declaring a nested class as private allows it to be accessed but not instatiated outside the containing class
- lambdas: 
  - default: `[] (params) { body }`
  - no params: `[] { body }`
  - explicit return type: `[] (params) -> return_type { body }`
  - capture external vars by reference: `[&] (params) { body }`
  - capture external vars by copy: `[=] (params) { body }`
- exceptions:
  - if no handler, exception within try will terminate the try block plus the function containing the try block
  - can catch anything using `catch (...)`
  - can throw exception objects, primitive types, or not throw at all
  - throwing within a nested handler will activate the containing try block's handler
### Algorithms

### Gotchas

- protecting against self-assignment in assignment operator overloads
- can't have an std::vector of references
- rule of three
- visibility between ctor initializer list members is determined by their order in the class prototype
- template class implementation must be .h file
- member function const-ness:

```
// won't compile because |cat| from the lambda is not const
std::list<level_pack::category>::iterator level_pack::find_category(std::string name) const {
    return std::find_if(m_categories.begin(), m_categories.end(),
        [&name] (category& cat) { return cat.name == name; }
    );
}
```

```
// won't compile because the function returns a non-const iterator when |cat| from the lambda is const
std::list<level_pack::category>::iterator level_pack::find_category(std::string name) const {
    return std::find_if(m_categories.begin(), m_categories.end(),
        [&name] (const category& cat) { return cat.name == name; }
    );
}
```

```
// compiles since all const
std::list<level_pack::category>::const_iterator level_pack::find_category(std::string name) const {
    return std::find_if(m_categories.begin(), m_categories.end(),
        [&name] (const category& cat) { return cat.name == name; }
    );
}
```

```
// compiles since all non-const
std::list<level_pack::category>::iterator level_pack::find_category(std::string name) {
    return std::find_if(m_categories.begin(), m_categories.end(),
        [&name] (category& cat) { return cat.name == name; }
    );
}
```
- for checking exception cases, begin each case with an `if` statement rather than `else`/`else if` so that they don't
get skipped over
- stl iostream was created before exceptions and is therefore not exception-friendly
- `std::unordered_set`: provide a custom hasher, either thru a function or an override of `std::hash<T>::operator()()`
