## vertex arrays

are opengl special. Way to bind vertex buffer with a certain kind of 
specifications (layout of that specific vertex buffer)

what we currently have to do is:

- bind out shader
- we set up their uniform 
- we bind up vertex buffer
- we set up the layout of that vertex buffer
- we finally bind their index buffer
- then we call gldrawelements

if we introduce the vertex array this changes to:

- bind out shader
- we set up their uniform 
- bind vertex array 
- bind index buffer
- then we call gldrawelements

technically speaking vertex arrays are mandatory 
(the compatibility profile creates a vertex array by default, 
the core profile does not)