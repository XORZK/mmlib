# TODO List

## DONE
- [x] Fix problem regarding drawing 3D circles 
- [x] Triangles 
- [x] Type casting from vector to matrices 
- [x] Matrix Expansion from (n,m) to (n', m') where n' >= n && m' >= m 
- [x] Fill matrix using some function f(i,j) => M(i,j) = f(i,j) where T f(u64 i, u64 j) 
- [x] Figure out how to draw in 3D --> Perspective Projection 
    - (Left, Bottom, Near) and (Right, Top, Far) are the bottom left and top right vertices, respectively, of the Orthographic Viewing Volume.
    - With an Orthographic Projection Matrix, we can project the objects in the Orthographic View Volume to the Canonical View Volume.
    - We then need to project each object using a perspective transformation.
    - Homogenous vector <x,y,z,w> corresponds to the position vector <x/w, y/w, z/w>.
    - Combining the Orthographic Projection and Perspective Matrix gives the Perspective Projection Matrix.
    - From NDC -> Screen: Multiply by (width/2, height/2) + (width/2, height/2) --> (x + 1) * (width/2, height/2)
- [x] Draw Polygons --> Triangulation (Ear Clipping Method => O(n^3) -> O(n^2)) 
    - Sort vertices --> Clockwise? Counter-Clockwise? (Change from Bubble to Merge Later)
    - Triangulation can be done recursively until only one triangle remains.
    - Remember to switch back from void to an actual valid return type.
- [x] Polygon Triangulation doesn't work? Fix Barycentric Coordinates. 
- [x] Camera Movement 
- [x] Bezier Curves 
- [x] Bezier Curves (Extra Functions => Translation, Rotation...) => Move from vec2 to vec3 
- [x] Convex Hull for Polygon? 

## TODO
High Priority:
- [ ] Forward Kinematics
    - Implement Limbs & Joints

Low Priority:
- [ ] Rewrite using Smart Pointers
- [ ] Write (some) Documentation (~3 hrs)
