make [S]name: "So..."{END{
    Wrap{
        quotes[]
    }
    Reform{
        alloc: 64
        PushValue{
            To: START
            Value: "Hello!\n"
            To: END
            Value: "I added in some new stuff :D"
        }
    }
    reference: n
}};
print[any](name,n);