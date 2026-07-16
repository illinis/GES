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
	
	file
		:
		
	block
		:
		
	function prototype
		:
		
	function
		:
	
stopped on page 34.	

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