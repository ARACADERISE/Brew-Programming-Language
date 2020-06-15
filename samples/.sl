/* Simple Lang info */

/* A multi line comment! */
# A single line comment!

make [S]MyName: "Aidan"; # A variable of type String
print[any](MyName); # print statement able to print any type, in this case printing MyName

/* 
    This is a constant variable in the .sl language.
*/
varconst{VarName}:"VarValue"; /* 
    NOTE: By default, a varconst variable is of type "any".
    Meaning, you can assign a integer value, a char value,
    a string value etc without telling the variable a certain type.
*/

/* 
    In order to print a varconst variable, we use the Print function.
*/
Print(VarName);

/* 
    Branding a variable basically means you're setting aside a block of memory for it.

    Here is an example:
*/

# we are going to use the varconst
# variable declaration as our example
/* 
    NOTE: We are branding the variable at declaration time.
    Since we do this, we have to put the name before the start of the brand method.
*/
varconst{Name}: brand Name {
    memalloc(32); # allocating 32 bits for a string.
};

/* 
  FUTURE NOTE:
    I want functions to look like the following:
      function(/*Args Here(Will get to this part wayyy later)*/) {
        /* Function Code. */
      } FunctionName -> FunctionReturnType;
    
    Example:
    function(x,y) {
      ret x * y;
    } Multiply -> int;
*/