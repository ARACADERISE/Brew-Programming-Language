-#
  First Beta Version Project In The Brew Language(BPL)!
#-

# Making a variable
make [S]MyGithubName: "ARACADERISE\nTHIS\nis\nHOW\nit\nIS\ndone"{END{
    Wrap{quotes} -#
        just having the keyword quotes assigns one set of "" around the string MyGithubName.
        NOTE: Having a odd number to the value quotes results in an error.
        quotes keyword, when assigning ammounts, is same as the breaks keyword:
        quotes[2] meaning we want """" around our string.
    #-
} Tab:3};
make [I]myage: 26;
# make [C]startsWith: 'A';

# printing my Github name and my age
print[any](myage,MyGithubName,MyGithubName);