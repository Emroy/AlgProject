# AlgProject
Software Development for Algorithmic Problems Project

###Team Members
* Lelegiannis Ioannis: 1115201200090
* Poulidis Nikolaos: 1115200000111

##Code Units
####List
A simple, general purpose linked list.  
The functions contained are as follows:  
* _**List list_create()**_  
  Returns a pointer to a list or NULL on failure.  
* _**void list_destroy(List l)**_  
  Destroy a list and set l to NULL.  
  Data contained in the list on this call will **NOT** be freed.  
  It is the caller's responsibility to have access to those data
  after the call in order to free them, or have freed them beforehand.  
  Empty lists _still_ need to be destroyed with this function.  
* _**void list_pushEnd(List l,void* data)**_  
  Attach a new element to the end of the list.  
* _**void* list_pop(List l)**_  
  Get the first element of the list and destroy the first node.  
  If the list is empty, NULL is returned instead.  
* _**int list_isEmpty(List l)**_  
  Returns a non-zero integer if the list is empty or zero if the list isn't empty.  
* _**unsigned int list_length(List l)**_  
  Returns the number of elements the list contains.  
  
**->**Note that all of the above functions have undefined behavior in case List l was
not created using list_create before the call.  
  
####HashTable
A General Purpose HashTable that works in conjecture with HashGen hash functions (described bellow).  
It works like an index to the data pointers that are passed to it.
The functions contained are as follows:  
* _**HashTable hashTable_create(int size,HashDescriptor hd)**_  
  Creates a new Hash Table with empty buckets or NULL on failure.  
  In case hd was not created using a hash_create function (see bellow)
  the behavior is undefined.  
  **It is the caller's responsibility to make the hash function
  able to accept the type of data that will be stored inside the table.**  
* _**void hashTable_destroy(HashTable ht)**_  
  Frees all allocated memory for given Hash Table.  
  Does not free memory pointed to by the pointers contained in the hash table.  
* _**int hashTable_insert(HashTable ht,void* data)**_  
  Inserts whatever data points to on Hash Table ht and returns 0.  
  A negative number is returned on failure.  
* _**void* hashTable_getNext(HashTable ht,void* q)**_  
  Returns next element from bucket corresponding to element q.  
  Returns NULL when there is no next element.  
  After initial call, parameter void* q is ignored until NULL is returned.  

**->**Note that all of the above functions have undefined behavior in case HashTable ht was
not created using hashTable_create before the call.  

####HashGen
A unit used to create hash functions and apply them.  
It's main purpose is to be used in conjecture with HashTable unit described above.  
It's functions are as follows:  
* _**unsigned int hash_apply(HashDescriptor hd,Data x)**_  
  Aplly function described by HashDescriptor on element x and return the ouput.  
* _**HashDescriptor hamming_hash_create(int d,int k)**_  
  Creates a hamming function g as described by theory.  
  Returns NULL on failure.  
* _**int hamming_is_equal(HashDescriptor a,HashDescriptor b)**_  
  Checks if HashDescriptor a and HashDescriptor b, that describe hamming hash functions, are equal.  
  Returns 1 if they are and 0 if not.  
* _**void hamming_hash_destroy(HashDescriptor hd)**_  
  Destory a hamming HashDescriptor and make it NULL.  
* _**HashDescriptor euclidean_hash_create(int d,int k,int n)**_  
  Creates an euclidean function g as described by theory or NULL on failure.  
  n: number of total elements in query. Used to determine TableSize. See theory for more info.  
* _**int euclidean_is_equal(HashDescriptor a,HashDescriptor b)**_  
  If HashDescriptor function a is the same as HashDescriptor function b returns 1 else returns 0.
* _**void euclidean_hash_destroy(HashDescriptor hd)**_  
  Destroy an euclidean HashDescriptor and make it NULL.  
* _**HashDescriptor cosine_hash_create(int d,int k)**_  
  Creates a cosine function g as described by theory or NULL on failure.  
* _**int cosine_is_equal(HashDescriptor a,HashDescriptor b)**_  
  If HashDescriptor function a is the same as HashDescriptor function b returns 1 else returns 0.  
* _**void cosine_hash_destroy(HashDescriptor hd)**_  
  Destroy a cosine HashDescriptor and make it NULL.  
* _**HashDescriptor matrix_hash_create(int k,int n)**_  
  Creates a matrix function g as described by theory or NULL on failure.  
* _**int matrix_is_equal(HashDescriptor a,HashDescriptor b)**_  
  If HashDescriptor function a is the same as HashDescriptor function b returns 1 else returns 0.
* _**void matrix_hash_destroy(HashDescriptor hd)**_  
  Destroy a matrix HashDescriptor and make it NULL.  
  
**->**Note that all of the functions that take a HashDescriptor as a parameter require that that
HashDescriptor was created with the coorespoding create funtion. If not the behavior is undefined.

####RNG
Random number generation distribution functions.  
They are described more specifically bellow:  
* _**int integerUniform(int n)**_  
  If n is possitive, the function returns a random natural value on [0,n] following Uniform distribution.  
  If n is negative, the function returns a random value on [-(1.0/2.0)*RAND_MAX,(1.0/2.0)*RAND_MAX].  
* _**float realUniform(float a,float b,int token)**_  
  Returns a random real values following Uniform distribution.  
  If token is 0 space is (a,b).  
  If token is 1 space is (a,b].  
  If token is 2 space is [a,b).  
  If token is 3 space is [a,b].  
* _**float Gaussian01(void)**_  
  Returns a random real value following Gaussian Distribution on space (0,1).  
  
####data
Data accepts strings formated as described in the input files and creates cooresponding data and queries.  
There are a number of functions mainly to access and mutate the structures holding the data.  
See data.h for more specific information.  

####Algorithms  
Algorithms contains initialization, assignment and update algorithms used for k-medoids.  
It also contains some utility functions used to produce output and mutate a struct that holds the medoids.  

##Main Files and Execution

####lsh
lsh executable is called from command line as follows:  
./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file>  
where –d <input file>, –q <query file> and -ο <output file> are mandatory while
–k <int> and -L <int> are optional and ahve default values 4 and 5 respectively.  

####medoids
medoids executable is called from command line as follows:  
./medoids –d <input file> –c <configuration file> -o <output file>  -complete
Input and Configuration files are mandatory, while Output file has default value "./Output"  
-complete option is optional and if given the program will output specifically the clusters
instead of just outputing the size and medoids.  

###Compilation and Makefile
* make: compiles using the main function in medoids.c
* make lsh: compiles using the main function in lsh.c
* make clean: removes files and executables created by the above commands
