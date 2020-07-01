make [S]name: "So..."{END{
    Wrap{
        quotes::[TERMINATE]
    }
    Reform{
        alloc: 64
        # TO BE
        -# 
            alloc START: ammount
            alloc END: ammount
            alloc: ammount - sets "ammount" to both START and END
        #-
        PushValue{
            To: START
            Value: "Hello!\n"
            To: END
            Value: "I added in some new stuff :D\0"
        }
    }
    reference: n
}};
make [S]namE: brand namE{
  memalloc
};
make [I]age: 15-#{signed}#-;
print[str](name,n);
print[any](21);
make [S]GithubName: "ARACADERISE"{END{Wrap{quotes}}Tab:1};