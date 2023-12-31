
## Usage

```shell
mkdir build
cd build
cmake ..

make
./Rasterizer
```

The procedure will generate a rectangle, you can press `A` or `D` to rotate the rectangle into left/right direction.  
You can press `esc` to end the procedure.  

You are invited to change the rotation axis in the procedure, and this method will change the perspective projection of the rectangle. Just changing the coordinate of parameter vertex `axis` (defined in `main()`) to achieve above method.