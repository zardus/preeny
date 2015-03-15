# preeny

Preeny helps you pwn noobs by making it easier to interact with services locally.
It disables `fork()` and `alarm()` and, if you want, can even convert a server application to a console one using clever/hackish tricks!

## Building

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
As a bonus, this should let you pass socket applications to AFL!

Of course, you can pick and choose which preloads you want:

```bash
echo 'No fork or alarm for you, but I still want to netcat!'
LD_PRELOAD="Linux_x86_64/defork.so Linux_x86_64/dealarm.so" ~/code/security/codegate/2015/rodent/rodent

echo 'Ok, go ahead and fork, but no alarm. Time to brute force that canary.'
LD_PRELOAD="Linux_x86_64/dealarm.so" ~/code/security/codegate/2015/rodent/rodent
```

Have fun!
