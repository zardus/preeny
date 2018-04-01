# preeny

Preeny helps you pwn noobs by making it easier to interact with services locally.
It disables `fork()`, `rand()`, and `alarm()` and, if you want, can convert a server application to a console one using clever/hackish tricks, and can even patch binaries!

Preeny has the following modules:

| Name | Summary |
|------|---------|
| dealarm | Disables alarm() |
| defork | Disables fork() |
| deptrace | Disables ptrace() |
| derand | Disables rand() and random() |
| desigact | Disables sigaction() |
| desock | Channels socket communication to the console |
| desock\_dup | Channels socket communication to the console (simpler method) |
| ensock | The opposite of desock -- like an LD\_PRELOAD version of socat! |
| desrand | Does tricky things with srand() to control randomness. |
| detime | Makes time() always return the same value. |
| desleep | Makes sleep() and usleep() do nothing. |
| mallocwatch | When ltrace is inconvenient, mallocwatch provides info on heap operations. |
| writeout | Some binaries write() to fd 0, expecting it to be a two-way socket. This makes that work (by redirecting to fd 1). |
| patch | Patches programs at load time. |
| decookie | Dumps stack canary on startup. |
| startstop | Sends SIGSTOP to itself on startup, to suspend the process. |
| crazyrealloc | ensures that whatever is being reallocated is always moved to a new location in memory, thus free()ing the old. |

## Building

preeny's patch functionality uses `libini_config` to read `.ini` files.

* On debian-based distros, you can install `libini-config-dev`.
* On Arch-based distros, you can install `ding-libs`.
* On Fedora-based distros, you can install `libini_config-devel`.

If you're not running a debian, Arch, or Fedora based distro, you've brought the pain upon yourself.

You can build preeny by doing:

    make

It'll create a directory named after the OS and architecture type, then put the libraries there.

## Cross-compilation

If you need to build 32-bit x86 preeny libs on a 64-bit x86 host, you can do:

    CFLAGS=-m32 make

Alternatively, if you want to utilize a cross-compiler, pass the `CC` variable to `make`.  For example:

    CC=mips-malta-linux-gnu-gcc make -i

Because some modules fail in cross-complilation, it's recommended to use `make -i`.

## Usage

Let's say that you have an application that you want to interact with on the commandline, but it a) forks, b) sets an alarm which makes it hard to take your time studying its behavior, and c) demands to be connected to even if you don't want to do that.
You can do:

```bash
LD_PRELOAD=x86_64-linux-gnu/desock.so:x86_64-linux-gnu/defork.so:x86_64-linux-gnu/dealarm.so \
  ~/code/security/codegate/2015/rodent/rodent
```

Pretty awesome stuff!  Of course, you can pick and choose which preloads you want:

```bash
echo 'No fork or alarm for you, but I still want to netcat!'
LD_PRELOAD=x86_64-linux-gnu/defork.so:x86_64-linux-gnu/dealarm.so ~/code/security/codegate/2015/rodent/rodent

echo 'Ok, go ahead and fork, but no alarm. Time to brute force that canary.'
LD_PRELOAD=x86_64-linux-gnu/dealarm.so ~/code/security/codegate/2015/rodent/rodent
```

Have fun!

## Simple Things

The simple functionality in preeny is disabling of fork and alarm.

CTF services frequently use alarm to help mitigate hung connections from players, but this has the effect of being frustrating when you're debugging the service.

Fork is sometimes frustrating because some tools are unable to follow fork on some platforms and, when they do follow fork, the parent is oftentimes abandoned in the background, needing to be terminated manually afterwards.

`dealarm.so` replaces `alarm()` with a function that just does a `return 0`.
`defork.so` does the same thing to `fork()`, means that the program will think that the fork has succeeded and that it's the child.

## Derandomization

It's often easiest to test your exploits without extra randomness, and then ease up on the cheating little by little.
Preeny ships with two modules to help: `derand` and `desrand`.

`derand.so` replaces `rand()` and `random()` and returns a configurable value. Just specify it in the RAND environment (or go with the default of 42):

```bash
# this will return 42 on each rand() call
LD_PRELOAD=x86_64-linux-gnu/derand.so tests/rand

# this will return 1337 on each rand() call
RAND=1337 LD_PRELOAD=x86_64-linux-gnu/derand.so tests/rand
```

For slightly more complex things, `desrand.so` lets you override the `srand` function to your liking.

```bash
# this simply sets the seed to 42
LD_PRELOAD=x86_64-linux-gnu/desrand.so tests/rand

# this sets the seed to 1337
SEED=1337 LD_PRELOAD=x86_64-linux-gnu/desrand.so tests/rand

# this sets the seed to such that the first "rand() % 128" will be 10
WANT=10 MOD=128 LD_PRELOAD=x86_64-linux-gnu/desrand.so tests/rand

# finally, this makes the *third* "rand() % 128" be 10
SKIP=2 WANT=10 MOD=128 LD_PRELOAD=x86_64-linux-gnu/desrand.so tests/rand
```

`desrand` does all this by brute-forcing the seed value, so keep in mind that startup speed will get considerably slower as `MOD` increases.

## De-socketing

Certain tools (such as American Fuzzy Lop, for example) are unable to handle network binaries.
Preeny includes two "de-socketing" modules.
`desock.so` neuters `socket()`, `bind()`, `listen()`, and `accept()`, making it return sockets that are, through hackish ways, synchronized to `stdin` and `stdout`.
`desock_dup.so` is a simpler version for programs that dup accepted sockets over file descriptors 0, 1, and 2.

A discussion of the different ways to de-socket program, and why Preeny does it the way it does, can be found [here](https://github.com/zardus/preeny/issues/10).

## En-socketing

You can also use preeny to turn a normal binary into a socket binary! Just set the `PORT` environment variable (default is 1337) and preload `ensock.so`!

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
# PATCH="hello.p" LD_PRELOAD=x86_64-linux-gnu/patch.so tests/hello 
--- section hello in file hello.p specifies 5-byte patch at 0x4005c4
--- section world in file hello.p specifies 5-byte patch at 0x4005ca
AAAAA aaaaa!

```

Having different patch files and just enabling/disabling them via preload is oftentimes easier than modifying the underlying binary.
