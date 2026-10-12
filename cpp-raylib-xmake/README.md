# Xmake-Raylib-CPP Starter Project.
This is the starter project that you download when you use Fe init xmake raylib cpp before the edits.


## Somethings you need to know

The named `Exe` is the name of the target in the `xmake.lua` file. Change it to whatever and rebuild.

To build use `xmake` in a CMD/Term.

The `gitignore` only handles the folder `.xmake` and `buld` nothing else.

Your build is found in `build/<OS>/<Arch>/release/<whatever you put in target>.exe`.


## For FE users

The tool should have renamed your target in xmake, and your top folder. If for whatever reason (Probably a prems issue) it didn't you want do that yourself. 

Raylib is included at installed at the first time you run `xmake`. 

You don't need to add files to `xmake` ever. It reads all `Cpp` files in `Src`, and all `.h` files in include. 

## If you want to add libs

If `xrepo` has it then add it to the `xmake.lua`, and if not and it's a single header or something light please throw it in your `src/include` and compile.
Do not add a complex depend setup for a single header.
