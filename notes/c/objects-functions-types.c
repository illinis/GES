/*

"The first thing you’ll learn in this chapter is one of the
last things that I learned: every type in C is either an
object type or a function type."

object
	: storage in which you can represent values.
	: "Region of data storage in the execution environment, the contends of which can represent values."

variable
	: have a declared type that tells you the kind of object its value represents.
	
functions
	: not objects.
	: is characterized by both its return type and the number and types of its parameters.
	: can return any complete object type except for an array type.
	
	
variable declaration
	: assignment of a type and name (identifier).
	: optionally, the variable can be initialized in its declaration.

C is a 'call-by-value' language
	: when you provide an argument to a function, the value of that argument is copied into a distinct variable for use within the function
	: in order to repair this defect, we use the indirection operator (*)
	
the unary * operator denotes indirection
	: when used in a function declaration or definitio, * acts as part of a pointer declarator indicating that the parameter is a pointer to an object or function of a specific type.
	: if its operand has type pointer to T, the result of the operation has type T.
	: converts a pointer to a type T into a value of type T.
	
the unary & operator generates a pointer to its operand

enumerations (or enum)
	: allows you to define a type that assigns names to integer values in cases with an enumerable set of constant values.
	: if you don't specify a value to the first enumerator with the '=' operator, the value of its enumeration constant is 0, and each subsequent enumerator without an '=' adds 1 to the value of the previous enumeration constant.
	
void
	: means "cannot hold any value"
	: used to indicate that a function doesn't return a value.
	: the derived type 'void *' means that the pointer can reference any object.

array
	: contiguously allocated sequence of objects that all have the same element type.
	: characterized by their element types and the number of elements in the array.
	
typedef
	: used to declare an alias for an existing type.
	: it never creates a new type.
	
struct
	: contains sequentially allocated members.
	: members of the struct are referenced by using the structure member '.' operator.
	: pointers to structure members are referenced with the structure pointer '->' operator.

union
	: structures with memory used by the member objects overlaps.
	: can be not portable because implementations may differ.
	: referenced identically as the struct.

tags
	: special naming mechanism for structs, unions and enums.
	: not a type name and cannot be used to declare a variable.
	: when used alongside a typedef declaration, struct tags can be thought as type names.
		: this works fine except in the case of self-referential structures that contain pointers to themselves.
		: "You can use the same name for the tag and the typedef, but a common idiom is to name the tag something ugly such as tnode_ to encourage programmers to use the type name."

type qualifiers
	: can be used interchangeably as arguments to functions, return values from functions, and structure and union members.

	: const
		: not assignable but can have constant initializers.
		: the compiler can place objects with const-qualified type in read-only memory, and any attempt to write to them will result in a runtime error.
		: it is possible to change const-qualified objects using pointers.
		: C allows you to modify an object that is referenced by a const-qualified pointer by casting the const away.
		: be careful not to pass a const-qualified pointer to a function that modifies the object.
		
	: volatile
		: volatile-qualified type allow for processes that are extrinsic to the compiler.
		: the values stored in these objects may change without the knowledge of the compiler, or a write may synchronize externally.
		
	: restrict
		: is used to promote optimization.
		: promotes optimization by assuring to the compiler that two pointers (alias) doesn't interfere with each other.
		: aliasing hurts optimization because in situations where two pointers may alias, the compiler chooses to reload memory in order to not return the memory's old value.
	
scope
	: delimits the contiguous region where they can be accessed.
	: is determined by where it is declared.
	
	: file
		: declaration is outside any block or parameter list.
		: scope is the entire text file in which it appears as well as any included files.
		
	: block
		: declaration is inside a block or within the list of parameters
		
	: function prototype
		: declaration appears within the list of parameter declarations in a function prototype
		: terminates at end of the function declarator.
		
	: function
		: declarations between the opening of a function definition and its closing.

storage duration
	: objects have a store duration that determines their lifetime.
	: scope and lifetime are different concepts: scope applies to identifiers, whereas lifetime applies to objects.
	
	: automatic
		: objects declared within a block or as a function parameter.
		: ends when execution of the block completes.
		: if the block is entered recursively, a new object is created each time the block is entered, and each has its own storage.
		
	: static
		: objects declared in file scope have static storage duration.
		: lifetime of those objects is the entire execution of the program, and their stored value is initialized prior to program startup.
		
	: thread
		: used in concurrent programming
	
	: allocated
		: involves dynamically allocated memory

storage class specifiers
	: static
		: specify both storage duration and linkage
		: file scope identifiers specified with 'static' or 'constexpr', or functions specified as static, have internal linkage
		: declaring a variable with block scope to have static storage duration (using the storage-class specifier static) make it persist after the function has exited.
		
	: extern
		: specifies static storage duration and external linkage.
		: can be used with function and object declarations in both file and block scope.
	
	: thread_local
		: gives thread storage duration when used in declaration.
		: initialized is evaluated prior to the program execution.
		: lifetime is the entire execution of the thread for which it is created.
		: stored value is initialized with the previously determined value when the thread is started.
		
	: constexpr (C23)
		: declares scalar objects as constants that has its value permanently fixed at translation time.
		: may appear with 'auto', 'register' or 'static'
		: static size_t const BFO = 0x100000000;
		: constexpr size_t BFO = 0x100000000;
		
	: register
		: suggest that access to an object be as fast as possible.
		: can be used only for an object that never has its address taken.
		
	: typedef
		: defines an identifier to be a typedef name that denotes the type specified for the identifier.
		
	: auto
		: allowed only for objects declared at block scope, with exception to function parameter lists. (prior C23).
		: indicates automatic storage duration and no linkage.
		: after C23, when the declaration includes an initialized, the type can be derived directly from the type of the expression used to initialize the variable.
		
typeof operators
	: honestly idgaf about these
	
alignment
	: define the allowed adresses for the variable.
	: CPUs can access aligned objects more efficiently, and on some architectures misaligned access is illegal.
	
*/

#include <stdio.h>

void swap(int a, int b){
	int t = a;
	a = b;
	b = t;
	printf("a=%d\nb=%d\n\n", a, b);
}

void pointer_swap(int *a, int *b){
	int t = *a;
	*a = *b;
	*b = t;
	
	printf("a=%d\nb=%d\n\n", *a, *b);
}

int main(int argc, char* argv[]){
		int a = 21;
		int b = 17;
		
		int *pa = &a;N
		int *pb = &b;
		
		swap(a, b);
		printf("a=%d\nb=%d\n\n", a, b);
		
		
		pointer_swap(pa, pb);
		printf("a=%d\nb=%d\n\n", a, b);
		return 0;
}