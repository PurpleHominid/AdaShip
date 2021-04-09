#pragma once //ensure header only included once during compilation

//	the general format is:
//	^ : start of string
//	[ : beginning of character group
//	a-z : any lowercase letter
//	A-Z : any uppercase letter
//	0-9 : any digit
//	  : space
//	_ : underscore
//	- : dash - there are special 'metacharacters' that need to be protected using a backslash; these are: . - ? ^ \ $ | * + ( ) [ ] { }
//	. : dot 
//	! : exclamation
//	] : end of character group
//	* : zero or more of the given characters
//	$ : end of string

//define regular expression patterns
const std::regex REGEXP_ALPHA("^[a-zA-Z]*$");
const std::regex REGEXP_NUMERIC("^[0-9]*$");
const std::regex REGEXP_NUMERICONLY("^[0-9]+$");
const std::regex REGEXP_NUMERICONLY0_6("^[0-6]+$");
const std::regex REGEXP_ALPHANUMERIC("^[a-zA-Z0-9 ]*$");
const std::regex REGEXP_NUMBER("^[0-9\\.\\-]*$");
const std::regex REGEXP_ALPHANUMERIC_EXT("^[a-zA-Z0-9_\\-\\.]*$");

//const std::regex REGEXP_BOATPLACEMENT("^[a-zA-Z0-9 ]*$");
const std::regex REGEXP_BOATPLACEMENT("^([0-9]{1,2} ?[a-zA-Z]{1,2}[0-9]{1,2} ?[vVhH])+");
const std::regex REGEXP_COORDINATES("^([a-zA-Z]{1,2}[0-9]{1,2} ?)+");

// (  )   : define a capture patten
// {i, n} : i of n letters/digits
//  ?     : ? used to indicate an optional character that preceeds the ?
// +      : indicates one or more instances

const std::regex REGEXP_ANY(".*");

const std::regex REGEXP_LEADINGSPACES("^\\s+");
const std::regex REGEXP_TRAILINGSPACES("\\s+$");