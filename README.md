# preeny

Preeny helps you pwn noobs by making it easier to interact with services locally.
It disables `fork()`, `rand()`, and `alarm()` and, if you want, can convert a server application to a console one using clever/hackish tricks, and can even patch binaries!

## Building

preeny's patch functionality uses `libini_config` to read `.ini` files.
On debian-based distros, you can install `libini-config-dev`.
If you're not running a debian-based distro, you've brought the pain upon yourself.

You can build preeny by doing `make`.
It'll create a directory named after the OS and architecture type, and put the libraries there.

## Usage

Let's say that you have an application that you want to interact with on the commandline, but it a) forks, b) sets an alarm which makes it hard to take your time studying its behavior, and c) demands to be connected to even if you don't want to do that.
You can do:

```bash
make
LD_PRELOAD="Linux_x86/desock.so Linux_x86_64/defork.so Linux_x86_64/dealarm.so" ~/code/security/codegate/2015/rodent/rodent
```

Pretty awesome stuff!
Of course, you can pick and choose which preloads you want:

```bash
echo 'No fork or alarm for you, but I still want to netcat!'
LD_PRELOAD="Linux_x86_64/defork.so Linux_x86_64/dealarm.so" ~/code/security/codegate/2015/rodent/rodent

echo 'Ok, go ahead and fork, but no alarm. Time to brute force that canary.'
LD_PRELOAD="Linux_x86_64/dealarm.so" ~/code/security/codegate/2015/rodent/rodent
```

Have fun!

## Simple Things

The simple functionality in preeny is disabling of fork and alarm.
CTF services frequently use alarm to help mitigate hung connections from players, but this has the effect of being frustrating when you're debugging the service.
Fork is sometimes frustrating because some tools are unable to follow fork on some platforms and, when they do follow fork, the parent is oftentimes abandoned in the background, needing to be terminated manually afterwards.
Random is random.

`dealarm.so` replaces `alarm()` with a function that just does a `return 0`.
`defork.so` does the same thing to `fork()`, means that the program will think that the fork has succeeded and that it's the child.
`derand.so` replaces `rand()` and `random()` with a `return 42;`.

## De-socketing

Certain tools (such as American Fuzzy Lop, for example) are unable to handle network binaries.
To mitigate this, `desock.so` neuters `socket()`, `bind()`, `listen()`, and `accept()`, making it return sockets that are, through hackish ways, synchronized to `stdin` and `stdout`.

## Preload patching

`patch.so` patches binaries!
This is done before program start, by triggering the patcher from a constructor function in `patch.so`.
Patches are specified in a `.ini` format, and applied by including `patch.so` in `LD_PRELOAD` and providing a patch file specified by the `PATCH` environment variable.
For example:

```ShellSession
# tests/hello 
Hello world!
# cat hello.p 
[hello]
address=0x4005c4
content='4141414141'

[world]
address=0x4005ca
content='6161616161'
# PATCH="hello.p" LD_PRELOAD="Linux_x86_64/patch.so" tests/hello 
--- section hello in file hello.p specifies 5-byte patch at 0x4005c4
--- section world in file hello.p specifies 5-byte patch at 0x4005ca
AAAAA aaaaa!

```

Having different patch files and just enabling/disabling them via preload is oftentimes easier than modifying the underlying binary.
