## Errors

Check for errors in opengl using opengl glGetError(), to be called inside a loop, gives us all the flags of tracked errors.

There are two ways to check errors
- we can check clear erros before a line, and then checkerrors after or
- define a macro GLCall() that:
    - clear errors
    - execute line
    - assert checkerrors and prints out useful info about it
