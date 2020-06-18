-# A multi line comment! #-
# A single line comment!

make [S]MyName: "Aidan"; # A variable of type String
print[any](MyName); # print statement able to print any type, in this case printing MyName

-#
    This is a constant variable in the language.
#-
varconst{VarName}:"VarValue"; -# 
    NOTE: By default, a varconst variable is of type "any".
    Meaning, you can assign a integer value, a char value,
    a string value etc without telling the variable a certain type.
#-

-# 
    In order to print a varconst variable, we use the Print function.
#-
Print(VarName);

-# 
    Branding a variable basically means you're setting aside a block of memory for it.

    Here is an example:
#-

make [S]name: brand name {
    memalloc(32);
};

-# 
  FUTURE NOTE:
    I want functions to look like the following:
      function(Args Here(Will get to this part wayyy later)) {
        Function Code
      } FunctionName -> FunctionReturnType;
    
    Example:
    function([I]x,[I]y) {
      ret x mult y;
    } Multiply -> int;
#-