This is an idea that Brandon came up with, so he can have the blame.

Basically, this XORs the user's shellcode against a stream of random bytes that
changes every so often (maybe once/minute, but I haven't decided yet). The user
needs to figure out how to determine the random bytes accurately and encode
their shellcode before it resets again.
