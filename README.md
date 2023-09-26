# stack
 my stack realization
 ## Install
 ```
 make install
 ```
 Compile
 ```
 make
 ```
 ## Functions
StackCtor - creates stack
StackDtor - destructs stack
StackPush - adds element in stack
StackPop  - gets element from stack
StackDump - prints all info about your stack
## Protection modes
### Canary protection
Stack and data have canary_t elements before and after them.
These canary elements must always be equal to canary_val (0xDEADDEAD for my program).
If canary was changed (some array could cross it's borders and start to change data of other elements),
it means that stack or data may be not correct and program returns error.
### Hash protection
We can count hash for stack and it's data with hash_function (we can choose it as a parameter of stack,
but default hash funtion is MurmurHash). We save counted hash as structure elements, then every stack function counts hashes again, and compares them with saved hashes. If they are not equal, that means that some external funtion changed stack, and it is not correct now. In this case program returns error. (Every stack function in the end updates hashes and saves their calues in structure, before it returns some value).
