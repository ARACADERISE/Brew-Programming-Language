make [S]MyGithubName: "ARACADERISE"{END{
    Wrap{quotes breaks}
    Reform{
        alloc: 32
        # The only reason this would possibly be used is for
        # allocating strict memory for the variable..idk.
        # this is just the beginning of the whole thing so I may find a
        # better use for PushValue..
        PushValue{
            To: START
            Value: "Hi My Name Is"
            To: END
            Value: "and this is my PROGRAMMING LANGUAGE!(Called Brew)"
        }
    }
    reference: MGN
} Tab: 1};
make [I]age: 16;
print[any](MyGithubName,age,MGN);