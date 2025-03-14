<div align=center>

# c_hooks - Shared object hooking
This repo explores use of shared object hooking using `LD_PRELOAD`

</div>

## Intro

### Shared vs static libaries?

- A static libary is a collection of `.obj` files containing the functionality of the library.
When you compile an executable with this static library it gets merged in with the object files of the executable resulting in the final ELF binary image.
This has a few benefits, its faster than a shared libary because the code is actually embedded in binary so it doens't have to deal with the linker.

- Shared (dynamic linked) libraries work a bit different, on glibc-based linux systems, starting up an ELF binary automatically causes the program loader to be loaded and run. This loader finds and loads all the other shared libaries used by the program.
It does all of this at *runtime!*, which is nice if you have a large libary that a lot of programs use. For example `glfw` or literally `glibc` that way you can save space in your image.
> It is important to note that none of the code from the shared libary is actually copied into the final executable. Instead, the linker copies some relocation and symbol table information that allows the loader to resolve the it at ***loadtime***.

### Loader?
When the loader loads and runs the partially linked executable, it looks for the `.interp` section in the [program header table](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format#Program_header), which contains the paths name of the dynamic linker. Instead of passing control to the application, as it would normally do, the loader loads and runs the dynamic linker. The dynamic linker then finishes the linking task by relocating the symbols and code.

You can read the `.interp` section with
```
readelf app -a | grep interp
```



### What shared libary to load?
[ELF](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format) is a common format for executables on the UNIX platform, it is made up of one ELF header followed by different sections.
We won't go too deep into it here but the point I wanted to touch on is the part where it stores its depencencies, which is in the `.dynamic` section.

You can read this section with the following command
```
readelf -d <myapp> | grep NEEDED
```
If you run this on any executable you will see `[libc.so.X]` show up.

The thing is the binary has no idea where `[libc.so.X]` is located, that is where the loader comes in, it will find the correct library for the program (if its installed).

You can check where to libarries are located at with.
```
ldd <myapp>
```

### Compilation
![Compilation](https://i.imgur.com/LNddTmk.png)



## Hooking functions
Cool, so now we know how dynamic libaries work, the realization is that apparently stuff gets linked on *runtime* which means we could hook onto that using our own shared library... 


Most programs will use the `write()` function from libc, lets try to hook onto that.

## Building a shared library
This is way simpler than you would expect. 

> NOTE: All `.so` file names should start with `lib` see [3.1.1](https://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html)


```
gcc -fPIC -shared -ldl libhook.c -o libhook.so
```
### Flags explained
#### `-fPIC`
Position Indepenend Code, we want this so the symbols are not located at a specific address, instead they are relative to the main code of the executable. We want this because we don't know what where in the load order our libary will be.


###### PIC
```
100: COMPARE REG1, REG2
101: JUMP_IF_EQUAL CURRENT+10
...
111: NOP

```
###### NON-PIC
```
100: COMPARE REG1, REG2
101: JUMP_IF_EQUAL 111
...
111: NOP
```


#### `-shared`
Produce a shared object which can then be linked with other objects to form an executable.

#### `-ldl`


## LD_PRELOAD
Lets address the elephant in the room, `LD_PRELOAD` is a configurable environment variable that allows users to specify a shared library to be loaded into memory for programs before other shared libraries.
When the loader runs your program it will look at the `LD_PRELOAD` env variable for any libaries it needs, these libaries will be loaded before the intended libary gets loaded. we can use that to have an executable link with our "libc" before linking with the intended one, allowing us to sit in the middle of them.

I've included an example in `basic-example` which is supposed to print "Hello World", however when you run it with the `LD_PRELOAD` set, it will replace the text passed into `write()`.
```
./app
Hello World
```

```
LD_PRELOAD=$(pwd)/libhook.so ./app
overwritten
```
To see verify its linking with our `libhook.so` run.
```
LD_PRELOAD=$(pwd)/libhook.so ldd app
```


## Logging to a file
In `logging-example` we log to a file.
T.B.A

## Logging to a file


## Hiding from netstat and lsof
T.B.A



## Hooking nginx?
T.B.A



## Resources
- [ELF Format Cheatsheet](https://gist.github.com/x0nu11byt3/bcb35c3de461e5fb66173071a2379779)
- [ELF Loaders, Libraries and Executables on Linux](https://dtrugman.medium.com/elf-loaders-libraries-and-executables-on-linux-e5cfce318f94)
- [Study of ELF loading and relocs](https://netwinder.osuosl.org/users/p/patb/public_html/elf_relocs.html)
- [UNIX Loader process](https://unix.stackexchange.com/a/50346)
- [Jynx2](https://github.com/chokepoint/Jynx2)
- [Learn-C-By-Creating-A-Rootkit](https://h0mbre.github.io/Learn-C-By-Creating-A-Rootkit/#)
- [revshells.com](https://www.revshells.com/)
- [gcc -fPIC option](https://stackoverflow.com/a/5311538)
- [-shared option](https://stackoverflow.com/a/25085725)


