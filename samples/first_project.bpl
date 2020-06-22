make [S]MyGithubName: "ARACADERISE"{END{
    Wrap{quotes breaks}
    Reform{
        alloc: 32
        PushValue{
            To: START
            Value: "Hi. I am "
            To: END
            Value: "and this is my PROGRAMMING LANGUAGE!(Called Brew)"
        }
    }
    reference: MGN
} Tab: 0};
make [I]age: 16;
print[any](MyGithubName,age,MGN);