## Writing a shader

A shader is usually made up of a vertex shader and a fragmet sha
ader
- create a vertex buffer and a vertex array
- define vertex attributes
- write a shader to tell how to interpret the buffer and draw a tringle
    - vertex shader: gets called for exach vertex (3 times for a triangle)
    tell opengl where in your window the vertex position needs to be
    - fragment or pixel shader: gets called once for each pixels that has to be 
    rasterized. Decide which color that pixel needs to be
