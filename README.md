libqtr3d - Cuter 3D with Qt
==========================
Target is to provide a simple Qt/C++ 3D Interface. Life's too short to learn shaders. And Qt3D has (IMHO) too much overhead. Qt delivers everything you need (QOpenGLWidget, QOpenGLShader). Why not having just a KISS-Interface for simple 3D-Apps?

- [x] Hello world in just a few lines of code.
- [x] Just Qt, OpenGL and C++. No extra dependencies.
- [x] Class-Library. Just include the profile. Step through the code.
- [ ] API unstable! Work in progress...

![libqtr3d](docs/showcase/iss-example.png)
![libqtr3d](docs/showcase/heightmap-example.png)
![libqtr3d](docs/showcase/pointcloudviewer.png)

First steps (after installing Qt)
---------------------------------
```
$ git clone https://github.com/solarix95/libqtr3d.git
$ cd libqtr3d
$ cd examples
$ ./runall.sh
$ cd ../apps/qtr3dviewer && qmake && make && ./qtr3dviewer
```

Project Roadmap
---------------------------------

| Version       | Features      | Status|
| ------------- |:-------------:| :-----:|
| 0.1           | static models, simple lighting, camera control, native GLB-Loader, basic animations | WIP |
| 0.2           | Skeletal animation       | WIP  |
| 0.3           | multiple lights, shadows | open |

3D Fileformat Compatibility
---------------------------------
| Format       | Implementation |  Maturity    |
| ------------ |:--------------:|  ------------|
| STL, ASCII   | native         |  High        |
| STL, Bin     | native         |  High        |
| GLB (glTF)   | native         |  High        |
| PLY          | native         |  Medium      |
| OBJ          | native         |  Low         |
| 3DS          | native         |  Low         |
| [ALL](https://github.com/assimp/assimp/blob/master/doc/Fileformats.md) | ASSIMP    |  High   |

3D Resources+Assets
---------------------------------
- https://nasa3d.arc.nasa.gov/models
- http://graphics.stanford.edu/data/3Dscanrep/
- http://www.semantic3d.net/
- https://www.solarsystemscope.com/textures/
- https://heightmap.skydark.pl/

