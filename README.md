# OS-101 project

- bash commision denied error
```bash
    // at the root directory
    chmod -R 755 ./*    
```

- build and run the program
(when now path is ~/github/pintos)
```bash
cd threads
make
```
then path is changed to ~/github/pintos/threads
```bash
cd build
../utils/pintos automated_warehouse 2 1A:3B // for example
```
- to use git
```
git add *
git commit -m "anywords"
git push
```

- thread.c
```c
thread_create (const char *name, int priority,
               thread_func *function, void *aux) 
```