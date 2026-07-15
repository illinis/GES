/*
this file contains notes about *behaviors* (portability issues) in C;

definitions:

the C standard
	: an aggregate of 'laws', a group of directives formally written to guide the C programming language definitions across a variety of implementations.

implementation-defined behavior
	: program behavior that is not specified by the C standard and that may produce different results between implementations but has consistent,documented behavior within an implementation.
	
unspecified behavior
	: program behavior for which the standard provides two or more options but doesn't mandate which option is chosen in any instance.
	
undefined behavior
	: program behavior that isn't defined by the C standard.
	: "The C standard intentionally makes no promises whatsoever about what should happen."
	
locale-specific behavior
	: behavior depends on local conventions of nationality, culture and language that each implementation documents.
	
common extensions
	: widely used in many systems but are not portable to all implementations.

*/