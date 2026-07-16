/*
    C Chapters 1 and 2 Review Questions

    Instructions:
    Write your answers below each question.
    Try to answer without compiling code or consulting the book first.
*/

/* =========================================================================
   OBJECTS, TYPES, AND FUNCTIONS
   ========================================================================= */

/*
1. Is a function an object in C? Why?

Answer: No. A function is not an object because it does not have a reserved space in memory to store data. A function is literally a piece of executable code.

*/

/*
2. Why is this declaration illegal?

       int f()[10];

Answer: This declaration is wrong because it defines f as an 'array' of functions that return an int value without using a '*' (turning the variable into a pointer). 

*/

/*
3. Can a function return an array? Why?

Answer: No, it can't. I don't know why. 

*/

/*
4. Can a function return a pointer to an array?

Answer: Yes, it can.

*/

/*
5. Why are arrays not assignable in C?

Answer: Because an array name is not a modifiable lvalue.

*/


/* =========================================================================
   POINTERS
   ========================================================================= */

/*
6. Explain exactly what this declaration means:

       int *const p;

Answer: This is a pointer to an int value that is constant, id est, the actual value of p (in this occasion a memory address of an integer variable) is not modifiable. 

*/

/*
7. Explain exactly what this declaration means:

       const int *p;

Answer: This line declares a pointer to a const int variable, id est, the address which the pointer is pointing have the type 'const int'. The pointer itself could be changed in runtime execution.

*/

/*
8. Explain exactly what this declaration means:

       const int *const p;

Answer: It is the addition of the two properties explained in questions 6 and 7. This line declares a pointer to a const int variable, that is constant. The pointer (the literal address of the const int variable) is not mutable (if that's a valid word for the context).

*/

/*
9. What is the type of the expression *p if p is declared as follows?

       int *p;

Answer: The type of the '*p' expression would be int, since *p dereferences the pointer, revealing the value of the variable it points to.

*/

/*
10. Why can we not write the following expression?

        &(a + b)

Answer: I am not sure about this one, but the (a + b) operation does not designate a value to a specific memory address, making the reference of this constant value (literally the sum of the two variables a and b) illegal.

*/


/* =========================================================================
   STORAGE DURATION
   ========================================================================= */

/*
11. Name all storage durations in C.

    When is an object with each storage duration created?
    When is it destroyed?

Answer: auto, static, thread and allocated. 

*/

/*
12. What is the difference between these two declarations?

    Inside a function:

        static int x;

    Outside every function:

        static int x;

Answer: When declaring static variables inside functions, you're basically expanding the lifetime of the variable you created so it does not disappears with the end of the function execution. When declaring static variables outside every function, you're declaring that this variable must not be accessed by any other code outside the file which the variable is defined.

*/

/*
13. Does static always mean "global"? Explain.

Answer: Not always. Static also is used to declare variables with file-scope.

*/


/* =========================================================================
   SCOPE AND LINKAGE
   ========================================================================= */

/*
14. What is the difference between:

    - scope
    - lifetime
    - linkage

Answer: Scope is what defines an object's lifetime. Lifetime is the 'area' (or time) between the initial instance of the object and its death. Linkage defines 'who' can access the object. 

*/

/*
15. Can an object continue to exist after its identifier goes out of scope?

    Give an example.

Answer: Yes. A static variable defined inside a function continue to exist with its old value even after the conclusion of the function.

*/


/* =========================================================================
   TYPEDEF
   ========================================================================= */

/*
16. Does this declaration create a new type?

        typedef int Integer;

    Explain.

Answer: Absolutely no. This declaration creates an alias for an already existing type or object.

*/

/*
17. Why can the following typedef sometimes be misleading or dangerous?

        typedef char *String;

Answer: 

*/


/* =========================================================================
   STRUCTURES AND UNIONS
   ========================================================================= */

/*
18. Why can sizeof(struct_type) be larger than the sum of the sizes of
    its individual members?

Answer: In order to guarantee memory alignment, the compiler may allocate some padding bits for the defined structure, depending on the present struct's member types.

*/

/*
19. Why does structure padding exist?

Answer: It exists so the memory stays aligned, causing the CPU to process read/write instructions easier. 

*/

/*
20. When would you choose a union instead of a struct?

Answer: When I need structures with different types of nodes.

*/


/* =========================================================================
   CONST
   ========================================================================= */

/*
21. Consider this code:

        const int x = 5;
        int *p = (int *)&x;
        *p = 10;

    Is the final assignment valid?
    What is the behavior of the program?
    Explain why.

Answer: This final assignment is valid (accepted by the compiler), but defective. A program containg this kind of assignment have undefined behavior. Honestly, I don't remember why this happens.

*/

/*
22. Why is this function declaration often better:

        void print(const char *s);

    than this declaration:

        void print(char *s);

Answer: Since the function should not change the value of its parameter, using the const keyword is recommended to avoid possible unwanted changes. The const identifier would not be necessary if the parameter wasn't a  pointer.

*/


/* =========================================================================
   VOLATILE
   ========================================================================= */

/*
23. Why does volatile exist?

    Give two real-world situations in which it may be needed.

Answer: The volatile type qualifier exists to tag variables that are allowed to be changed by processes extrinsic to the compiler. Two examples are reading time from the operational system and using setjmp() and longjmp() functions.

*/

/*
24. Why must some automatic local variables modified between setjmp()
    and longjmp() be declared volatile?

Answer: So the compiler is 'notified' that these local variables are allowed to be changed by processes extrinsic to the compiler.

*/


/* =========================================================================
   RESTRICT
   ========================================================================= */

/*
25. Consider this function:

        void f(int *a, int *b);

    Why must the compiler normally assume that a and b may point to the
    same object?

Answer: Because there is a chance that both variables change the variable value. 

*/

/*
26. How does restrict change the compiler's assumptions about pointer
    aliasing?

Answer: Restrict assures to the compiler that the pointers doesn't point to one same object.

*/


/* =========================================================================
   PORTABILITY AND PROGRAM BEHAVIOR
   ========================================================================= */

/*
27. Give one example of each of the following:

    - undefined behavior
    - unspecified behavior
    - implementation-defined behavior

Answer: 
Undefined behavior: changing a const variable using a pointer.
Unspecified behavior: I don't know. 
Implementation-defined behavior: quantity of bytes in a int value.

*/

/*
28. Why does the C Standard intentionally leave some behavior undefined?

Answer: In order to give developers license.

*/


/* =========================================================================
   ALIGNMENT
   ========================================================================= */

/*
29. Consider this code:

        char buffer[32];
        struct S *p = (struct S *)buffer;

    Can p safely be used to access a struct S object?

    Explain all relevant issues, including alignment and object type.

Answer: p cannot be safely used to access a struct S object, since the memory reserved is still a composed of char addresses.

*/

/*
30. What problem does _Alignas solve?

    Does it solve every problem involved in treating raw storage as an
    object of another type?

Answer: I don't know.

*/


/* =========================================================================
   CHALLENGE QUESTIONS
   ========================================================================= */

/*
31. Why does C distinguish between:

    - an object
    - an identifier
    - a value

    instead of treating them as the same thing?

Answer:

*/

/*
32. Why does C have both the . operator and the -> operator instead of
    automatically dereferencing structure pointers?

Answer:

*/

/*
33. Suppose C passed everything by reference instead of by value.

    How would the language behave differently?
    What advantages could that provide?
    What disadvantages could it introduce?

Answer:

*/