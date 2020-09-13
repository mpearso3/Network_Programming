#### Multi-threaded Learning

### Lesson 1
* Simple example using threads to have two functions running concurrently
* Uses pthread_create (no args or attributes)
* Uses pthread_join (no return arg)

### Lesson 2
* Simple example using threads to have two functions running concurrently with args and return values
* Uses pthread_create (passes a simple int as an arg, no attributes)
* Uses pthread_join (returns an int\* as an example))

### Lesson 3
* Example using two threads to count to 2 billion (shows why threads can slow a program down)
* Uses pthread_create
* Uses pthread_join
* Uses mutex to lock/unlock critical section (contains a global variable)
