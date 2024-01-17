## start using modern opengl
- check for errors in opengl using opengl glGetError(), to be called inside a loop, gives us all flags of errors tracked
    - we can check clear erros before a line, and then checkerrors after or
    - define a macro GLCall() that:
        - clear errors
        - execute line
        - assert checkerrors and prints out useful info about it
