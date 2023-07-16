# TODO List

**DONE:**
- Type casting from vector to matrices [x]
- Matrix Expansion from (n,m) to (n', m') where n' >= n && m' >= m [x]
- Figure out how to draw in 3D --> Perspective Projection [x]
    - (Left, Bottom, Near) and (Right, Top, Far) are the bottom left and top right vertices, respectively, of the Orthographic Viewing Volume.
    - With an Orthographic Projection Matrix, we can project the objects in the Orthographic View Volume to the Canonical View Volume.
    - We then need to project each object using a perspective transformation.
    - Homogenous vector <x,y,z,w> corresponds to the position vector <x/w, y/w, z/w>.
    - Combining the Orthographic Projection and Perspective Matrix gives the Perspective Projection Matrix.
    - From NDC -> Screen: Multiply by (width/2, height/2) + (width/2, height/2) --> (x + 1) * (width/2, height/2)

**TODO:**
- Rewrite using Smart Pointers
- Implement matrices which hold functions?
- Forward Kinematics
    - Implement Limbs & Joints
- Camera Movement
- Shapes Library? --> Is it required? Yes.
- Fill matrix using some function f(i,j) => M(i,j) = f(i,j) where T f(u64 i, u64 j)
- Write (some) Documentation (~3 hrs)
- Triangles
- Fix problem regarding drawing 3D circles
