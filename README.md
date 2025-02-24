## Intro
### Shared vs static libaries?

- A static libary is a collection of `.obj` files containing the functionality of the library.
When you compile an executable with this static library it gets merged in with the object files of the executable resulting in the final ELF binary image.
This has a few benefits, its faster than a shared libary because the code is actually embedded in binary so it doens't have to deal with the linker.

- Shared (dynamic linked) libraries work a bit different, on glibc-based linux systems, starting up an ELF binary automatically causes the program loader to be loaded and run. This loader finds and loads all the other shared libaries used by the program.
It does all of this at *runtime!*, which is nice if you have a large libary that a lot of programs use. For example `glfw` or literally `glibc` that way you can save space in your image.
> It is important to note that none of the code from the shared libary is actually copied into the final executable. Instead, the linker copies some relocation and symbol table information that allows the loader to resolve the it at *loadtime*.

When the loader loads and runs the partially linked executable, it notices it contains a `.interp` section, which contains the paths name of the dynamic linker. Instead of passing control to the application, as it would normally do, the loader loads and runs the dynamic linker. The dynamic linker then finishes the linking task by relocating the symbols and code 



### What shared libary to load?
The [ELF](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format) is a common format for executables on the linux platform, it is made up of one ELF header followed by the different sections.
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


```
ldd <file-name>
```

### Compilation
![Compilation](https://i.imgur.com/LNddTmk.png)



## Hooking functions
Cool, so now we know how dynamic libaries work, the realization is that apparently stuff gets linked on *runtime* which means we could hook onto that... 

Most programs will use the `write()` function from libc, lets try to hook onto that.


## LD_PRELOAD
When the loader runs your program it will look at the `LD_PRELOAD` env variable for any libaries it needs, these libaries will be loaded before the intended libary gets loaded. we can use that to have an executable link with our "libc" before linking with intended one, allowing us to sit in the middle of them.



## Logging to a file


## Hiding the executable





## Resources
[ELF Format Cheatsheet](https://gist.github.com/x0nu11byt3/bcb35c3de461e5fb66173071a2379779)
[UNIX Loader process](https://unix.stackexchange.com/a/50346)
