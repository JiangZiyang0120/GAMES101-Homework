
## Usage

```shell
mkdir build
cd build
cmake ..

make
./Rasterizer
```

The procedure will generate a rectangle, you can press `A` or `D` to rotate the rectangle into left/right direction.  
You can press `Esc` to end the procedure.  

You are invited to change the rotation axis in the procedure, and this method will change the perspective projection of the rectangle. Just changing the macro value `X_AXIS`, `Y_AXIS` and `Z_AXIS` defined in `main.cpp` and recompiling to achieve above method.
