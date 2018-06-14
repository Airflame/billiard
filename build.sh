echo "Building server"
g++ server.cpp src/*.cpp -o bin/server -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system
echo "Building client"
g++ client.cpp src/*.cpp -o bin/client -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system
echo "Done"
