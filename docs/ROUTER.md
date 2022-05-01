### Router
`cereal_pack` provides a router class that can be used to work with messages
created with `cereal_pack`. It is small and simple to use
```C++
using namespace cereal_pack::router;
BasicRouter<MyCustomHeader> r([] (const MyCustomHeader& h) { return h.body_name().get(); });

r.attatch_route<ImportantEvent>([](const ImportantEvent& event) {
    // TODO handle important event
});

r.attatch_route<UrgentEvent>([](const UrgentEvent& event) {
    // TODO handle urgent event
});

while (true) {
    std::vector<uint8_t> cereal_data = read_ipc();
    r.handle_message(cereal_data.data());
}
```
It's up to you to define how you want your message headers to look,
you just need to tell `cereal_pack` how to read your header.

First, create a new schema that will serve as your header:
```toml
name = "MyCustomHeader"

[props]
    # since we are using the `BasicRouter` in this example
    # we just need to define a string property that can hold
    # a schema name
    [props.body_name]
    type = "string"
    max_length = "64"

    # you can add any other extra fields you like
    [props.dispatched_at]
    type = "uint64_t"

    [props.etc]
    type = "uint32_t"

```

Now in your C++ code, create a `cereal_pack::router::BasicRouter<MyCustomHeader>`
```C++
using namespace cereal_pack::router;
BasicRouter<MyCustomHeader> router([] (const MyCustomHeader& header) {
    // this callback defines how the router should get the body_name property
    // from our custom header
    return header.body_name().get();
});
```

Now we can attatch a handler for each of our schema types
```C++
bool success = router.attatch_route<ImportantEvent>[](const ImportantEvent& ev) {
    std::cout << "Received an important event!" << std::endl;
    // TODO: do something when this important event happens
}

if (!success) {
    std::cout << "Unable to attatch route!" << std::endl
              << "This means that a handler is already attatched for this type" << std::endl;
}
```

And finally we can pass a message to the handler and it will get handled by our route
```C++
router.handle_message(serialized_msg);
```

#### But using the names of schemas is so inefficient!!!!
Well, yeah it is a bit. However, the names are auto-generated, and likely won't get
updated in a way that could break backwards compatibility.

However, if you _really_ need these to be as compact as possible, you can actually
use any key you like with the `Router<Header, Key>` class instead
(which is the parent class of `BasicRouter`)

Just define your header in the same way as before. The only difference is you must
supply the key for a message type when attaching the route.
```C++
Router<MyCustomHeader, EventEnum> router ([] const MyCustomHeader& h) { return h.event().get(); });

router.attatch_route<ImportantEvent>(EventEnum::Important, [](const ImportantEvent& event) {
    // etc...
};
```

