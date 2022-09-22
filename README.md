# tiny-renderer-cpp-only

It is a project that helps me to understand how computer graphics API like openGL works. It is written in c++ using minimum of external dependencies(no openGL, SFML etc.).<br>
To work it through I follow [these lessons](https://github.com/ssloy/tinyrenderer/wiki).<br>

In main function of the program you can provide path to the .obj file. Program then parses given data and retrieves info about vertices and faces. <br>
That data is used to render a wired mesh of the object. For now it's an ortographic perspective(ignoring z coordinate). Output is then saved to TGA format file. Example output:

![example output](https://raw.githubusercontent.com/Aenvis/tiny-renderer-cpp-only/master/tiny-renderer/output/output.png)
