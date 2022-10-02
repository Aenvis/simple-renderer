# tiny-renderer-cpp-only

It is a project that helps me to understand how computer graphics API like openGL works. It is written in c++ using minimum of external dependencies(no openGL, SFML etc.).<br>
To work it through I follow [these lessons](https://github.com/ssloy/tinyrenderer/wiki).<br>

In main function of the program you can provide path to the .obj file. Program then parses given data and retrieves info about vertices and faces. <br>
That data is used to render a rasterized mesh of the object. Output is then saved to TGA format file. Example output:

![example output](https://user-images.githubusercontent.com/76266906/193473169-3b4e22ec-6a39-4772-a124-dd670bc24552.png)
