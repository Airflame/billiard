echo "Building server"
g++ server.cpp src/*.cpp -o server -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system
echo "Building client"
g++ client.cpp src/*.cpp -o client -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system
echo "Done"
